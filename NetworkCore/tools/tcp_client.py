#!/usr/bin/env python3
"""Interactive/one-shot TCP client using only the Python standard library.

Frame wire format: magic:u16, body_length:u32, msg_id:u32, seq:u32,
player_id:u32, body bytes. Integers use network byte order; header size is 18.
"""

import argparse
import math
import socket
import struct
import sys
import threading
import time


HEADER = struct.Struct("!HIIII")
MAGIC = 0xFCCD
MAX_BODY = 16 * 1024


def log(message):
    print(message, flush=True)


def preview(data):
    sample = data[:64]
    suffix = " ... (preview truncated)" if len(data) > len(sample) else ""
    return f"hex={sample.hex(' ')} text={sample.decode('utf-8', errors='replace')!r}{suffix}"


class Receiver(threading.Thread):
    def __init__(self, sock, mode):
        super().__init__(daemon=True)
        self.sock = sock
        self.mode = mode
        self.stop_requested = threading.Event()
        self.done = threading.Event()
        self.pending = bytearray()
        self.received_bytes = 0
        self.received_frames = 0
        self.error = None

    def run(self):
        try:
            while not self.stop_requested.is_set():
                try:
                    data = self.sock.recv(65536)
                except socket.timeout:
                    continue
                if not data:
                    if not self.stop_requested.is_set():
                        log("[closed] peer closed the connection; press Enter if at the prompt")
                    break
                self.received_bytes += len(data)
                if self.mode == "raw":
                    log(f"[recv raw] bytes={len(data)} {preview(data)}")
                else:
                    self.pending.extend(data)
                    self.decode_frames()
        except (OSError, ValueError) as error:
            if not self.stop_requested.is_set():
                self.error = str(error)
                log(f"[error] {self.error}")
        finally:
            if self.pending and self.error is None:
                self.error = f"incomplete frame: {len(self.pending)} buffered bytes at end of receive"
                log(f"[error] {self.error}")
            self.done.set()

    def decode_frames(self):
        while len(self.pending) >= HEADER.size:
            magic, length, msg_id, seq, player_id = HEADER.unpack_from(self.pending)
            if magic != MAGIC:
                raise ValueError(f"invalid magic: expected 0x{MAGIC:04x}, got 0x{magic:04x}")
            if length > MAX_BODY:
                raise ValueError(f"body length {length} exceeds {MAX_BODY}")
            frame_size = HEADER.size + length
            if len(self.pending) < frame_size:
                return
            body = bytes(self.pending[HEADER.size:frame_size])
            del self.pending[:frame_size]
            self.received_frames += 1
            log(f"[recv frame] msg_id={msg_id} seq={seq} player_id={player_id} "
                f"body_bytes={length} {preview(body)}")


def uint32(value):
    try:
        number = int(value, 0)
    except ValueError as error:
        raise argparse.ArgumentTypeError("expected decimal or 0x-prefixed integer") from error
    if not 0 <= number <= 0xFFFFFFFF:
        raise argparse.ArgumentTypeError("expected an integer from 0 to 4294967295")
    return number


def parse_args():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--host", default="127.0.0.1", help="server host (default: 127.0.0.1)")
    parser.add_argument("--port", type=int, default=9981, help="server port (default: 9981)")
    parser.add_argument("--mode", choices=("raw", "frame"), default="raw",
                        help="raw bytes or project frame format (default: raw)")
    parser.add_argument("--send", action="append", help="send text; repeat for multiple messages; omit for interactive mode")
    parser.add_argument("--hex", action="store_true", help="interpret input as hex bytes instead of UTF-8")
    parser.add_argument("--msg-id", type=uint32, default=1)
    parser.add_argument("--seq", type=uint32, default=1, help="first sequence number; increments for each frame")
    parser.add_argument("--player-id", type=uint32, default=0)
    parser.add_argument("--batch", action="store_true", help="combine all --send messages before sending")
    parser.add_argument("--chunk-size", type=int, default=0, help="split sends into chunks of this many bytes; 0 disables splitting")
    parser.add_argument("--chunk-delay", type=float, default=0.1, help="seconds between chunks (default: 0.1)")
    parser.add_argument("--timeout", type=float, default=3.0, help="connection/socket I/O timeout in seconds")
    parser.add_argument("--wait", type=float, default=2.0,
                        help="receive window after --send or stdin EOF; 0 means send-only for --send")
    args = parser.parse_args()
    if not 1 <= args.port <= 65535:
        parser.error("--port must be between 1 and 65535")
    if args.chunk_size < 0:
        parser.error("--chunk-size must not be negative")
    for name in ("timeout", "wait", "chunk_delay"):
        value = getattr(args, name)
        if not math.isfinite(value) or value < 0 or (name == "timeout" and value == 0):
            parser.error(f"invalid --{name.replace('_', '-')}")
    if args.batch and args.send is None:
        parser.error("--batch requires --send")
    return args


def encode_messages(messages, args, first_seq):
    packets = []
    for offset, message in enumerate(messages):
        body = bytes.fromhex(message) if args.hex else message.encode("utf-8")
        if args.mode == "frame":
            if len(body) > MAX_BODY:
                raise ValueError(f"body length {len(body)} exceeds {MAX_BODY}")
            seq = first_seq + offset
            if seq > 0xFFFFFFFF:
                raise ValueError("sequence number would overflow uint32")
            packets.append(HEADER.pack(MAGIC, len(body), args.msg_id, seq, args.player_id) + body)
        else:
            packets.append(body)
    return packets


def send_packets(sock, packets, args):
    groups = [b"".join(packets)] if args.batch else packets
    for data in groups:
        chunk_size = args.chunk_size or max(1, len(data))
        for offset in range(0, len(data), chunk_size):
            if offset and args.chunk_delay:
                time.sleep(args.chunk_delay)
            chunk = data[offset:offset + chunk_size]
            sock.sendall(chunk)
            log(f"[send chunk] bytes={len(chunk)} {preview(chunk)}")
    log(f"[sent] messages={len(packets)} bytes={sum(map(len, packets))}")


def run(args):
    # Validate one-shot input before opening a connection.
    packets = encode_messages(args.send, args, args.seq) if args.send is not None else None
    receive_enabled = packets is None or args.wait > 0
    receiver = None
    sent_messages = 0
    with socket.create_connection((args.host, args.port), timeout=args.timeout) as sock:
        sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
        log(f"[connected] {args.host}:{args.port} mode={args.mode}")
        receiver = Receiver(sock, args.mode)
        if receive_enabled:
            receiver.start()
        try:
            if packets is not None:
                send_packets(sock, packets, args)
                sent_messages = len(packets)
                if receive_enabled:
                    receiver.done.wait(args.wait)
                else:
                    log("[send-only] no response was requested; this does not confirm server processing")
            else:
                log("Enter one message per line; /quit closes the connection. Replies appear asynchronously.")
                while not receiver.done.is_set():
                    try:
                        message = input("> ")
                    except EOFError:
                        receiver.done.wait(args.wait)
                        break
                    if message == "/quit" or receiver.done.is_set():
                        break
                    wire = encode_messages([message], args, args.seq + sent_messages)
                    send_packets(sock, wire, args)
                    sent_messages += 1
        finally:
            receiver.stop_requested.set()
            try:
                sock.shutdown(socket.SHUT_RDWR)
            except OSError:
                pass
            if receive_enabled:
                receiver.join()
    log(f"[summary] received_bytes={receiver.received_bytes} received_frames={receiver.received_frames}")
    if receiver.error:
        return 1
    if packets is not None and receive_enabled and receiver.received_bytes == 0:
        log(f"[error] no response within the receive window ({args.wait}s), or peer closed without replying")
        return 1
    return 0


def main():
    args = parse_args()
    try:
        return run(args)
    except KeyboardInterrupt:
        log("\n[closed] interrupted")
        return 130
    except (OSError, ValueError) as error:
        log(f"[error] {type(error).__name__}: {error}")
        return 1


if __name__ == "__main__":
    sys.exit(main())
