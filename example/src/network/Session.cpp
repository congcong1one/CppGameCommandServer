#include "Session.hpp"

#include <algorithm>

#include "Player.hpp"
static std::unordered_map<int, std::shared_ptr<Player>> players;


Session::Session(boost::asio::ip::tcp::socket socket, int sessionId)
    : _socket(std::move(socket)), id(sessionId) {
    _saveBuffer.data = std::vector<char>(MAX_BUFFER_SIZE);
    _saveBuffer.left = 0;
    _saveBuffer.right = 1;
}

bool Session::BindPlayer(int PlayerId) {
    if (players.find(PlayerId) != players.end()) {
        _player = players[PlayerId];
    } else {
        auto player = std::make_shared<Player>(PlayerId);
        players[PlayerId] = player;
        _player = player;
    }

    return true;
}

void Session::saveBuffer(std::array<char, MAX_SAVE_BUFFER_SIZE>& buffer, size_t length,
                         Buffer& _saveBuffer) {
    if (_saveBuffer.right > _saveBuffer.left) {
        size_t rightSize = _saveBuffer.data.size() - _saveBuffer.right;

        if (length > rightSize) {
            size_t leftSize = _saveBuffer.left;
            if (length > rightSize + leftSize) {
                _saveBuffer.data.resize(_saveBuffer.data.size() + length);
                saveBuffer(
                    buffer, length,
                    _saveBuffer);  // Recursively call to handle the new size
                return;

            } else {
                // Move the left part to the beginning of the buffer
                std::copy(buffer.begin(), buffer.begin() + rightSize - 1,
                          _saveBuffer.data.begin() + _saveBuffer.right);
                std::copy(buffer.begin() + rightSize, buffer.end(),
                          _saveBuffer.data.begin());
                _saveBuffer.right = buffer.size() - rightSize;
            }
        } else {
            std::copy(_saveBuffer.data.begin() + _saveBuffer.right,
                      _saveBuffer.data.begin() + _saveBuffer.right + length,
                      buffer.begin());
                      _saveBuffer.right += length;
        }
        if(_saveBuffer.right > _saveBuffer.left && _saveBuffer.data.size() - _saveBuffer.right + _saveBuffer.left > MIN_MESSAGE_REMAINING_SIZE) {
            // Handle the received data
            doRead();  // Continue reading
            
        }

        // Handle the received data
        _saveBuffer.data.insert(_saveBuffer.data.end(), buffer.begin(),
                                buffer.begin() + length);
    }else{
        int remainingSize = _saveBuffer.left - _saveBuffer.right;
        if (length > remainingSize) {
            
        }
    }
}

void Session::doRead() {
    _socket.async_read_some(
        boost::asio::buffer(_buffer),
        [self = shared_from_this()](boost::system::error_code ec,
                                    size_t length) {
            if (!ec) {
                if (self->_saveBuffer.right > self->_saveBuffer.left) {
                    size_t rightSize =
                        self->_saveBuffer.data.size() - self->_saveBuffer.right;

                    if (length > rightSize) {
                        size_t leftSize = self->_saveBuffer.left;
                        if (length > rightSize + leftSize) {
                            self->_saveBuffer.data.resize(
                                self->_saveBuffer.data.size() + length);
                            rightSize = self->_saveBuffer.data.size() -
                                        self->_saveBuffer.right;
                            leftSize = self->_saveBuffer.left;
                        }
                    }  // Handle the received data
                    self->_saveBuffer.data.insert(
                        self->_saveBuffer.data.end(), self->_buffer.begin(),
                        self->_buffer.begin() + length);
                    self->doRead();  // Continue reading
                }
            }
        });
}