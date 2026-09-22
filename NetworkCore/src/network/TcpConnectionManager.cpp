#include "TcpConnectionManager.hpp"

int TcpConnectionManager::getNextConnectionId() {
    // 可优化
    std::lock_guard<std::mutex> _lock(_connectionsMutex);
    if (connectionIdNext >= INT32_MAX) {
        connectionIdNext = 10000;  // Reset to a safe starting point
    }

    return connectionIdNext++;
}

TcpConnectionManager::TcpConnectionManager(int initialConnectionId,
                                           int bucketCount)
    : connectionIdNext(initialConnectionId),
      bucketCount(bucketCount),
      _connectionBuckets(bucketCount) {}

void TcpConnectionManager::addConnection(
    std::shared_ptr<TcpConnection> connection) {
    int id = getNextConnectionId();
    int bucketIndex = id % bucketCount;
    _connectionBuckets[bucketIndex][id] = std::move(connection);
}
std::shared_ptr<TcpConnection> TcpConnectionManager::createAndAddConnection(
    boost::asio::ip::tcp::socket _socket) {
    int id = getNextConnectionId();
    auto connection = std::make_shared<TcpConnection>(
        std::move(_socket),
        id);  // Assuming playerId is 1 for this example
    int bucketIndex = id % bucketCount;
    _connectionBuckets[bucketIndex][id] = connection;
    return connection;
}
void TcpConnectionManager::removeConnection(int connectionId) {
    int bucketIndex = connectionId % bucketCount;
    _connectionBuckets[bucketIndex].erase(connectionId);
}

TcpConnectionManager::~TcpConnectionManager() {}