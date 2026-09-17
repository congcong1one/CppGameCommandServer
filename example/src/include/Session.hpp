#pragma once
#include <algorithm>
#include <boost/asio.hpp>
#include <iostream>

#include "PlayerManager.hpp"
class Player;

struct Buffer {
    std::vector<char> data;
    int head;
    int size;
};
constexpr int MAX_BUFFER_SIZE = 1024 * 8;  // 8KB buffer size
constexpr int MAX_SAVE_BUFFER_SIZE =
    MAX_BUFFER_SIZE * 4;  // 16KB save buffer size
constexpr int MIN_MESSAGE_REMAINING_SIZE = MAX_BUFFER_SIZE * 2;
class Session : public std::enable_shared_from_this<Session> {
    Session() = default;

public:
    Session(boost::asio::ip::tcp::socket socket, int sessionId,
            PlayerManager& playerManager);
    bool BindPlayer(int PlayerId);
    int getId() const { return id; }

private:
    void doRead();
    bool saveBuffer(std::array<char, MAX_BUFFER_SIZE>& buffer, size_t length,
                    Buffer& saveBuffer);
    void resumeBuffer();
    void closeSession();
    boost::asio::ip::tcp::socket _socket;
    std::shared_ptr<Player> _player;
    PlayerManager& _playerManager;
    int id;
    Buffer _saveBuffer;
    std::array<char, MAX_BUFFER_SIZE> _buffer;
    bool _isReading = false;
    bool _bClosed = false;
};