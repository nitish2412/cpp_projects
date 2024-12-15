#ifndef WEBSOCKET_SERVER_HPP
#define WEBSOCKET_SERVER_HPP

#include "../hpp/ClientManager.hpp"
#include <boost/asio.hpp>
#include <boost/beast.hpp>

class WebSocketServer {
public:
    WebSocketServer(asio::io_context& ioc, uint16_t port);
    void run();
    ClientManager& getClientManager();

private:
    void acceptConnections();
    void handleClient(std::shared_ptr<Client> client);

    asio::ip::tcp::acceptor acceptor_;
    ClientManager clientManager_;
};

#endif // WEBSOCKET_SERVER_HPP
