#ifndef PLAYER_MANAGER_HPP
#define PLAYER_MANAGER_HPP

#include <memory>
#include <unordered_map>

#include "Player.hpp"

class PlayerManager {
public:
    std::shared_ptr<Player> getOnlinePlayer(int playerId);
    std::shared_ptr<Player> getCachePlayer(int playerId);
    void addOnlinePlayer(std::shared_ptr<Player> player);
    void removeOnlinePlayer(int playerId);

    void addCachePlayer(std::shared_ptr<Player> player);
    void removeCachePlayer(int playerId);

    void playerOffline(int playerId);

    void checkPlayerTimeouts(int timeoutSeconds);

private:
    std::unordered_map<int, std::shared_ptr<Player>> onlinePlayers;
    std::unordered_map<int, std::shared_ptr<Player>> cachePlayers;
};

#endif  // PLAYER_MANAGER_HPP