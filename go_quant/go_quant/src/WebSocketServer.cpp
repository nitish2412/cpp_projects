#include "../hpp/WebSocketServer.hpp"
#include "../hpp/Client.hpp"
#include <boost/json.hpp>
#include <iostream>

namespace json = boost::json;

WebSocketServer::WebSocketServer(asio::io_context& ioc, uint16_t port)
    : acceptor_(ioc, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)), clientManager_() {}

void WebSocketServer::run() {
    acceptConnections();
}

ClientManager& WebSocketServer::getClientManager() {
    return clientManager_;
}

void WebSocketServer::acceptConnections() {
    acceptor_.async_accept([this](beast::error_code ec, tcp::socket socket) {
        if (!ec) {
            auto client = std::make_shared<Client>(std::move(socket));
            clientManager_.addClient(client);

            // Handle client in a new thread
            std::thread(&WebSocketServer::handleClient, this, client).detach();
        }
        acceptConnections();
    });
}

void WebSocketServer::handleClient(std::shared_ptr<Client> client) {
    try {
        client->ws.accept();

        beast::flat_buffer buffer;
        while (true) {
            client->ws.read(buffer);
            auto message = beast::buffers_to_string(buffer.cdata());
            buffer.consume(buffer.size());

            auto parsed = json::parse(message);
            auto action = parsed.as_object()["action"].as_string();
            auto symbol = parsed.as_object()["symbol"].as_string();

            if (action == "subscribe") {
                clientManager_.subscribeClient(client, std::string(symbol));
            } else if (action == "unsubscribe") {
                clientManager_.unsubscribeClient(client, std::string(symbol));
            }
        }
    } catch (const std::exception&) {
        clientManager_.removeClient(client);
    }
}
