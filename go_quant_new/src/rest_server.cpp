#include "../hpp/rest_server.hpp"
#include "../hpp/OrderManager.hpp"
#include <iostream>

RestServer::RestServer(OrderManager *orderManager): orderManager(orderManager){
    setupRoutes();
}

void RestServer::setupRoutes() {
    // ✅ Place Order
    CROW_ROUTE(app, "/place_order").methods(crow::HTTPMethod::POST)([this](const crow::request& req) {
        auto json_body = crow::json::load(req.body);
        if (!json_body)
            return crow::response(400, "Invalid JSON");

        try {
            if (!json_body.has("symbol") || !json_body.has("price") || !json_body.has("quantity") || !json_body.has("order"))
                return crow::response(400, "Missing required fields");

            std::string symbol = json_body["symbol"].s();
            std::string order_type = json_body["order_type"].s();

            if (json_body["price"].t() != crow::json::type::Number)
                return crow::response(400, "Invalid price type");
            double price = json_body["price"].d();

            if (json_body["quantity"].t() != crow::json::type::Number)
                return crow::response(400, "Invalid quantity type");
            double quantity = json_body["quantity"].d();

            CROW_LOG_INFO << "Placing order: " << symbol << " @ " << price << " x " << quantity;
            std::cout << "Testing Place Order..." << std::endl;
            
            std::cout<<symbol<<" "<<quantity<<" "<<price<<"\n";
            if (orderManager->placeOrder(symbol, order_type, quantity, price)) {
            //if (orderManager->placeOrder("BTC-PERPETUAL", "buy", 10.0, 50000.0)) {
                std::cout << "Order placed successfully!" << std::endl;
                return crow::response(200, "{\"status\": \"Order placed\"}");
            } 
            else {
                std::cerr << "Failed to place order." << std::endl;
                return crow::response(500, "Order placement failed");
            }
        } 
        catch (const std::exception& e) {
            CROW_LOG_ERROR << "Exception occurred: " << e.what();
            return crow::response(500, "Internal Server Error");
        }
    });


    // ✅ Cancel Order
    CROW_ROUTE(app, "/cancel_order").methods(crow::HTTPMethod::POST)([this](const crow::request& req) {
        auto json_body = crow::json::load(req.body);
        if (!json_body)
            return crow::response(400, "Invalid JSON");

        std::string order_id = json_body["order_id"].s();

        CROW_LOG_INFO << "Cancelling order: " << order_id;

        if (orderManager->cancelOrder(order_id)) {
            return crow::response(200, "{\"status\": \"Order cancelled\"}");
        } else {
            return crow::response(500, "{\"error\": \"Failed to cancel order\"}");
        }
    });

    // ✅ Modify Order
    CROW_ROUTE(app, "/modify_order").methods(crow::HTTPMethod::POST)([this](const crow::request& req) {
        auto json_body = crow::json::load(req.body);
        if (!json_body)
            return crow::response(400, "Invalid JSON");

        std::string order_id = json_body["order_id"].s();
        double new_price = json_body["new_price"].d();
        double new_quantity = json_body["new_quantity"].d();

        CROW_LOG_INFO << "Modifying order: " << order_id << " -> Price: " << new_price << ", Quantity: " << new_quantity;

        if (orderManager->modifyOrder(order_id, new_price, new_quantity)) {
            return crow::response(200, "{\"status\": \"Order modified\"}");
        } else {
            return crow::response(500, "{\"error\": \"Failed to modify order\"}");
        }
    });

    // ✅ Get positions
    CROW_ROUTE(app, "/get_positions").methods(crow::HTTPMethod::GET)([this](const crow::request& req) {
        std::string responseString;
        
        if (orderManager->viewCurrentPositions(responseString)) {
            return crow::response(200, responseString);
        } else {
            return crow::response(500, "{\"error\": \"Failed to get positions\"}");
        }
    });

    // ✅ Get Orderbook (POST request)
    CROW_ROUTE(app, "/get_orderbook").methods(crow::HTTPMethod::POST)([this](const crow::request& req) {
        auto json_body = crow::json::load(req.body);
        if (!json_body)
            return crow::response(400, "{\"error\": \"Invalid JSON\"}");

        if (!json_body.has("instrument_name"))
            return crow::response(400, "{\"error\": \"Missing instrument_name\"}");

        std::string instrument_name = json_body["instrument_name"].s();
        std::string responseString;

        if (orderManager->viewOrderBook(instrument_name, responseString)) {
            return crow::response(200, responseString);
        } else {
            return crow::response(500, "{\"error\": \"Failed to get orderbook\"}");
        }
    });

}

void RestServer::run(int port) {
    std::cout << "Starting REST server on port " << port << "...\n";
    app.port(port).multithreaded().run();
}
