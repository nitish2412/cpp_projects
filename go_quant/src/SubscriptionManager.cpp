#include "../hpp/SubscriptionManager.hpp"
#include "../hpp/WebSocketClient.hpp"
#include <iostream>

SubscriptionManager::SubscriptionManager(WebSocketClient& client) : client(client) {}

void SubscriptionManager::subscribe(const std::string& topic) {
    std::lock_guard<std::mutex> lock(mutex);
    if (subscriptions.find(topic) == subscriptions.end()) {
        subscriptions.insert(topic);
        client.sendMessage("{\"action\":\"subscribe\", \"topic\":\"" + topic + "\"}");
        std::cout << "Subscribed to topic: " << topic << std::endl;
    }
}

void SubscriptionManager::unsubscribe(const std::string& topic) {
    std::lock_guard<std::mutex> lock(mutex);
    if (subscriptions.find(topic) != subscriptions.end()) {
        subscriptions.erase(topic);
        client.sendMessage("{\"action\":\"unsubscribe\", \"topic\":\"" + topic + "\"}");
        std::cout << "Unsubscribed from topic: " << topic << std::endl;
    }
}

bool SubscriptionManager::isSubscribed(const std::string& topic) {
    std::lock_guard<std::mutex> lock(mutex);
    return subscriptions.find(topic) != subscriptions.end();
}
