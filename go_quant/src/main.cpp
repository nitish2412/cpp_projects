#include<iostream>
#include "../hpp/AuthManager.hpp"
#include "../hpp/OrderManager.hpp"
#include "../hpp/ApiRequestManager.hpp"

#include "../hpp/WebSocketClient.hpp"
#include "../hpp/SubscriptionManager.hpp"
#include "../hpp/MessageHandler.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>

using namespace std;

int main(){
    cout<<"hello world\n";
    // Step 1: Create WebSocketClient instance
    boost::asio::io_context io_context;
    //WebSocketClient client(io_context,"wss://www.deribit.com/ws/api/v2");
    WebSocketClient client(io_context,"http://www.deribit.com/ws/api/v2");

    // Step 2: Create SubscriptionManager instance
    SubscriptionManager manager(client);

    // Step 3: Create MessageHandler instance
    MessageHandler messageHandler(manager);

    // Step 4: Set MessageHandler in WebSocketClient
    client.setMessageHandler(messageHandler);

    // Connect to the WebSocket server
    client.connect();
    // Replace with your actual client ID and secret
    std::string client_id = "NwlKlPaT";
    std::string client_secret = "WbfqBP49qWBL1MuacHhK9rXQ5BmBjvKSSOx20_e0QmA";


    // Create AuthManager object
    /*AuthManager auth_manager(client_id, client_secret);

    // Authenticate and retrieve access token
    if (auth_manager.authenticate()) {
        std::cout << "Authentication successful!" << std::endl;
        std::cout << "Access Token: " << auth_manager.getAccessToken() << std::endl;
    } else {
        std::cout << "Authentication failed!" << std::endl;
    }
    ApiRequestManager api_manager(&auth_manager);
    OrderManager orderManager(&api_manager);*/
    //OrderManager orderManager(&auth_manager);
    // Test Place Order
    /*std::cout << "Testing Place Order..." << std::endl;
    if (orderManager.placeOrder("BTC-PERPETUAL", "buy", 10.0, 50000.0)) {
        std::cout << "Order placed successfully!" << std::endl;
    } 
    else {
        std::cerr << "Failed to place order." << std::endl;
    }*/
    /*std::cout << "Testing modify Order..." << std::endl;
    bool result = orderManager.modifyOrder("30482141941", 10, 48000);
    if (result) {
        std::cout << "Order modified successfully!" << std::endl;
    } else {
        std::cerr << "Failed to modify order." << std::endl;
    }*/
    /*std::cout << "Testing Cancel Order..." << std::endl;
    std::string orderId = "30482295539";  // Replace with actual order ID
    if (orderManager.cancelOrder(orderId)) {
        std::cout << "Order cancelled successfully." << std::endl;
    } else {
        std::cout << "Failed to cancel order." << std::endl;
    }*/
    /*std::cout << "View order book..." << std::endl;
    std::string instrumentName = "BTC-PERPETUAL"; // Replace with your desired instrument
    if (orderManager.viewOrderBook(instrumentName)) {
        std::cout << "Successfully retrieved the order book!" << std::endl;
    } else {
        std::cerr << "Failed to retrieve the order book." << std::endl;
    }*/

   /*std::cout << "View current positionr..." << std::endl; // Replace with your desired instrument
    if (orderManager.viewCurrentPositions()) {
        std::cout << "Successfully View current positionk!" << std::endl;
    } else {
        std::cerr << "Failed to View current position." << std::endl;
    }*/

    return 0;
}