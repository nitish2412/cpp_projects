#ifndef API_REQ_MANAGER_HPP
#define API_REQ_MANAGER_HPP
#include "../hpp/AuthManager.hpp"
#include "../hpp/json.hpp"
#include <curl/curl.h>
#include <string>

using json = nlohmann::json;


class ApiRequestManager {
private:
    AuthManager* authManager;

public:
    ApiRequestManager(AuthManager* authManager);
    //bool sendRequest(std::string& url, const std::string& method, json& requestBody, std::string& responseString);
    bool sendRequest(std::string& url, const std::string& method, std::string& request_data, std::string& responseString);

};

#endif // AUTH_MANAGER_HPP

