#include "../hpp/MarketDataFetcher.hpp"
#include "../hpp/ClientSession.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include "../hpp/json.hpp"

/*MarketDataFetcher::MarketDataFetcher(SubscriptionManager& subManager, AuthManager& authManager) 
    : subscriptionManager(subManager), webSocketClient(authManager), running(false) {}

*/
MarketDataFetcher::MarketDataFetcher(SubscriptionManager& subManager, AuthManager& authManager)
    : subscriptionManager(subManager), webSocketClient(authManager) {
    
    // Set callback to process received messages
    webSocketClient.setMessageCallback([this](const std::string& message) {
        handleMarketData(message);
    });
}

MarketDataFetcher::~MarketDataFetcher() {
    running = false;
    if (marketThread.joinable()) {
        marketThread.join();
    }
}
/*void MarketDataFetcher::start() {
    running = true;
    std::thread([this]() { fetchData(); }).detach();
}*/

void MarketDataFetcher::start() {
    running = true;
    marketThread = std::thread([this]() {
        webSocketClient.connect();  // Connect WebSocket to fetch real data
    });
}

/*void MarketDataFetcher::handleMarketData(const std::string& data) {
    std::cout << "Received Market Data: " << data << std::endl;

    // Notify subscribed clients
    nlohmann::json jsonData = nlohmann::json::parse(data);
    if (jsonData.contains("symbol")) {
        std::string symbol = jsonData["symbol"];
        for (auto* client : subscriptionManager.getSubscribers(symbol)) {
            client->sendMessage(data);
        }
    }
}*/

void MarketDataFetcher::handleMarketData(const std::string& data) {
    std::cout << "[DEBUG] Received Market Data: " << data << std::endl;

    try {
        nlohmann::json jsonData = nlohmann::json::parse(data);
        std::cout << "[DEBUG] JSON parsed successfully" << std::endl;

        if (jsonData.contains("params") && jsonData["params"].contains("channel")) {
            std::string channel = jsonData["params"]["channel"];
            std::cout << "[DEBUG] Extracted channel: " << channel << std::endl;

            // Extract symbol from channel (e.g., "deribit_price_index.btc_usd" → "btc_usd")
            std::string symbol = channel.substr(channel.find_last_of('.') + 1);
            std::cout << "[DEBUG] Extracted symbol: " << symbol << std::endl;

            auto subscribers = subscriptionManager.getSubscribers(symbol);
            std::cout << "[DEBUG] Found " << subscribers.size() << " subscribers for symbol: " << symbol << std::endl;

            for (auto* client : subscribers) {
                std::cout << "[DEBUG] Sending message to client: " << data << std::endl;
                client->sendMessage(data);
            }
        } else {
            std::cerr << "[ERROR] JSON does not contain expected 'params.channel' structure. Data: " << jsonData.dump(4) << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] JSON parsing failed: " << e.what() << std::endl;
    }
}


void MarketDataFetcher::subscribe(const std::string& symbol) {
    if (activeSubscriptions.find(symbol) == activeSubscriptions.end()) {
        activeSubscriptions.insert(symbol);
        webSocketClient.subscribe(symbol);  // Send subscription request to WebSocket API
    }
}

void MarketDataFetcher::unsubscribe(const std::string& symbol) {
    if (activeSubscriptions.find(symbol) != activeSubscriptions.end()) {
        activeSubscriptions.erase(symbol);
        webSocketClient.unsubscribe(symbol);  // Send unsubscription request to WebSocket API
    }
}
