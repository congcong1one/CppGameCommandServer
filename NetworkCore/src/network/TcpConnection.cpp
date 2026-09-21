#include "TcpConnection.hpp"

#include <iterator>

#include "Player.hpp"
#include "Tool.hpp"
const size_t PACKET_HEADER_SIZE = sizeof(uint16_t) + sizeof(uint32_t) +
                                  sizeof(uint32_t) + sizeof(uint32_t) +
                                  sizeof(uint32_t);
TcpConnection::TcpConnection(boost::asio::ip::tcp::socket socket,
                             int connectionId)
    : _socket(std::move(socket)), id(connectionId) {
    _saveBuffer.data = std::vector<char>(MAX_BUFFER_SIZE);
    _saveBuffer.head = 0;
    _saveBuffer.size = 0;
}
const uint16_t _magic =
    0xFCCD;  // Example magic number, replace with your actual value
// bool TcpConnection::BindPlayer(int PlayerId) {
//     if (PlayerId <= 0) {
//         return false;  // Invalid PlayerId
//     }
//     if (_playerManager.getOnlinePlayer(PlayerId)) {
//         _player = _playerManager.getOnlinePlayer(PlayerId);
//     } else {
//         auto player = std::make_shared<Player>(PlayerId);
//         _playerManager.addOnlinePlayer(PlayerId, player);
//         _player = player;
//     }

//     return true;
// }

void TcpConnection::closeConnection() {
    if (_bClosed) {
        return;  // TcpConnection is already closed
    }
    boost::system::error_code ec;
    _socket.close(ec);
    if (ec) {
        std::cerr << "Error closing socket: " << ec.message() << std::endl;
    }
    _bClosed = true;
}

bool TcpConnection::saveBuffer(std::array<char, MAX_BUFFER_SIZE>& buffer,
                               size_t length, Buffer& _saveBuffer) {
    if (_saveBuffer.data.size() - _saveBuffer.size < length) {
        closeConnection();
        return false;
    }
    int endIndex =
        (_saveBuffer.head + _saveBuffer.size) % _saveBuffer.data.size();
    if (endIndex >= _saveBuffer.head) {
        if (endIndex + length <= _saveBuffer.data.size()) {
            std::copy(buffer.begin(), buffer.begin() + length,
                      _saveBuffer.data.begin() + endIndex);
        } else {
            size_t firstPartSize = _saveBuffer.data.size() - endIndex;
            std::copy(buffer.begin(), buffer.begin() + firstPartSize,
                      _saveBuffer.data.begin() + endIndex);
            std::copy(buffer.begin() + firstPartSize, buffer.begin() + length,
                      _saveBuffer.data.begin());
        }
    } else {
        std::copy(buffer.begin(), buffer.begin() + length,
                  _saveBuffer.data.begin() + endIndex);
    }
    _saveBuffer.size += length;
    return true;
}

void TcpConnection::doRead() {
    if (_bClosed) {
        return;  // TcpConnection is closed, stop reading
    }
    _socket.async_read_some(
        boost::asio::buffer(_buffer),
        [self = shared_from_this()](boost::system::error_code ec,
                                    size_t length) {
            if (!ec) {
                if (!self->saveBuffer(self->_buffer, length,
                                      self->_saveBuffer)) {
                    return;  // Buffer overflow, stop reading
                }
                self->doRead();  // Continue reading
            }
        });
}

void TcpConnection::resumeBuffer() {
    while (true) {
        if (_saveBuffer.size < PACKET_HEADER_SIZE) {
            return;  // Not enough data to process a message
        }
        PacketHeader header;
        header.magic = Tool::readRingBufferUint16(_saveBuffer);
        if (header.magic != _magic) {
            closeConnection();
            return;  // Invalid magic number, close the connection
        }
        header.length = Tool::readRingBufferUint32(_saveBuffer);
        if (header.length > MAX_MESSAGE_REMAINING_SIZE) {
            closeConnection();
            return;  // Invalid length, close the connection
        }
        if (_saveBuffer.size < PACKET_HEADER_SIZE + header.length) {
            return;  // Not enough data to process the complete message
        }
        header.msgId = Tool::readRingBufferUint32(_saveBuffer);
        header.seq = Tool::readRingBufferUint32(_saveBuffer);
        header.playerId = Tool::readRingBufferUint32(_saveBuffer);

        // header.body = Tool::readRingBuffer(_saveBuffer, header.length);

        // 投递消息入口，与消息层的交互处
    }
}