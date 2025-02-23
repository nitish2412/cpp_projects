#include "../hpp/ApiRequestManager.hpp"
#include "../hpp/json.hpp"
#include <iostream>
#include <curl/curl.h>
#include "../hpp/Utils.hpp"

using json = nlohmann::json;

ApiRequestManager::ApiRequestManager(AuthManager* authManager):authManager(authManager){
        std::cout<<"Api request classs initiated\n";
}

//bool ApiRequestManager::sendRequest(std::string& url, const std::string& method, json& requestBody, std::string& responseString) {
bool ApiRequestManager::sendRequest(std::string& url, const std::string& method,  std::string& request_data, std::string& responseString) {
        CURL* curl = curl_easy_init();
        if (!curl) {
            std::cerr << "Failed to initialize CURL!" << std::endl;
            return false;
        }

        std::string accessToken = authManager->getAccessToken();
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + accessToken).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");
        //std::cout<<"URL:"<<url<<"\n";
        if (method == "POST") {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_data.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, request_data.size());
        } else if (method == "GET") {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
        }
      
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

        CURLcode res = curl_easy_perform(curl);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
            return false;
        }

        return true;
    }