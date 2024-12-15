class OrderManager {
public:
    // Constructor now only requires ApiRequestManager, as AuthManager is already handled inside ApiRequestManager
    OrderManager(ApiRequestManager* apiRequestManager)
        : apiRequestManager(apiRequestManager) {}

    bool placeOrder(const std::string& instrument, const std::string& side, double quantity, double price) {
        std::string url = baseUrl + "/api/v2/private/" + (side == "buy" ? "buy" : "sell");
        json requestBody = {
            {"jsonrpc", "2.0"},
            {"id", 1},
            {"method", side == "buy" ? "private/buy" : "private/sell"},
            {"params", {
                {"instrument_name", instrument},
                {"amount", quantity},
                {"price", price}
            }}
        };

        std::string responseString;
        if (!apiRequestManager->sendRequest(url, "POST", requestBody, responseString)) {
            return false;
        }

        // Parse response (same as before)
        return processResponse(responseString);
    }

    bool cancelOrder(const std::string& orderId) {
        std::string url = baseUrl + "/api/v2/private/cancel";
        json requestBody = {
            {"jsonrpc", "2.0"},
            {"id", 3},
            {"method", "private/cancel"},
            {"params", {
                {"order_id", orderId}
            }}
        };

        std::string responseString;
        if (!apiRequestManager->sendRequest(url, "POST", requestBody, responseString)) {
            return false;
        }

        // Parse response (same as before)
        return processResponse(responseString);
    }

    bool modifyOrder(const std::string& orderId, double newQuantity, double newPrice) {
        std::string url = baseUrl + "/api/v2/private/edit";
        json requestBody = {
            {"jsonrpc", "2.0"},
            {"id", 3},
            {"method", "private/edit"},
            {"params", {
                {"order_id", orderId},
                {"amount", newQuantity},
                {"price", newPrice}
            }}
        };

        std::string responseString;
        if (!apiRequestManager->sendRequest(url, "POST", requestBody, responseString)) {
            return false;
        }

        // Parse response (same as before)
        return processResponse(responseString);
    }

    bool viewOrderBook(const std::string& instrumentName) {
        std::string url = baseUrl + "/api/v2/public/get_order_book?instrument_name=" + instrumentName;
        std::string responseString;

        if (!apiRequestManager->sendRequest(url, "GET", json(), responseString)) {
            return false;
        }

        // Parse response (same as before)
        return processResponse(responseString);
    }

private:
    bool processResponse(const std::string& responseString) {
        try {
            json responseJson = json::parse(responseString);
            if (responseJson.contains("error")) {
                std::cerr << "Error: " << responseJson.dump(4) << std::endl;
                return false;
            }
            std::cout << "Response: " << responseJson.dump(4) << std::endl;
            return true;
        } catch (json::parse_error& e) {
            std::cerr << "JSON parsing error: " << e.what() << std::endl;
            return false;
        }
    }

    ApiRequestManager* apiRequestManager;  // Dependency injected
};
