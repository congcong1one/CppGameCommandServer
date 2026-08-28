#include <boost/asio.hpp>
class network {
    network(boost::asio::io_context& io) : _acceptor(io) {}

    void init(const std::string& address, unsigned short port) {
        _acceptor.open(boost::asio::ip::tcp::v4());
        _acceptor.bind(boost::asio::ip::tcp::endpoint(
            boost::asio::ip::make_address(address), port));
        _acceptor.listen();
        _acceptor.async_accept([this](boost::system::error_code ec,
                                      boost::asio::ip::tcp::socket socket) {
            if (!ec) {
                // Handle the accepted connection
            }
            // Continue accepting new connections
        });
    }
    boost::asio::ip::tcp::acceptor _acceptor;
};