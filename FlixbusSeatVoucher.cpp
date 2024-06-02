#include <iostream>
#include <regex>
#include <string>
#include <curl/curl.h>
#include <sstream>
#include <iomanip>
#include <cstdlib>

bool isValidEmail(const std::string &email) {
    const std::regex pattern(R"((^[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+@[a-zA-Z0-9-]+(?:\.[a-zA-Z0-9-]+)*$))");
    return std::regex_match(email, pattern);
}

std::string urlEncode(const std::string &value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (char c : value) {
        if (isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
        } else {
            escaped << '%' << std::setw(2) << int(static_cast<unsigned char>(c));
        }
    }

    return escaped.str();
}

// Callback function to handle the data received from the server
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to analyze the response and provide feedback
void analyzeResponse(const std::string &response) {
    if (response.find("Error | FlixBus") != std::string::npos) {
        std::cout << "Error: You have already redeemed a voucher using that email." << std::endl;
    } else if (response.find("Thank you!") != std::string::npos) {
        std::cout << "Success: The code has been sent to your email address!" << std::endl;
    } else {
        std::cout << "Unexpected response from server." << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <email>" << std::endl;
        return 1;
    }

    const char* url = std::getenv("VOUCHER_URL");
    if (!url) {
        std::cerr << "Error: VOUCHER_URL environment variable not set." << std::endl;
        return 1;
    }

    std::string email = argv[1];

    if (!isValidEmail(email)) {
        std::cerr << "Invalid email format." << std::endl;
        return 1;
    }

    std::string encodedEmail = urlEncode(email);

    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7");
        headers = curl_slist_append(headers, "accept-language: en-US,en;q=0.9");
        headers = curl_slist_append(headers, "cache-control: max-age=0");
        headers = curl_slist_append(headers, "content-type: application/x-www-form-urlencoded");
        headers = curl_slist_append(headers, "cookie: PH_HPXY_CHECK=s1");
        headers = curl_slist_append(headers, "origin: https://www.hiphiphour.com");
        headers = curl_slist_append(headers, "priority: u=0, i");
        std::string refererHeader = "referer: ";
        refererHeader += url;
        headers = curl_slist_append(headers, refererHeader.c_str());
        headers = curl_slist_append(headers, "sec-ch-ua: \"Google Chrome\";v=\"125\", \"Chromium\";v=\"125\", \"Not.A/Brand\";v=\"24\"");
        headers = curl_slist_append(headers, "sec-ch-ua-mobile: ?0");
        headers = curl_slist_append(headers, "sec-ch-ua-platform: \"Windows\"");
        headers = curl_slist_append(headers, "sec-fetch-dest: document");
        headers = curl_slist_append(headers, "sec-fetch-mode: navigate");
        headers = curl_slist_append(headers, "sec-fetch-site: same-origin");
        headers = curl_slist_append(headers, "sec-fetch-user: ?1");
        headers = curl_slist_append(headers, "upgrade-insecure-requests: 1");
        headers = curl_slist_append(headers, "user-agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/125.0.0.0 Safari/537.36");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        std::string postFields = "action=redeem&first_field=test&email=" + encodedEmail + "&checkbox_field=on";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            std::cerr << "cURL request failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            analyzeResponse(readBuffer);
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return 0;
}
