#ifndef WEBSOCKET_SERVER_HPP
#define WEBSOCKET_SERVER_HPP

#include <App.h>
#include <unordered_map>
#include <memory>
#include <string_view>
#include "../hpp/ClientSession.hpp"
#include "../hpp/SubscriptionManager.hpp"
#include "../hpp/MarketDataFetcher.hpp"

class WebSocketServer {
public:
    WebSocketServer(int port, AuthManager& authManager);
    void run();

private:
    uWS::App app;
    std::unordered_map<uWS::WebSocket<false, true, PerClientData> *, std::unique_ptr<ClientSession>> clients;
    
    MarketDataFetcher marketDataFetcher;  // ✅ Constructed first
    SubscriptionManager subscriptionManager;  // ✅ Constructed after MarketDataFetcher

    void handleMessage(uWS::WebSocket<false, true, PerClientData> *ws, std::string_view message);
    void removeClient(uWS::WebSocket<false, true, PerClientData> *ws);
    void addClient(uWS::WebSocket<false, true, PerClientData> *ws);
};

#endif // WEBSOCKET_SERVER_HPP
