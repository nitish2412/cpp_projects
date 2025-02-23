#pragma once

#include <unordered_map>
#include <unordered_set>
#include <string>

class ClientSession;  // Forward declaration
class MarketDataFetcher;  // Forward declaration

class SubscriptionManager {
public:
    SubscriptionManager(MarketDataFetcher &fetcher);  // Constructor with MarketDataFetcher reference

    void subscribe(ClientSession* client, const std::string& symbol);
    void unsubscribe(ClientSession* client, const std::string& symbol);
    
    std::unordered_set<ClientSession*> getSubscribers(const std::string& symbol) const;
    std::unordered_set<std::string> getSubscriptionsForClient(const ClientSession* client) const;

private:
    std::unordered_map<std::string, std::unordered_set<ClientSession*>> symbolToClients;
    MarketDataFetcher &marketDataFetcher;  // Reference to MarketDataFetcher
};