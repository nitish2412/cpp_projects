#pragma once
#include "ClientSession.hpp"
#include "SubscriptionManager.hpp"
#include <string>

class MessageHandler {
public:
    explicit MessageHandler(SubscriptionManager& subManager);
    void handleMessage(ClientSession* client, const std::string& message);

private:
    SubscriptionManager& subscriptionManager;
};
