#pragma once
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>
class TcpConnection;
class TcpConnectionManager {
public:
    TcpConnectionManager(int initialConnectionId = 10000, int bucketCount = 4);
    ~TcpConnectionManager();

    void addConnection(std::unique_ptr<TcpConnection> connection);
    void removeConnection(int connectionId);
    void handleMessages();
    int getNextConnectionId();

private:
    // std::unordered_map<int, std::shared_ptr<TcpConnection>> _connections;
    std::mutex _connectionsMutex;
    int connectionIdNext;
    int bucketCount;
    std::vector<std::unordered_map<int, std::unique_ptr<TcpConnection>>>
        _connectionBuckets;
};