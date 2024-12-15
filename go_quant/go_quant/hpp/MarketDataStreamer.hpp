#ifndef MARKET_DATA_STREAMER_HPP
#define MARKET_DATA_STREAMER_HPP

#include "ClientManager.hpp"

class MarketDataStreamer {
public:
    explicit MarketDataStreamer(ClientManager& manager);
    void start();

private:
    ClientManager& clientManager_;
};

#endif // MARKET_DATA_STREAMER_HPP
