#include "../hpp/ClientManager.hpp"

void ClientManager::addClient(std::shared_ptr<Client> client) {
    std::lock_guard<std::mutex> lock(mutex_);
    clients_.insert(client);
}

void ClientManager::removeClient(std::shared_ptr<Client> client) {
    std::lock_guard<std::mutex> lock(mutex_);
    clients_.erase(client);
}

void ClientManager::subscribeClient(std::shared_ptr<Client> client, const std::string& symbol) {
    std::lock_guard<std::mutex> lock(mutex_);
    client->subscribedSymbols.insert(symbol);
}

void ClientManager::unsubscribeClient(std::shared_ptr<Client> client, const std::string& symbol) {
    std::lock_guard<std::mutex> lock(mutex_);
    client->subscribedSymbols.erase(symbol);
}

std::vector<std::shared_ptr<Client>> ClientManager::getClientsForSymbol(const std::string& symbol) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<std::shared_ptr<Client>> result;
    for (const auto& client : clients_) {
        if (client->subscribedSymbols.count(symbol)) {
            result.push_back(client);
        }
    }
    return result;
}
