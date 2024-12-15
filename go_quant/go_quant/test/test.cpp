#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <iostream>

// Namespace aliases for convenience
namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;

int main() {
    try {
        // Create I/O context and WebSocket connection
        net::io_context ioc;
        websocket::stream<beast::tcp_stream> ws(ioc);

        // Connect and send a WebSocket request
        auto const host = "echo.websocket.org";
        auto const port = "80";
        net::ip::tcp::resolver resolver(ioc);
        auto const results = resolver.resolve(host, port);
        ws.next_layer().connect(*results);
        ws.handshake(host, "/");

        // Send a message
        ws.write(net::buffer(std::string("Hello, world!")));

        // Receive a response
        beast::flat_buffer buffer;
        ws.read(buffer);
        std::cout << beast::buffers_to_string(buffer.data()) << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
