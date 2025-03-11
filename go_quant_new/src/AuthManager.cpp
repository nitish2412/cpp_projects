#include "../hpp/AuthManager.hpp"
#include <curl/curl.h>
#include <iostream>
#include "../hpp/json.hpp"
#include "../hpp/Utils.hpp"

using json = nlohmann::json;

AuthManager::AuthManager(const std::string& client_id, const std::string& client_secret)
    : client_id(client_id), client_secret(client_secret), access_token(""), logger(Logger::getInstance()) {
        SPDLOG_LOGGER_INFO(logger, "AuthManager initaited..");
    }

bool AuthManager::authenticate() {
    CURL* curl = curl_easy_init();
    if (curl) {
        CURLcode res;
        std::string url = "https://test.deribit.com/api/v2/public/auth";

        // Create JSON payload
        json request_json = {
            {"method", "public/auth"},
            {"params", {
                {"grant_type", "client_credentials"},
                {"client_id", client_id},
                {"client_secret", client_secret}
            }},
            {"jsonrpc", "2.0"},
            {"id", 3}
        };
        std::string request_data = request_json.dump();

        // Response storage
        std::string response_string;

        // Set CURL options
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_data.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, []() {
            struct curl_slist* headers = nullptr;
            headers = curl_slist_append(headers, "Content-Type: application/json");
            return headers;
        }());

        // Perform the request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
            std::string curl_error = std::string("CURL error: ") + curl_easy_strerror(res);
            SPDLOG_LOGGER_ERROR(logger, curl_error);
            curl_easy_cleanup(curl);
            return false;
        }

        // Parse the response
        try {
            json response_json = json::parse(response_string);
            if (response_json.contains("result")) {
                access_token = response_json["result"]["access_token"];
                SPDLOG_LOGGER_INFO(logger, "Successfully Get Access token..");
                curl_easy_cleanup(curl);
                return true;
            } else if (response_json.contains("error")) {
                std::cerr << "Authentication failed: " << response_json["error"]["message"] << std::endl;
                std::string error = std::string("Authentication failed: ") + response_json["error"]["message"].dump();
                SPDLOG_LOGGER_ERROR(logger, error);
            } else {
                std::cerr << "Unexpected response: " << response_json.dump(4) << std::endl;
                std::string error = std::string("Unexpected response: ") + response_json.dump(4);
                SPDLOG_LOGGER_ERROR(logger, error);
            }
        } catch (json::parse_error& e) {
            std::cerr << "JSON parsing error: " << e.what() << std::endl;
            std::string error = std::string("JSON parsing error: ") + e.what();
            SPDLOG_LOGGER_ERROR(logger, error);
        }

        // Cleanup
        curl_easy_cleanup(curl);
    } else {
        std::cerr << "CURL initialization failed!" << std::endl;
        std::string error = "CURL initialization failed!";
        SPDLOG_LOGGER_ERROR(logger, error);
    }

    return false;
}

std::string AuthManager::getAccessToken() const {
    return access_token;
}
