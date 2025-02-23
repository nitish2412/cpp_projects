#include <iostream>
#include <uwebsockets/App.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
constexpr const char* DERIBIT_WS_URL = "wss://www.deribit.com/ws/api/v2";

class DeribitClient {
public:
    DeribitClient() {
        connect();
    }

private:
    void connect() {
        uWS::App().ws<PerClientData>(DERIBIT_WS_URL, {
            .open = [this](auto *ws) {
                std::cout << "Connected to Deribit WebSocket\n";
                subscribeToMarketData(ws);
            },
            .message = [this](auto *ws, std::string_view message, uWS::OpCode) {
                handleMessage(ws, message);
            },
            .close = [this](auto *ws, int, std::string_view) {
                std::cout << "Disconnected from Deribit, reconnecting...\n";
                connect(); // Auto-reconnect
            }
        }).connect(DERIBIT_WS_URL, nullptr).run();
    }

    void subscribeToMarketData(auto *ws) {
        json request = {
            {"jsonrpc", "2.0"},
            {"method", "public/subscribe"},
            {"params", {{"channels", {"ticker.BTC-PERPETUAL.raw"}}}},
            {"id", 42}
        };
        ws->send(request.dump(), uWS::OpCode::TEXT);
    }

    void handleMessage(auto *ws, std::string_view message) {
        try {
            json response = json::parse(message);
            if (response.contains("params") && response["params"].contains("data")) {
                std::string symbol = response["params"]["data"]["instrument_name"];
                double price = response["params"]["data"]["last_price"];

                std::cout << "Received Market Data: " << symbol << " -> $" << price << "\n";
            }
        } catch (const std::exception &e) {
            std::cerr << "JSON Parsing Error: " << e.what() << "\n";
        }
    }
};

int main() {
    DeribitClient client;
    return 0;
}
