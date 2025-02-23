#ifndef MESSAGE_HANDLER_HPP
#define MESSAGE_HANDLER_HPP

#include <string>

class SubscriptionManager;

class MessageHandler {
public:
    //MessageHandler();
    MessageHandler(SubscriptionManager& manager);

    void processMessage(const std::string& message);

private:
    SubscriptionManager& subscriptionManager;

    void handleSubscriptionEvent(const std::string& topic, const std::string& data);
    void handleErrorEvent(const std::string& errorMessage);

    // Add more specific message handlers as required
};

#endif // MESSAGE_HANDLER_HPP
