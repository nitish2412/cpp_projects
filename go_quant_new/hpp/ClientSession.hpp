#pragma once
#include <unordered_set>
#include <string>
#include <App.h>
#include "../hpp/SubscriptionManager.hpp"

struct PerClientData {};

class ClientSession {
public:
    //using WebSocket = uWS::WebSocket<false, true>;
    using WebSocket = uWS::WebSocket<false, true, PerClientData>;
    void onMessage(std::string_view message);
    //explicit ClientSession(WebSocket *ws);
    explicit ClientSession(WebSocket *ws, SubscriptionManager &subManager);
    void subscribe(const std::string &symbol);
    void unsubscribe(const std::string &symbol);
    void sendMessage(const std::string &message);
    std::unordered_set<std::string> getSubscriptions() const;

private:
    WebSocket *ws;
    //std::unordered_set<std::string> subscriptions;
    SubscriptionManager &subscriptionManager;
};
