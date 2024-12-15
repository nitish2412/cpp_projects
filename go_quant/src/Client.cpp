#include "../hpp/Client.hpp"

Client::Client(tcp::socket&& socket) : ws(std::move(socket)) {}
