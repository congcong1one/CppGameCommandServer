#include <boost/asio.hpp>

class Session {
    Session() = default;
    Session(boost::asio::ip::tcp::socket socket) : _socket(std::move(socket)) {}

private:
    boost::asio::ip::tcp::socket _socket;
};