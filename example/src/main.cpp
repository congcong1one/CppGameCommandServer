#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <unordered_map>

#include "Player.hpp"
static std::unordered_map<int, Player> players;

int main() {
    boost::asio::io_context io;

    io.run();

    return 0;
}