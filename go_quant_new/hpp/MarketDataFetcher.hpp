#ifndef MARKET_DATA_FETCHER_HPP
#define MARKET_DATA_FETCHER_HPP

#include "../hpp/SubscriptionManager.hpp"
#include "../hpp/websocket_client.hpp"
#include <thread>
#include <atomic>
#include <unordered_set>
#include <string>

class MarketDataFetcher {
private:
    SubscriptionManager& subscriptionManager;
    WebSocketClient webSocketClient;  // Now this requires AuthManager
    std::unordered_set<std::string> activeSubscriptions;
    std::atomic<bool> running {false};  // Manage thread safety
    std::thread marketThread;  // Add this

public:
    MarketDataFetcher(SubscriptionManager& subManager, AuthManager& authManager);
    ~MarketDataFetcher();
    void start();
    //void fetchData();
    void handleMarketData(const std::string& data);

    // New methods for handling subscriptions
    void subscribe(const std::string& symbol);
    void unsubscribe(const std::string& symbol);
};

#endif // MARKET_DATA_FETCHER_HPP
