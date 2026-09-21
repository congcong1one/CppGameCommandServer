#include "network.hpp"

#include <atomic>
#include <memory>

#include "TcpConnectionManager.hpp"

static std::unordered_map<int, std::shared_ptr<TcpConnection>> connections;

void network::init(const std::string& address, unsigned short port) {
    _acceptor.open(boost::asio::ip::tcp::v4());
    _acceptor.bind(boost::asio::ip::tcp::endpoint(
        boost::asio::ip::make_address(address), port));
    _acceptor.listen();
}

void network::doAccept() {
    _acceptor.async_accept([this](boost::system::error_code ec,
                                  boost::asio::ip::tcp::socket socket) {
        if (!ec) {
            // Handle the accepted connection
            int newConnectionId = _connectionManager.getNextConnectionId();
            auto connection = std::make_unique<TcpConnection>(
                std::move(socket),
                newConnectionId);  // Assuming playerId is 1 for this example
            _connectionManager.addConnection(std::move(connection));
        }
        // Continue accepting new connections
        this->doAccept();
    });
}
