#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/json.hpp>
#include <unordered_map>
#include <set>
#include <thread>
#include <iostream>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace json = boost::json;

using tcp = asio::ip::tcp;

// Client structure to manage subscriptions
struct Client {
    websocket::stream<tcp::socket> ws;
    std::set<std::string> subscribedSymbols;

    Client(tcp::socket&& socket) : ws(std::move(socket)) {}
};

// Global container for managing connected clients
std::vector<std::shared_ptr<Client>> clients;

// Function to broadcast orderbook updates
void broadcastOrderbookUpdate(const std::string& symbol, const json::value& orderbookData) {
    for (const auto& client : clients) {
        if (client->subscribedSymbols.count(symbol)) {
            try {
                // Convert JSON data to string and send to client
                client->ws.text(true);
                client->ws.write(asio::buffer(json::serialize(orderbookData)));
            } catch (const std::exception& e) {
                std::cerr << "Error broadcasting to client: " << e.what() << std::endl;
            }
        }
    }
}

// Handle individual WebSocket client connection
void handleClient(std::shared_ptr<Client> client) {
    try {
        beast::flat_buffer buffer;
        while (true) {
            client->ws.read(buffer);
            std::string message = beast::buffers_to_string(buffer.cdata());
            buffer.consume(buffer.size()); // Clear buffer for next message

            // Parse the received message as JSON
            auto parsedMessage = json::parse(message);
            auto action = parsedMessage.as_object()["action"].as_string();

            if (action == "subscribe") {
                auto symbol = parsedMessage.as_object()["symbol"].as_string();
                client->subscribedSymbols.insert(std::string(symbol));
                std::cout << "Client subscribed to: " << symbol << std::endl;
            } else if (action == "unsubscribe") {
                auto symbol = parsedMessage.as_object()["symbol"].as_string();
                client->subscribedSymbols.erase(std::string(symbol));
                std::cout << "Client unsubscribed from: " << symbol << std::endl;
            }
        }
    } catch (const beast::system_error& se) {
        if (se.code() != websocket::error::closed) {
            std::cerr << "WebSocket error: " << se.what() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error in client handler: " << e.what() << std::endl;
    }
}

// WebSocket server to handle incoming connections
void websocketServer(asio::io_context& ioc, uint16_t port) {
    tcp::acceptor acceptor(ioc, tcp::endpoint(tcp::v4(), port));

    std::cout << "WebSocket server running on port " << port << "..." << std::endl;

    while (true) {
        // Accept a new connection
        auto socket = acceptor.accept();
        auto client = std::make_shared<Client>(std::move(socket));

        // Perform WebSocket handshake
        client->ws.accept();

        // Add client to the global list
        clients.push_back(client);

        // Start a new thread to handle this client
        std::thread(handleClient, client).detach();
    }
}

// Simulated orderbook updates
void simulateOrderbookUpdates() {
    while (true) {
        // Example orderbook data
        json::value orderbookData = {
            {"symbol", "BTC-USD"},
            {"bids", {{"price", 30000}, {"quantity", 1}}},
            {"asks", {{"price", 30010}, {"quantity", 2}}}
        };

        // Broadcast orderbook updates to subscribed clients
        broadcastOrderbookUpdate("BTC-USD", orderbookData);

        // Sleep for a short duration to simulate periodic updates
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    try {
        asio::io_context ioc;

        // Start the WebSocket server in a separate thread
        std::thread serverThread([&ioc]() {
            websocketServer(ioc, 8080);
        });

        // Simulate orderbook updates in the main thread
        simulateOrderbookUpdates();

        serverThread.join();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
