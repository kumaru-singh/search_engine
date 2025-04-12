// Web Crawler and Extras (save web pages, index subfolders, test functionality)

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <curl/curl.h>

namespace fs = std::filesystem;

// Write callback for libcurl
size_t writeToFile(void* ptr, size_t size, size_t nmemb, void* stream) {
    std::ofstream* out = static_cast<std::ofstream*>(stream);
    size_t totalSize = size * nmemb;
    out->write(static_cast<char*>(ptr), totalSize);
    return totalSize;
}

bool downloadPage(const std::string& url, const std::string& savePath) {
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    std::ofstream file(savePath);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToFile);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return res == CURLE_OK;
}

// Recursively collect all .txt files
void collectTxtFiles(const std::string& folder, std::vector<std::string>& files) {
    for (const auto& entry : fs::recursive_directory_iterator(folder)) {
        if (entry.path().extension() == ".txt") {
            files.push_back(entry.path().string());
        }
    }
}

// Basic unit test to confirm file creation and download
void runTests() {
    std::cout << "\nRunning basic tests...\n";
    std::string testUrl = "https://example.com";
    std::string testSave = "./texts/example_download.txt";

    bool success = downloadPage(testUrl, testSave);
    if (success && fs::exists(testSave)) {
        std::cout << "✅ Web page download test passed. Saved to " << testSave << "\n";
    } else {
        std::cout << "❌ Web page download test failed.\n";
    }

    std::vector<std::string> txtFiles;
    collectTxtFiles("./texts", txtFiles);
    std::cout << "✅ Indexable .txt files found (including subfolders): " << txtFiles.size() << "\n";
}

int main() {
    std::string url;
    std::cout << "Enter a URL to download and save as text: ";
    std::getline(std::cin, url);
    std::string savePath = "./texts/webpage_" + std::to_string(std::time(nullptr)) + ".txt";

    if (downloadPage(url, savePath)) {
        std::cout << "Downloaded and saved to: " << savePath << "\n";
    } else {
        std::cerr << "Failed to download page.\n";
    }

    runTests();
    return 0;
}
