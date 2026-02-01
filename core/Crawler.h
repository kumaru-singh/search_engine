#pragma once
#include <string>

class Crawler {
public:
    bool fetch(const std::string& url, std::string& out);
};
