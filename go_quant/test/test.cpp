#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/ip/tcp.hpp>
//#include <json/json.h>  // Use your preferred JSON library
#include <iostream>
#include <string>
#include "../hpp/json.hpp"
using json = nlohmann::json;

namespace beast = boost::beast;         // From <boost/beast.hpp>
namespace websocket = beast::websocket; // From <boost/beast/websocket.hpp>
namespace net = boost::asio;           // From <boost/asio.hpp>
namespace ssl = net::ssl;              // From <boost/asio/ssl.hpp>
using tcp = net::ip::tcp;              // From <boost/asio/ip/tcp.hpp>

class WebSocketClient {
public:
    WebSocketClient(net::io_context& ioc, ssl::context& ctx)
        : resolver_(net::make_strand(ioc)),
          ws_(net::make_strand(ioc), ctx) {}

    void connect(const std::string& host, const std::string& port, const std::string& path) {
        // Look up the domain name
        resolver_.async_resolve(host, port,
            beast::bind_front_handler(&WebSocketClient::on_resolve, this, path));
    }

    void send_message(const std::string& message) {
        ws_.async_write(
            net::buffer(message),
            beast::bind_front_handler(&WebSocketClient::on_write, this));
    }

private:
    tcp::resolver resolver_;
    websocket::stream<beast::ssl_stream<tcp::socket>> ws_;

    void on_resolve(const std::string& path,
                    beast::error_code ec,
                    tcp::resolver::results_type results) {
        if (ec) {
            std::cerr << "Resolve Error: " << ec.message() << "\n";
            return;
        }

        // Make the connection on the IP address we get from a lookup
        beast::get_lowest_layer(ws_).async_connect(
            results,
            beast::bind_front_handler(&WebSocketClient::on_connect, this, path));
    }

    void on_connect(const std::string& path,
                    beast::error_code ec,
                    const tcp::resolver::results_type::endpoint_type&) {
        if (ec) {
            std::cerr << "Connect Error: " << ec.message() << "\n";
            return;
        }

        // Perform the SSL handshake
        ws_.next_layer().async_handshake(
            ssl::stream_base::client,
            beast::bind_front_handler(&WebSocketClient::on_ssl_handshake, this, path));
    }

    void on_ssl_handshake(const std::string& path, beast::error_code ec) {
        if (ec) {
            std::cerr << "SSL Handshake Error: " << ec.message() << "\n";
            return;
        }

        // Perform the WebSocket handshake
        ws_.async_handshake("www.deribit.com", path,
            beast::bind_front_handler(&WebSocketClient::on_handshake, this));
    }

    void on_handshake(beast::error_code ec) {
        if (ec) {
            std::cerr << "Handshake Error: " << ec.message() << "\n";
            return;
        }

        std::cout << "WebSocket Handshake Complete!\n";

        // Send an authentication request
        Json::Value auth_request;
        auth_request["jsonrpc"] = "2.0";
        auth_request["id"] = 1;
        auth_request["method"] = "public/auth";
        auth_request["params"]["grant_type"] = "client_credentials";
        auth_request["params"]["client_id"] = "your_client_id";
        auth_request["params"]["client_secret"] = "your_client_secret";

        Json::StreamWriterBuilder writer;
        std::string message = Json::writeString(writer, auth_request);
        send_message(message);
    }

    void on_write(beast::error_code ec, std::size_t bytes_transferred) {
        if (ec) {
            std::cerr << "Write Error: " << ec.message() << "\n";
            return;
        }

        std::cout << "Message Sent (" << bytes_transferred << " bytes).\n";

        // Start reading messages
        do_read();
    }

    void do_read() {
        ws_.async_read(
            buffer_,
            beast::bind_front_handler(&WebSocketClient::on_read, this));
    }

    void on_read(beast::error_code ec, std::size_t bytes_transferred) {
        if (ec) {
            std::cerr << "Read Error: " << ec.message() << "\n";
            return;
        }

        std::cout << "Received: " << beast::make_printable(buffer_.data()) << "\n";

        // Clear the buffer and read the next message
        buffer_.consume(buffer_.size());
        do_read();
    }

    beast::flat_buffer buffer_; // Buffer for incoming messages
};

int main() {
    const std::string host = "www.deribit.com";
    const std::string port = "443";
    const std::string path = "/ws/api/v2";

    // Set up the I/O context and SSL context
    net::io_context ioc;
    ssl::context ctx(ssl::context::tlsv12_client);

    // Verify the certificate
    ctx.set_default_verify_paths();

    WebSocketClient client(ioc, ctx);
    client.connect(host, port, path);

    ioc.run();

    return 0;
}
