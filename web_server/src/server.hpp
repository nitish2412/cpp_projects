#ifndef SERVER_HPP
#define SERVER_HPP
#include <netinet/in.h>
#include <string>

class Server{
    
    private:
        int serverSocket;
        sockaddr_in serverAddr;
        int port;
    public:
        Server(int port);
        void start();
        void handleclient(int clientsocket);

};

#endif