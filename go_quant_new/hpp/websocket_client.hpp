#ifndef WEBSOCKET_CLIENT_HPP
#define WEBSOCKET_CLIENT_HPP

#include <libwebsockets.h>
#include "../hpp/AuthManager.hpp"
#include <string>
#include <functional>

class WebSocketClient {
public:
    using MessageCallback = std::function<void(const std::string&)>;

    WebSocketClient(AuthManager& authManager);
    ~WebSocketClient();

    void connect();
    void sendMessage(const std::string& message);
    void setMessageCallback(MessageCallback callback);
    void authenticate();

    // ✅ Added subscribe and unsubscribe functions
    void subscribe(const std::string& symbol);
    void unsubscribe(const std::string& symbol);

private:
    static int callback(lws* wsi, lws_callback_reasons reason, void* user, void* in, size_t len);
    
    struct lws* client_wsi;
    struct lws_context* context;
    MessageCallback messageCallback;  // Callback for notifying MarketDataFetcher
    AuthManager& authManager;  // Reference to AuthManager
};
#endif // WEBSOCKET_CLIENT_HPP
