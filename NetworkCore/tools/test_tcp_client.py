"""Exercise the client against real, local TCP peers; no C++ changes required."""

import socket
import struct
import subprocess
import sys
import threading
import time
import unittest
from pathlib import Path


CLIENT = Path(__file__).with_name("tcp_client.py")


def receive_exact(sock, size):
    data = bytearray()
    while len(data) < size:
        chunk = sock.recv(size - len(data))
        if not chunk:
            raise AssertionError("Client closed before sending all expected bytes")
        data.extend(chunk)
    return bytes(data)


class ClientTests(unittest.TestCase):
    def run_peer(self, handler, *arguments, stdin=None):
        failures = []
        with socket.socket() as listener:
            listener.bind(("127.0.0.1", 0))
            listener.listen(1)
            listener.settimeout(1.0)
            port = listener.getsockname()[1]

            def serve():
                try:
                    with listener.accept()[0] as connection:
                        connection.settimeout(2.0)
                        handler(connection)
                except Exception as error:
                    failures.append(error)

            worker = threading.Thread(target=serve, daemon=True)
            worker.start()
            result = subprocess.run(
                [sys.executable, "-B", str(CLIENT), "--port", str(port),
                 "--wait", "0.5", *arguments],
                input=stdin, capture_output=True, text=True, timeout=5,
            )
            worker.join(3)
            self.assertFalse(worker.is_alive(), "Test peer did not finish")
        return result, failures

    def assert_success(self, result, failures):
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertEqual(failures, [])

    def test_raw_round_trip(self):
        def peer(sock):
            self.assertEqual(receive_exact(sock, 3), b"abc")
            sock.sendall(b"abc")

        result, failures = self.run_peer(peer, "--send", "abc")
        self.assert_success(result, failures)
        self.assertIn("received_bytes=3", result.stdout)

    def test_framed_batch_and_fragmented_replies(self):
        # Fixed wire fixtures independently specify endian order and body length.
        first = bytes.fromhex("fccd 00000003 00000007 00000001 00000000 616263")
        second = bytes.fromhex("fccd 00000003 00000007 00000002 00000000 646566")

        def peer(sock):
            self.assertEqual(receive_exact(sock, len(first + second)), first + second)
            for chunk in [first[:1], first[1:17], first[17:] + second[:8], second[8:]]:
                sock.sendall(chunk)
                time.sleep(0.005)

        result, failures = self.run_peer(
            peer, "--mode", "frame", "--msg-id", "7", "--seq", "1",
            "--send", "abc", "--send", "def", "--batch",
        )
        self.assert_success(result, failures)
        self.assertIn("msg_id=7 seq=1 player_id=0", result.stdout)
        self.assertIn("msg_id=7 seq=2 player_id=0", result.stdout)
        self.assertIn("received_frames=2", result.stdout)

    def test_split_send_and_binary_payload(self):
        def peer(sock):
            data = receive_exact(sock, 5)
            self.assertEqual(data, bytes.fromhex("00 ff 01 80 02"))
            sock.sendall(data)

        result, failures = self.run_peer(
            peer, "--hex", "--send", "00 ff 01 80 02",
            "--chunk-size", "2", "--chunk-delay", "0.005",
        )
        self.assert_success(result, failures)
        self.assertEqual(result.stdout.count("[send chunk]"), 3)
        self.assertIn("received_bytes=5", result.stdout)

    def test_interactive_input_and_eof(self):
        def peer(sock):
            self.assertEqual(receive_exact(sock, 3), b"abc")
            sock.sendall(b"abc")

        result, failures = self.run_peer(peer, stdin="abc\n")
        self.assert_success(result, failures)
        self.assertIn("received_bytes=3", result.stdout)

    def test_invalid_magic(self):
        def peer(sock):
            receive_exact(sock, 19)
            sock.sendall(struct.pack("!HIIII", 0, 0, 1, 1, 0))

        result, failures = self.run_peer(peer, "--mode", "frame", "--send", "a")
        self.assertEqual(failures, [])
        self.assertEqual(result.returncode, 1)
        self.assertIn("invalid magic", result.stdout + result.stderr)

    def test_oversize_incoming_frame(self):
        def peer(sock):
            receive_exact(sock, 19)
            sock.sendall(struct.pack("!HIIII", 0xFCCD, 16385, 1, 1, 0))

        result, failures = self.run_peer(peer, "--mode", "frame", "--send", "a")
        self.assertEqual(failures, [])
        self.assertEqual(result.returncode, 1)
        self.assertIn("body length", result.stdout + result.stderr)

    def test_truncated_frame(self):
        def peer(sock):
            receive_exact(sock, 19)
            sock.sendall(b"\xfc\xcd\x00")

        result, failures = self.run_peer(peer, "--mode", "frame", "--send", "a")
        self.assertEqual(failures, [])
        self.assertEqual(result.returncode, 1)
        self.assertIn("incomplete frame", result.stdout + result.stderr)

    def test_no_response_timeout(self):
        def peer(sock):
            receive_exact(sock, 3)
            time.sleep(0.2)

        result, failures = self.run_peer(peer, "--send", "abc", "--wait", "0.05")
        self.assertEqual(failures, [])
        self.assertEqual(result.returncode, 1)
        self.assertIn("no response", result.stdout + result.stderr)

    def test_send_only(self):
        def peer(sock):
            self.assertEqual(receive_exact(sock, 3), b"abc")

        result, failures = self.run_peer(peer, "--send", "abc", "--wait", "0")
        self.assert_success(result, failures)
        self.assertIn("send-only", result.stdout)


if __name__ == "__main__":
    unittest.main()
