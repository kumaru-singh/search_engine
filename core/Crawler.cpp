#include "Crawler.h"
#include <curl/curl.h>
#include <iostream>

static size_t WriteCB(void* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*)ptr, size * nmemb);
    return size * nmemb;
}

bool Crawler::fetch(const std::string& url, std::string& out) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "[CRAWLER ERROR] curl init failed\n";
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCB);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    // Force user agent
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64)");

    // Debug info
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_CAINFO, "D:/search_engine/cacert.pem");

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        std::cerr << "[CRAWLER ERROR] "
                  << curl_easy_strerror(res)
                  << " | URL: " << url << "\n";
        curl_easy_cleanup(curl);
        return false;
    }

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    std::cout << "[CRAWLED] " << url
              << " | HTTP " << http_code
              << " | Bytes: " << out.size() << "\n";

    if (http_code != 200 || out.empty()) {
        std::cerr << "[CRAWLER WARNING] Empty or non-200 response\n";
        curl_easy_cleanup(curl);
        return false;
    }

    curl_easy_cleanup(curl);
    return true;
}
