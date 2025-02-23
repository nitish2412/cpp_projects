#include "../hpp/websocket_client.hpp"
#include <iostream>
#include <cstring>
#include "../hpp/json.hpp"

#define SERVER_ADDRESS "test.deribit.com"
#define SERVER_PATH "/ws/api/v2"
#define SERVER_PORT 443

WebSocketClient::WebSocketClient(AuthManager& authManager)
    : authManager(authManager), client_wsi(nullptr), context(nullptr) {}

WebSocketClient::~WebSocketClient() {
    if (context) {
        lws_context_destroy(context);
        context = nullptr;
    }
}

void WebSocketClient::setMessageCallback(MessageCallback callback) {
    messageCallback = std::move(callback);
}

void WebSocketClient::sendMessage(const std::string& message) {
    if (!client_wsi) return;

    size_t msg_size = message.size();
    unsigned char* buf = new unsigned char[LWS_PRE + msg_size];

    memcpy(buf + LWS_PRE, message.c_str(), msg_size);

    int n = lws_write(client_wsi, buf + LWS_PRE, msg_size, LWS_WRITE_TEXT);
    if (n < 0) {
        std::cerr << "[ERROR] Failed to send message!" << std::endl;
    } else {
        std::cout << "[DEBUG] Sent: " << message << std::endl;
    }

    delete[] buf;
}

void WebSocketClient::subscribe(const std::string& symbol) {
    std::string channel = "deribit_price_index." + symbol;
    nlohmann::json request = {
        {"jsonrpc", "2.0"},
        {"id", 1},
        {"method", "public/subscribe"},
        {"params", {{"channels", {channel}}}}
    };
    std::string subscribe_msg = R"({
        "jsonrpc": "2.0",
        "method": "public/subscribe",
        "params": {
            "channels": ["deribit_price_index.btc_usd"]
        }
    })";
    //sendMessage(subscribe_msg);
    sendMessage(request.dump());
    std::cout << "[WebSocket] Subscribed to: " << symbol << std::endl;
}


void WebSocketClient::unsubscribe(const std::string& symbol) {
    nlohmann::json request = {
        {"jsonrpc", "2.0"},
        {"id", 2},
        {"method", "public/unsubscribe"},
        {"params", {{"channels", {symbol}}}}
    };
    sendMessage(request.dump());
    std::cout << "[WebSocket] Unsubscribed from: " << symbol << std::endl;
}

int WebSocketClient::callback(lws* wsi, lws_callback_reasons reason, void* user, void* in, size_t len) {
    auto* client = reinterpret_cast<WebSocketClient*>(user);

    switch (reason) {
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            std::cout << "[WebSocket] Connection established!" << std::endl;
            client->authenticate();
            break;

        case LWS_CALLBACK_CLIENT_RECEIVE: {
            std::string response(static_cast<char*>(in), len);
            std::cout << "[WebSocket] Received: " << response << std::endl;
            if (client->messageCallback) {
                 std::cout << "[WebSocket] Message send to client: " << response << std::endl;
                client->messageCallback(response);
            }
            break;
        }

        case LWS_CALLBACK_CLIENT_CLOSED:
            std::cout << "[WebSocket] Connection closed." << std::endl;
            break;

        default:
            break;
    }
    return 0;
}

void WebSocketClient::authenticate() {
    std::string token = authManager.getAccessToken();

    if (token.empty()) {
        std::cerr << "[ERROR] Authentication failed. No token available." << std::endl;
        return;
    }

    std::cout << "[WebSocket] Using auth token: " << token << std::endl;
}

void WebSocketClient::connect() {
    struct lws_context_creation_info info = {};
    memset(&info, 0, sizeof(info));
    info.port = CONTEXT_PORT_NO_LISTEN;
    info.protocols = (const struct lws_protocols[]) {
        { "ws", callback, 0, 65536 },
        { NULL, NULL, 0, 0 }
    };
    info.options = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;

    context = lws_create_context(&info);
    if (!context) {
        std::cerr << "[ERROR] Failed to create WebSocket context!" << std::endl;
        return;
    }

    struct lws_client_connect_info ccinfo = {};
    memset(&ccinfo, 0, sizeof(ccinfo));
    ccinfo.context = context;
    ccinfo.address = SERVER_ADDRESS;
    ccinfo.path = SERVER_PATH;
    ccinfo.host = SERVER_ADDRESS;
    ccinfo.origin = "https://test.deribit.com";
    ccinfo.port = SERVER_PORT;
    ccinfo.ssl_connection = LCCSCF_USE_SSL | LCCSCF_ALLOW_SELFSIGNED | LCCSCF_SKIP_SERVER_CERT_HOSTNAME_CHECK;
    ccinfo.userdata = this;

    client_wsi = lws_client_connect_via_info(&ccinfo);
    if (!client_wsi) {
        std::cerr << "[ERROR] WebSocket connection failed!" << std::endl;
        return;
    }

    while (true) {
        lws_service(context, 100);
    }
}
