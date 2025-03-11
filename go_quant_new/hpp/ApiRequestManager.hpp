#ifndef API_REQ_MANAGER_HPP
#define API_REQ_MANAGER_HPP
#include "../hpp/AuthManager.hpp"
#include "../hpp/json.hpp"
#include <curl/curl.h>
#include <string>
#include "../hpp/Logger.hpp"

using json = nlohmann::json;


class ApiRequestManager {
private:
    AuthManager* authManager;
    std::shared_ptr<spdlog::logger> logger; // Store logger as a member

public:
    ApiRequestManager(AuthManager* authManager);
    bool sendRequest(std::string& url, const std::string& method, std::string& request_data, std::string& responseString);

};

#endif // AUTH_MANAGER_HPP

