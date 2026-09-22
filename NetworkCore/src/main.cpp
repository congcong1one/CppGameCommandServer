#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>

#include "network.hpp"

int main() {
    boost::asio::io_context io;
    network net(io);
    net.init("127.0.0.1", 9981);
    net.doAccept();

    io.run();

    return 0;
}