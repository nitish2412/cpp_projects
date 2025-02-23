#ifndef WEBSOCKET_CLIENT_HPP
#define WEBSOCKET_CLIENT_HPP

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <string>
#include <thread>

class MessageHandler;

class WebSocketClient {
public:
    //WebSocketClient(const std::string& serverUri, MessageHandler& handler);
    WebSocketClient(boost::asio::io_context& io_context,const std::string& uri);
    ~WebSocketClient();

    void connect();
    void disconnect();
    void sendMessage(const std::string& message);
    void setMessageHandler(MessageHandler& handler);

private:
    void handleMessages(); // Thread function to handle incoming messages

    std::string serverUri;
    boost::asio::io_context &ioContext;
    boost::beast::websocket::stream<boost::asio::ip::tcp::socket> websocket;
    std::thread messageThread;

    std::mutex handlerMutex; 

    MessageHandler* messageHandler; // Delegates message processing to MessageHandler
    
};

#endif // WEBSOCKET_CLIENT_HPP
