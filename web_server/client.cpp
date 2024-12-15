#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

int main() {
    // Step 1: Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        std::cerr << "Socket creation failed!" << std::endl;
        return -1;
    }

    // Step 2: Define the server address
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;               // IPv4
    serverAddr.sin_port = htons(8085);             // Server port
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP (localhost)

    // Step 3: Connect to the server
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection to server failed!" << std::endl;
        close(clientSocket);
        return -1;
    }
    std::cout << "Connected to the server!" << std::endl;

    // Step 4: Send a message to the server
    const char* message = "Hello, Server!";
    send(clientSocket, message, strlen(message), 0);
    std::cout << "Message sent: " << message << std::endl;

    // Step 5: Receive the server's response
    char buffer[1024] = {0};
    ssize_t bytesReceived = read(clientSocket, buffer, sizeof(buffer) - 1);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; // Null-terminate the received data
        std::cout << "Server response: " << buffer << std::endl;
    }

    // Step 6: Close the socket
    close(clientSocket);
    return 0;
}
