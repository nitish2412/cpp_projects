#ifndef SUBSCRIPTION_MANAGER_HPP
#define SUBSCRIPTION_MANAGER_HPP

#include <string>
#include <unordered_set>
#include <mutex>

class WebSocketClient;

class SubscriptionManager {
public:
    SubscriptionManager(WebSocketClient& client);

    void subscribe(const std::string& topic);
    void unsubscribe(const std::string& topic);
    bool isSubscribed(const std::string& topic);

private:
    WebSocketClient& client;
    std::unordered_set<std::string> subscriptions; // Set of active subscriptions
    std::mutex mutex; // To ensure thread-safe access to the subscriptions set
};

#endif // SUBSCRIPTION_MANAGER_HPP
