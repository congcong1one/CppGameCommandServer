#pragma once
#include "Player.hpp"
#include "TcpConnection.hpp"

int Player::getConnectionId() const {
    if (auto connection = _connection.lock()) {
        return connection->getId(); // Assuming TcpConnection has a public member 'id'
    }
    return -1; // Return -1 or some invalid value if the connection is expired
}