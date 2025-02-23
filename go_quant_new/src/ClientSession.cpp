#include "../hpp/ClientSession.hpp"
#include "../hpp/json.hpp"
#include <iostream>

//ClientSession::ClientSession(WebSocket *ws) : ws(ws) {}
ClientSession::ClientSession(WebSocket *ws, SubscriptionManager &subManager) 
    : ws(ws), subscriptionManager(subManager) {}


void ClientSession::onMessage(std::string_view message) {
    std::cout << "Received raw message: " << message << std::endl;

    try {
        // Parse JSON message
        nlohmann::json jsonMessage = nlohmann::json::parse(message);
        std::cout << "Parsed JSON: " << jsonMessage.dump() << std::endl;

        if (!jsonMessage.contains("type")) {
            sendMessage("Error: Missing 'type' field");
            return;
        }

        std::string type = jsonMessage["type"];

        if (type == "subscribe") {
            if (!jsonMessage.contains("symbol")) {
                sendMessage("Error: Missing 'symbol' field");
                return;
            }

            std::string symbol = jsonMessage["symbol"];
            subscribe(symbol);
            
            sendMessage("Subscribed to " + symbol);
        } 
        else if (type == "unsubscribe") {
            if (!jsonMessage.contains("symbol")) {
                sendMessage("Error: Missing 'symbol' field");
                return;
            }

            std::string symbol = jsonMessage["symbol"];
            unsubscribe(symbol);
            sendMessage("Unsubscribed from " + symbol);
        } 
        else {
            sendMessage("Unknown command: " + type);
        }
    } catch (const std::exception& e) {
        sendMessage(std::string("JSON Parsing Error: ") + e.what());
    }
}

void ClientSession::subscribe(const std::string &symbol) {
    subscriptionManager.subscribe(this, symbol); // Use SubscriptionManager
    //std::cout << "Subscribed: " << symbol << " | Total subscriptions: " << subscriptionManager.size() << std::endl;
}

void ClientSession::unsubscribe(const std::string &symbol) {
    subscriptionManager.unsubscribe(this,symbol);
    //subscriptions.erase(symbol);
}



void ClientSession::sendMessage(const std::string &message) {
    if (ws) {
        ws->send(message, uWS::OpCode::TEXT);
    }
}

std::unordered_set<std::string> ClientSession::getSubscriptions() const {
    return subscriptionManager.getSubscriptionsForClient(this);
}


