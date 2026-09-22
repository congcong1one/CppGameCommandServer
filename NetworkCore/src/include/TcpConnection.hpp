#pragma once
#include <boost/asio.hpp>
#include <deque>
#include <vector>

#include "entity/Buffer.hpp"

constexpr int MAX_BUFFER_SIZE = 1024 * 8;  // 8KB buffer size
constexpr int MAX_SAVE_BUFFER_SIZE =
    MAX_BUFFER_SIZE * 4;  // 16KB save buffer size
constexpr int MAX_MESSAGE_REMAINING_SIZE = MAX_BUFFER_SIZE * 2;
class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
    TcpConnection(boost::asio::ip::tcp::socket socket, int connectionId);
    int getId() const { return id; }
    boost::asio::ip::tcp::socket& getSocket();
    void closeConnection();
    void doRead();

private:
    void doWrite();
    bool saveBuffer(std::array<char, MAX_BUFFER_SIZE>& buffer, size_t length,
                    Buffer& saveBuffer);
    void resumeBuffer();
    boost::asio::ip::tcp::socket _socket;
    int id;
    Buffer _recvBuffer;
    std::deque<std::vector<char>> _sendBuffer;
    std::array<char, MAX_BUFFER_SIZE> _buffer;
    bool _isReading = false;
    bool _bClosed = false;
};