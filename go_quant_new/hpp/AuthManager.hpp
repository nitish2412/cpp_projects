#ifndef AUTH_MANAGER_HPP
#define AUTH_MANAGER_HPP

#include <string>

class AuthManager {
private:
    std::string client_id;
    std::string client_secret;
    std::string access_token;

public:
    AuthManager(const std::string& client_id, const std::string& client_secret);
    bool authenticate();
    std::string getAccessToken() const;
};

#endif // AUTH_MANAGER_HPP
