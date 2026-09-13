#pragma once
#include "Player.hpp"
#include "Session.hpp"

int Player::getSessionId() const {
    if (auto session = _session.lock()) {
        return session->getId(); // Assuming Session has a public member 'id'
    }
    return -1; // Return -1 or some invalid value if the session is expired
}