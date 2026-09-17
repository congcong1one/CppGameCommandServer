#include "Session.hpp"

#include "Player.hpp"
static std::unordered_map<int, std::shared_ptr<Player>> onlinePlayers;
static std::unordered_map<int, std::shared_ptr<Player>> cachePlayers;

Session::Session(boost::asio::ip::tcp::socket socket, int sessionId,
                 PlayerManager& playerManager)
    : _socket(std::move(socket)), id(sessionId), _playerManager(playerManager) {
    _saveBuffer.data = std::vector<char>(MAX_BUFFER_SIZE);
    _saveBuffer.head = 0;
    _saveBuffer.size = 0;
}

bool Session::BindPlayer(int PlayerId) {
    if (onlinePlayers.find(PlayerId) != onlinePlayers.end()) {
        _player = onlinePlayers[PlayerId];
    } else {
        auto player = std::make_shared<Player>(PlayerId);
        onlinePlayers[PlayerId] = player;
        _player = player;
    }

    return true;
}

void Session::closeSession() {
    if (_bClosed) {
        return;  // Session is already closed
    }
    boost::system::error_code ec;
    _socket.close(ec);
    this->_player->lastTime =
        std::time(nullptr);  // Record the last time the player was active
    _playerManager.playerOffline(this->_player->getId());
    if (ec) {
        std::cerr << "Error closing socket: " << ec.message() << std::endl;
    }
    _bClosed = true;
}

bool Session::saveBuffer(std::array<char, MAX_BUFFER_SIZE>& buffer,
                         size_t length, Buffer& _saveBuffer) {
    if (_saveBuffer.data.size() - _saveBuffer.size < length) {
        closeSession();
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

void Session::doRead() {
    if (_bClosed) {
        return;  // Session is closed, stop reading
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

void Session::resumeBuffer() {
    while (true) {
    }
}