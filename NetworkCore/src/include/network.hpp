#include <boost/asio.hpp>

#include "TcpConnectionManager.hpp"
class network {
public:
    network(boost::asio::io_context& io) : _acceptor(io) {}

    void init(const std::string& address, unsigned short port);
    void doAccept();
    boost::asio::ip::tcp::acceptor _acceptor;
    TcpConnectionManager _connectionManager;
};