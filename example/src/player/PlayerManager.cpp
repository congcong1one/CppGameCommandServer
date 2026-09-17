#include "PlayerManager.hpp"

void PlayerManager::playerOffline(int playerId) {
    auto it = onlinePlayers.find(playerId);
    if (it != onlinePlayers.end()) {
        auto player = it->second;
        player->lastTime =
            time(nullptr);  // Record the last time the player was active
        cachePlayers[playerId] = player;  // Move the player to cachePlayers
        onlinePlayers.erase(it);  // Remove the player from onlinePlayers
    }
}

void PlayerManager::removeOnlinePlayer(int playerId) {
    onlinePlayers.erase(playerId);
}

// 可以考虑分片？
void PlayerManager::checkPlayerTimeouts(int timeoutSeconds) {
    auto now = time(nullptr);
    for (auto it = onlinePlayers.begin(); it != onlinePlayers.end();) {
        if (now - it->second->lastTime > timeoutSeconds) {
            cachePlayers[it->first] = it->second;
            it = onlinePlayers.erase(it);
        } else {
            ++it;
        }
    }
}