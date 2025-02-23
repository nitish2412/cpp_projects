#include "../hpp/MessageHandler.hpp"
#include "../hpp/json.hpp"

MessageHandler::MessageHandler(SubscriptionManager& subManager) : subscriptionManager(subManager) {}

void MessageHandler::handleMessage(ClientSession* client, const std::string& message) {
    auto json = nlohmann::json::parse(message, nullptr, false);
    if (json.is_discarded() || !json.contains("type")) {
        client->sendMessage(R"({"error": "Invalid message format"})");
        return;
    }

    std::string type = json["type"];

    if (type == "subscribe" && json.contains("symbol")) {
        std::string symbol = json["symbol"];
        client->subscribe(symbol);
        subscriptionManager.subscribe(client, symbol);
        client->sendMessage(R"({"status": "subscribed", "symbol": ")" + symbol + R"("})");
    } 
    else if (type == "unsubscribe" && json.contains("symbol")) {
        std::string symbol = json["symbol"];
        client->unsubscribe(symbol);
        subscriptionManager.unsubscribe(client, symbol);
        client->sendMessage(R"({"status": "unsubscribed", "symbol": ")" + symbol + R"("})");
    } 
    else if (type == "publish" && json.contains("symbol") && json.contains("data")) {
        std::string symbol = json["symbol"];
        std::string data = json["data"];

        auto subscribers = subscriptionManager.getSubscribers(symbol);
        for (auto* subscriber : subscribers) {
            subscriber->sendMessage(R"({"symbol": ")" + symbol + R"(", "data": ")" + data + R"("})");
        }
    } 
    else {
        client->sendMessage(R"({"error": "Unknown message type"})");
    }
}
