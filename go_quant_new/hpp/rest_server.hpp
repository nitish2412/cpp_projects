#ifndef REST_SERVER_HPP
#define REST_SERVER_HPP

#include "../hpp/crow_all.h"
#include "../hpp/OrderManager.hpp"

class RestServer {
public:
    RestServer(OrderManager *orderManager);
    void run(int port);

private:
    crow::SimpleApp app;

    void setupRoutes();

    OrderManager *orderManager;
};

#endif // REST_SERVER_HPP
