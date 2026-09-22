#include "network.hpp"

#include <atomic>
#include <memory>
#include <utility>

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
            auto connection =
                _connectionManager.createAndAddConnection(std::move(socket));
            connection->doRead();  // Start reading from the socket
            // Continue accepting new connections
        }
        this->doAccept();
    });
}
