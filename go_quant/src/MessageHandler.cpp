#include "../hpp/MessageHandler.hpp"
#include "../hpp/SubscriptionManager.hpp"
#include <iostream>
#include "../hpp/json.hpp"

using json = nlohmann::json;

// Default constructor
//MessageHandler::MessageHandler() {}

MessageHandler::MessageHandler(SubscriptionManager& manager)
    : subscriptionManager(manager) {}

void MessageHandler::processMessage(const std::string& message) {
    try {
        auto jsonMessage = json::parse(message);

        if (jsonMessage.contains("type")) {
            std::string type = jsonMessage["type"];

            if (type == "subscription_event") {
                std::string topic = jsonMessage["topic"];
                std::string data = jsonMessage["data"];
                handleSubscriptionEvent(topic, data);
            } else if (type == "error") {
                std::string errorMessage = jsonMessage["error"];
                handleErrorEvent(errorMessage);
            } else {
                std::cerr << "Unknown message type: " << type << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Message processing error: " << e.what() << std::endl;
    }
}

void MessageHandler::handleSubscriptionEvent(const std::string& topic, const std::string& data) {
    if (subscriptionManager.isSubscribed(topic)) {
        std::cout << "Message for subscribed topic [" << topic << "]: " << data << std::endl;
    } else {
        std::cerr << "Message for unsubscribed topic: " << topic << std::endl;
    }
}

void MessageHandler::handleErrorEvent(const std::string& errorMessage) {
    std::cerr << "Error received: " << errorMessage << std::endl;
}
