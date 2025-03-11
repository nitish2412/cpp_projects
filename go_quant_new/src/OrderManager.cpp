#include "../hpp/OrderManager.hpp"
#include "../hpp/json.hpp"
#include <iostream>
#include <curl/curl.h>
#include "../hpp/Utils.hpp"
#include "../hpp/AuthManager.hpp"

using json = nlohmann::json;


// Constructor
OrderManager::OrderManager(ApiRequestManager* apiRequestManager) : apiRequestManager(apiRequestManager) {
    std::cout<<"order manager initiated\n";
}

/*OrderManager::OrderManager(AuthManager* authManager) : authManager(authManager) {
    std::cout<<"order manager initiated\n";
}*/

// Place an order
bool OrderManager::placeOrder(const std::string& instrument, const std::string& side, double quantity, double price) {
    std::string url = baseUrl + "/api/v2/private/" + (side == "buy" ? "buy" : "sell");
    //std::cout<<"URL:"<<url<<"\n";
        json requestBody = {
        {"jsonrpc", "2.0"},
        {"id", 1},
        {"method", side == "buy" ? "private/buy" : "private/sell"},
        {"params", {
            {"instrument_name", instrument},
            {"amount", quantity}, // Ensure this is a numeric value
            {"price", price}      // Ensure this is a numeric value
        }}
    };
        
        std::string request_data = requestBody.dump();
        std::string responseString;
        if (!apiRequestManager->sendRequest(url, "POST", request_data, responseString)) {
            return false;
        }

        // Parse response (same as before)
        return processResponse(responseString);

}

// Cancel an order
bool OrderManager::cancelOrder(const std::string& orderId) {
    
    std::string url = baseUrl + "/api/v2/private/cancel";
        json requestBody = {
            {"jsonrpc", "2.0"},
            {"id", 3},
            {"method", "private/cancel"},
            {"params", {
                {"order_id", orderId}
            }}
        };
        std::string request_data = requestBody.dump();
        std::string responseString;
        if (!apiRequestManager->sendRequest(url, "POST", request_data, responseString)) {
            return false;
        }

        // Parse response (same as before)
        return processResponse(responseString);
    
}

// Modify an order
bool OrderManager::modifyOrder(const std::string& orderId, double newQuantity, double newPrice) {
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
        std::string request_data = requestBody.dump();
        std::string responseString;
        if (!apiRequestManager->sendRequest(url, "POST", request_data, responseString)) {
            return false;
        }

        // Parse response (same as before)
        return processResponse(responseString);
   
}


// View order book
bool OrderManager::viewOrderBook(const std::string& instrumentName,std::string& responseString) {
    std::string url = baseUrl + "/api/v2/public/get_order_book?instrument_name=" + instrumentName;
        //std::string responseString;
        std::string dummy_data = instrumentName;
        if (!apiRequestManager->sendRequest(url, "GET", dummy_data, responseString)) {
            return false;
        }

        // Parse response (same as before)
        return processResponse(responseString);
    
}

// View current positions
bool OrderManager::viewCurrentPositions(std::string& responseString) {
    std::string url = baseUrl + "/api/v2/private/get_positions";
    std::string dummy_data = "dummy";

    // Call the API and fetch response
    if (!apiRequestManager->sendRequest(url, "GET", dummy_data, responseString)) {
        return false;
    }

    // Process response (error checking)
    return processResponse(responseString);
}

bool OrderManager::fetchInstruments(std::string& responseString) {
    std::string url = baseUrl + "/api/v2/public/get_instruments?currency=BTC&expired=false";
    std::string dummy_data = "dummy";
    
    if (!apiRequestManager->sendRequest(url, "GET", dummy_data, responseString)) {
        return false;
    }
    
    return processResponse(responseString);
}

// Process JSON response
bool OrderManager::processResponse(std::string& responseString) {
    try {
        json responseJson = json::parse(responseString);

        // Check if response contains an error
        if (responseJson.contains("error")) {
            std::cerr << "Error: " << responseJson.dump(4) << std::endl;
            return false;
        }

        // Convert JSON to string and return valid response
        responseString = responseJson.dump(4); // Pretty-print JSON
        return true;

    } catch (json::parse_error& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return false;
    }
}

