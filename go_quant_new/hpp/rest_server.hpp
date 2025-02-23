#ifndef REST_SERVER_HPP
#define REST_SERVER_HPP

#include "../hpp/crow_all.h"

class RestServer {
public:
    RestServer();
    void run(int port);

private:
    crow::SimpleApp app;

    void setupRoutes();
};

#endif // REST_SERVER_HPP
