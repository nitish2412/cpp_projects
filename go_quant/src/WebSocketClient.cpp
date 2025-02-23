#include "../hpp/WebSocketClient.hpp"
#include "../hpp/MessageHandler.hpp"
#include <iostream>

using namespace boost::beast;

/*WebSocketClient::WebSocketClient(boost::asio::io_context& io_context,const std::string& uri) 
    :  ioContext(io_context),serverUri(uri), messageHandler(nullptr) {}*/

WebSocketClient::WebSocketClient(boost::asio::io_context& io_context, const std::string& uri)
    : ioContext(io_context), 
      websocket(boost::asio::make_strand(ioContext)),
      serverUri(uri), 
      messageHandler(nullptr) {}

void WebSocketClient::setMessageHandler(MessageHandler& handler) {
    std::lock_guard<std::mutex> lock(handlerMutex); // Protect access to messageHandler
    messageHandler = &handler;
}

WebSocketClient::~WebSocketClient() {
    disconnect();
    if (messageThread.joinable()) {
        messageThread.join();
    }
}

void WebSocketClient::connect() {
    try {
        // Resolve the server URI
        boost::asio::ip::tcp::resolver resolver(ioContext);
        auto const results = resolver.resolve(serverUri, "http"); // or "ws" for WebSocket
        
        // Connect to the resolved endpoint
        boost::asio::connect(websocket.next_layer(), results.begin(), results.end());
        
        // Perform WebSocket handshake
        websocket.handshake(serverUri, "/");
        
        // Start the message handling thread
        messageThread = std::thread(&WebSocketClient::handleMessages, this);
    } catch (const std::exception& e) {
        std::cerr << "Connection error: " << e.what() << std::endl;
    }
}

void WebSocketClient::disconnect() {
    try {
        websocket.close(websocket::close_code::normal);
    } catch (const std::exception& e) {
        std::cerr << "Disconnection error: " << e.what() << std::endl;
    }
}

void WebSocketClient::sendMessage(const std::string& message) {
    try {
        websocket.write(boost::asio::buffer(message));
    } catch (const std::exception& e) {
        std::cerr << "Send error: " << e.what() << std::endl;
    }
}

void WebSocketClient::handleMessages() {
    try {
        while (websocket.is_open()) {
            boost::beast::flat_buffer buffer;
            websocket.read(buffer);
            
            // Thread-safe access to messageHandler
            std::string receivedMessage = boost::beast::buffers_to_string(buffer.data());
            {
                std::lock_guard<std::mutex> lock(handlerMutex);
                if (messageHandler) {
                    messageHandler->processMessage(receivedMessage);
                }
            }
        }
    } catch (const boost::system::system_error& e) {
        if (e.code() != boost::beast::websocket::error::closed) {
            std::cerr << "Message handling error: " << e.what() << std::endl;
        }
    }
}
