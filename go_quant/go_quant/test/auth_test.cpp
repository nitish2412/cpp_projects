#include <curl/curl.h>
#include <iostream>
#include "../hpp/json.hpp"

using json = nlohmann::json;

// Callback function to handle the response from the server
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    std::string* response = static_cast<std::string*>(userp);
    response->append(static_cast<char*>(contents), realsize);
    return realsize;
}

int main() {
    // Initialize CURL
    CURL* curl = curl_easy_init();
    std::string access_token;
    if (curl) {
        CURLcode res;
        std::string url = "https://test.deribit.com/api/v2/public/auth";

        // Create JSON payload
        json request_json = {
            {"method", "public/auth"},
            {"params", {
                {"grant_type", "client_credentials"},
                {"client_id", "NwlKlPaT"},
                {"client_secret", "WbfqBP49qWBL1MuacHhK9rXQ5BmBjvKSSOx20_e0QmA"}
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
        } else {
            std::cout << "Request succeeded!" << std::endl;

            // Parse the response to extract the access token
            try {
                json response_json = json::parse(response_string);
                if (response_json.contains("result")) {
                    access_token = response_json["result"]["access_token"];
                    std::cout << "Access Token: " << access_token << std::endl;
                } else if (response_json.contains("error")) {
                    std::cerr << "Authentication failed: " << response_json["error"]["message"] << std::endl;
                } else {
                    std::cerr << "Unexpected response: " << response_json.dump(4) << std::endl;
                }
            } catch (json::parse_error& e) {
                std::cerr << "JSON parsing error: " << e.what() << std::endl;
            }
        }

        // Clean up
        curl_easy_cleanup(curl);
    } else {
        std::cerr << "CURL initialization failed!" << std::endl;
    }

    return 0;
}
