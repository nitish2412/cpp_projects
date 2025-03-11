#ifndef AUTH_MANAGER_HPP
#define AUTH_MANAGER_HPP

#include <string>
#include "../hpp/Logger.hpp"

class AuthManager {
private:
    std::string client_id;
    std::string client_secret;
    std::string access_token;

    std::shared_ptr<spdlog::logger> logger; // Store logger as a member

public:
    AuthManager(const std::string& client_id, const std::string& client_secret);
    bool authenticate();
    std::string getAccessToken() const;
};

#endif // AUTH_MANAGER_HPP
