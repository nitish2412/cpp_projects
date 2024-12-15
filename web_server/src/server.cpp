#include "server.hpp"
#include <unistd.h>   
#include <cstring>
#include <iostream>
#include "thread_pool.hpp"

Server::Server(int p): port(p){
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 10) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "Server started on port " << port << std::endl;

}

/*void Server::start() {
    ThreadPool pool(4); // Creates a thread pool with 4 threads
    while (true) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0) {
            perror("Accept failed");
            continue;
        }
        pool.enqueue([this, clientSocket]() { handleclient(clientSocket); });
    }
}*/

void Server::start(){
    while(true){
        int clientsocket=accept(serverSocket,nullptr,nullptr);
        if(clientsocket<0){
            perror("Accept failed");
            continue;
        }
        handleclient(clientsocket);
    }   
}

void Server::handleclient(int clientSocket){
    // Basic response example
    std::string httpResponse = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 13\r\n\r\n"
        "welcome to my first server Hello, World!";

    char buffer[1024];
    ssize_t bytesReceived = read(clientSocket, buffer, sizeof(buffer) - 1);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; // Null-terminate the buffer
        std::cout << "Received: " << buffer << std::endl;
        //std::string response = "Hello from server!";
        //send(clientSocket, response.c_str(), response.size(), 0);
        send(clientSocket, httpResponse.c_str(), httpResponse.size(), 0);
    }
    close(clientSocket);  // Close the connection

}