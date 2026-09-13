#include "network.hpp"

#include <atomic>
#include <memory>

#include "Session.hpp"

static std::unordered_map<int, std::shared_ptr<Session>> sessions;

static int sessionIdNext = 10000;
int getNextSessionId() { return sessionIdNext++; }

void network::init(const std::string& address, unsigned short port) {
    _acceptor.open(boost::asio::ip::tcp::v4());
    _acceptor.bind(boost::asio::ip::tcp::endpoint(
        boost::asio::ip::make_address(address), port));
    _acceptor.listen();
    
}

void network::doAccept(){
    _acceptor.async_accept([this](boost::system::error_code ec,
                       boost::asio::ip::tcp::socket socket) {
        if (!ec) {
            // Handle the accepted connection
            int newSessionId = getNextSessionId();
            auto session = std::make_shared<Session>(
                std::move(socket),
                newSessionId);  // Assuming playerId is 1 for this example
            sessions[newSessionId] = session;
        }
        // Continue accepting new connections
        this->doAccept();
    });
}




