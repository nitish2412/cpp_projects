#include "../hpp/rest_server.hpp"
#include <iostream>

RestServer::RestServer() {
    setupRoutes();
}

void RestServer::setupRoutes() {
    // ✅ Place Order
    CROW_ROUTE(app, "/place_order").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto json_body = crow::json::load(req.body);
        if (!json_body)
            return crow::response(400, "Invalid JSON");

        std::string symbol = json_body["symbol"].s();
        double price = json_body["price"].d();
        int quantity = json_body["quantity"].i();

        // TODO: Call order execution logic
        CROW_LOG_INFO << "Placing order: " << symbol << " @ " << price << " x " << quantity;
        
        return crow::response(200, "{\"status\": \"Order placed\"}");
    });

    // ✅ Cancel Order
    CROW_ROUTE(app, "/cancel_order").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto json_body = crow::json::load(req.body);
        if (!json_body)
            return crow::response(400, "Invalid JSON");

        int order_id = json_body["order_id"].i();
        
        // TODO: Call order cancellation logic
        CROW_LOG_INFO << "Cancelling order: " << order_id;

        return crow::response(200, "{\"status\": \"Order canceled\"}");
    });

    // ✅ Modify Order
    CROW_ROUTE(app, "/modify_order").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto json_body = crow::json::load(req.body);
        if (!json_body)
            return crow::response(400, "Invalid JSON");

        int order_id = json_body["order_id"].i();
        double new_price = json_body["new_price"].d();

        // TODO: Call order modification logic
        CROW_LOG_INFO << "Modifying order: " << order_id << " -> New Price: " << new_price;

        return crow::response(200, "{\"status\": \"Order modified\"}");
    });

    // ✅ Get Orderbook
    CROW_ROUTE(app, "/orderbook").methods(crow::HTTPMethod::GET)([]() {
        // TODO: Fetch and return the order book
        return crow::response(200, R"({"bids": [], "asks": []})");
    });

    // ✅ View Positions
    CROW_ROUTE(app, "/positions").methods(crow::HTTPMethod::GET)([]() {
        // TODO: Fetch and return open positions
        return crow::response(200, R"({"positions": []})");
    });
}

void RestServer::run(int port) {
    std::cout << "Starting REST server on port " << port << "...\n";
    app.port(port).multithreaded().run();
}
