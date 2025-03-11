#include <iostream>
#include <thread>
#include <chrono>
#include "spdlog/spdlog.h"
#include "../hpp/AuthManager.hpp"
#include "../hpp/OrderManager.hpp"
#include "../hpp/ApiRequestManager.hpp"
#include "../hpp/WebSocketServer.hpp"
#include "../hpp/rest_server.hpp"
#include "../hpp/Logger.hpp"

using namespace std;

int main() {
    cout << "Hello world\n";
    auto logger = Logger::getInstance();

    SPDLOG_LOGGER_INFO(logger, "Application started");
    SPDLOG_LOGGER_WARN(logger, "Application started");
    SPDLOG_LOGGER_ERROR(logger, "Application started");

    std::string client_id = "NwlKlPaT";
    std::string client_secret = "WbfqBP49qWBL1MuacHhK9rXQ5BmBjvKSSOx20_e0QmA";

    // Create AuthManager object
    AuthManager auth_manager(client_id, client_secret);

    if (!auth_manager.authenticate()) {
        std::cout << "Authentication failed!" << std::endl;
        SPDLOG_LOGGER_INFO(logger, "Authentication failed!");
        return 1;
    }
    std::cout << "Authentication successful!\n";
    SPDLOG_LOGGER_INFO(logger, "Authentication successful!");
    
    // Create API and Order managers
    ApiRequestManager api_manager(&auth_manager);
    OrderManager orderManager(&api_manager);

    // Create REST and WebSocket server instances
    RestServer rest_server(&orderManager);
    WebSocketServer ws_server(9001, auth_manager); 

    // Start REST server in a background thread
    std::thread rest_thread([&]() {
        std::cout << "[REST] Running on port 8080..." << std::endl;
        SPDLOG_LOGGER_INFO(logger, "[REST] Running on port 8080...");
        rest_server.run(8080);
    });

    // Run WebSocket server in the main thread
    std::cout << "[WebSocket] Running on port 9001..." << std::endl;
    SPDLOG_LOGGER_INFO(logger, "[WebSocket] Running on port 9001...");
    ws_server.run();

    // Join threads (this will never be reached in this example)
   
    rest_thread.join();
    spdlog::shutdown();

    return 0;
}