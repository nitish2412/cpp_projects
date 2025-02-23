#include "../hpp/SubscriptionManager.hpp"
#include "../hpp/MarketDataFetcher.hpp"
#include "../hpp/ClientSession.hpp"

SubscriptionManager::SubscriptionManager(MarketDataFetcher &fetcher) 
    : marketDataFetcher(fetcher) {}

void SubscriptionManager::subscribe(ClientSession* client, const std::string& symbol) {
    bool firstSubscriber = symbolToClients[symbol].empty();  // Check if first subscription

    symbolToClients[symbol].insert(client);

    if (firstSubscriber) {
        marketDataFetcher.subscribe(symbol);  // Fetch market data when first client subscribes
    }
}

void SubscriptionManager::unsubscribe(ClientSession* client, const std::string& symbol) {
    if (symbolToClients.find(symbol) != symbolToClients.end()) {
        symbolToClients[symbol].erase(client);

        if (symbolToClients[symbol].empty()) {  // If no more subscribers, stop fetching
            marketDataFetcher.unsubscribe(symbol);
            symbolToClients.erase(symbol);
        }
    }
}

std::unordered_set<ClientSession*> SubscriptionManager::getSubscribers(const std::string& symbol) const {
    auto it = symbolToClients.find(symbol);
    if (it != symbolToClients.end()) {
        return it->second;
    }
    return {};
}

std::unordered_set<std::string> SubscriptionManager::getSubscriptionsForClient(const ClientSession* client) const {
    std::unordered_set<std::string> clientSubscriptions;
    for (const auto& [symbol, clients] : symbolToClients) {
        auto it = clients.find(const_cast<ClientSession*>(client)); 
        if (it != clients.end()) {
            clientSubscriptions.insert(symbol);
        }
    }
    return clientSubscriptions;
}

