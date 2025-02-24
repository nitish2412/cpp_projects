#ifndef ORDER_MANAGER_HPP
#define ORDER_MANAGER_HPP

#include <string>
#include <unordered_map>
#include "../hpp/ApiRequestManager.hpp"

class OrderManager {
private:
    ApiRequestManager* apiRequestManager;
    //AuthManager* authManager;
    std::string baseUrl = "https://test.deribit.com";
    bool processResponse(std::string& responseString);

public:
    // Constructor
    OrderManager(ApiRequestManager* apiRequestManager);
    //OrderManager(AuthManager* authManager);

    // Function to place an order (buy/sell)
    bool placeOrder(const std::string& instrument, const std::string& side, double quantity, double price);

    // Function to cancel an order
    bool cancelOrder(const std::string& orderId);

    // Function to modify an order
    bool modifyOrder(const std::string& orderId, double newQuantity, double newPrice);

    // Function to view the order book
    bool viewOrderBook(const std::string& instrument,std::string& responseString);

    // view current position
    bool viewCurrentPositions(std::string& responseString);

    // Destructor
    ~OrderManager() = default;
};

#endif
