#ifndef CLIENT_MANAGER_HPP
#define CLIENT_MANAGER_HPP

#include "Client.hpp"
#include <memory>
#include <mutex>
#include <set>
#include <vector>

class ClientManager {
public:
    void addClient(std::shared_ptr<Client> client);
    void removeClient(std::shared_ptr<Client> client);
    void subscribeClient(std::shared_ptr<Client> client, const std::string& symbol);
    void unsubscribeClient(std::shared_ptr<Client> client, const std::string& symbol);
    std::vector<std::shared_ptr<Client>> getClientsForSymbol(const std::string& symbol);

private:
    std::set<std::shared_ptr<Client>> clients_;
    std::mutex mutex_;
};

#endif // CLIENT_MANAGER_HPP
