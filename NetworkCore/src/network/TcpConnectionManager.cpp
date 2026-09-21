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
    std::unique_ptr<TcpConnection> connection) {
    int bucketIndex = getNextConnectionId() % bucketCount;
    _connectionBuckets[bucketIndex][getNextConnectionId()] =
        std::move(connection);
}
void TcpConnectionManager::removeConnection(int connectionId) {
    int bucketIndex = connectionId % bucketCount;
    _connectionBuckets[bucketIndex].erase(connectionId);
}