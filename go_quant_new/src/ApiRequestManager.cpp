#include "../hpp/ApiRequestManager.hpp"
#include "../hpp/json.hpp"
#include <iostream>
#include <curl/curl.h>
#include "../hpp/Utils.hpp"

using json = nlohmann::json;

ApiRequestManager::ApiRequestManager(AuthManager* authManager):authManager(authManager),logger(Logger::getInstance()){
        std::cout<<"Api request classs initiated\n";
        SPDLOG_LOGGER_INFO(logger, "Api request classs initiated...");
}

bool ApiRequestManager::sendRequest(std::string& url, const std::string& method, std::string& request_data, std::string& responseString) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        SPDLOG_LOGGER_ERROR(logger, "Failed to initialize CURL!");
        return false;
    }

    std::string accessToken = authManager->getAccessToken();
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, ("Authorization: Bearer " + accessToken).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // Log request details
    SPDLOG_LOGGER_DEBUG(logger, "Sending {} request to: {}", method, url);
    SPDLOG_LOGGER_DEBUG(logger, "Authorization: Bearer [REDACTED]");
    SPDLOG_LOGGER_DEBUG(logger, "Content-Type: application/json");
    if (method == "POST") {
        SPDLOG_LOGGER_DEBUG(logger, "Request Body: {}", request_data);
    }

    // Set CURL options
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);
    if (method == "POST") {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_data.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, request_data.size());
    } else if (method == "GET") {
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
    }

    // Perform request
    CURLcode res = curl_easy_perform(curl);

    // Log response details
    long response_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    SPDLOG_LOGGER_DEBUG(logger, "Response Code: {}", response_code);
    SPDLOG_LOGGER_DEBUG(logger, "Response Body: {}", responseString);

    // Cleanup
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    // Check for errors
    if (res != CURLE_OK) {
        SPDLOG_LOGGER_ERROR(logger, "CURL error: {}", curl_easy_strerror(res));
        return false;
    }

    SPDLOG_LOGGER_INFO(logger, "API request successful: {}", url);
    return true;
}