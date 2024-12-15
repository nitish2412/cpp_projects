#include "../hpp/MarketDataStreamer.hpp"
#include <boost/json.hpp>
#include <chrono>
#include <iostream>
#include <thread>

namespace json = boost::json;

MarketDataStreamer::MarketDataStreamer(ClientManager& manager) : clientManager_(manager) {}

void MarketDataStreamer::start() {
    std::thread([this]() {
        while (true) {
            // Simulate market data updates
            json::value orderbookUpdate = {
                {"symbol", "BTC-USD"},
                {"bids", {{"price", 30000}, {"quantity", 1}}},
                {"asks", {{"price", 30010}, {"quantity", 2}}}
            };

            // Broadcast updates to subscribed clients
            auto clients = clientManager_.getClientsForSymbol("BTC-USD");
            for (const auto& client : clients) {
                try {
                    client->ws.text(true);
                    client->ws.write(asio::buffer(json::serialize(orderbookUpdate)));
                } catch (const std::exception& e) {
                    std::cerr << "Error sending update: " << e.what() << std::endl;
                }
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }).detach();
}
