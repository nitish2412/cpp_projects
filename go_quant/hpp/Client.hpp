#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <boost/beast.hpp>
#include <set>
#include <string>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

// Represents a single WebSocket client
class Client {
public:
    websocket::stream<tcp::socket> ws;
    std::set<std::string> subscribedSymbols;

    Client(tcp::socket&& socket);
};

#endif // CLIENT_HPP
