#pragma once
class TcpConnection;
#include <memory>
class Player {
public:
    Player(int id) : _id(id) {}
    int getId() const { return _id; }

    inline static int PlayerIdNext = 10000;
    int getNextPlayerId() { return PlayerIdNext++; }
    int getConnectionId() const;
    int lastTime;

private:
    int _id;
    std::weak_ptr<TcpConnection>
        _connection;  // Use weak_ptr to avoid circular reference
};