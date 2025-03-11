#include "../hpp/WebSocketServer.hpp"
#include <iostream>

WebSocketServer::WebSocketServer(int port, AuthManager& authManager)
    : subscriptionManager(marketDataFetcher),marketDataFetcher(subscriptionManager, authManager) {  // Then pass it to SubscriptionManager

    app.ws<PerClientData>("/*", {
        .open = [this](auto *ws) {
            addClient(ws);
            std::cout << "Client connected\n";
        },
        .message = [this](auto *ws, std::string_view message, uWS::OpCode opCode) {
            try {
                handleMessage(ws, message);
            } catch (const std::exception &e) {
                std::cerr << "Error handling message: " << e.what() << std::endl;
            }
        },
        .close = [this](auto *ws, int /*code*/, std::string_view /*message*/) {
            removeClient(ws);
            std::cout << "Client disconnected\n";
        }
    }).listen(port, [port](auto *listenSocket) {
        if (listenSocket) {
            std::cout << "Server listening on port " << port << "\n";
        }
    });

    marketDataFetcher.start();  // Start fetching market data in a separate thread
}

void WebSocketServer::run() {
    std::cout << "[WebSocket] Entering run()..." << std::endl;

    app.run();

    std::cout << "[WebSocket] Exiting run() unexpectedly!" << std::endl;  // Should never print
}

void WebSocketServer::addClient(uWS::WebSocket<false, true, PerClientData> *ws) {
    clients[ws] = std::make_unique<ClientSession>(ws, subscriptionManager);
}

void WebSocketServer::removeClient(uWS::WebSocket<false, true, PerClientData> *ws) {
    auto it = clients.find(ws);
    if (it != clients.end()) {
        ClientSession *client = it->second.get();

        // Unsubscribe from all symbols before removing the client
        for (const auto &symbol : subscriptionManager.getSubscriptionsForClient(client)) {
            subscriptionManager.unsubscribe(client, symbol);
        }

        clients.erase(it);
    }
}

void WebSocketServer::handleMessage(uWS::WebSocket<false, true, PerClientData> *ws, std::string_view message) {
    auto it = clients.find(ws);
    if (it != clients.end()) {
        it->second->onMessage(message);
    }
}
