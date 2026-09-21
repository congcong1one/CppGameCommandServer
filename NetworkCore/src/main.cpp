#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <unordered_map>

#include "Player.hpp"

int main() {
    boost::asio::io_context io;

    io.run();

    return 0;
}