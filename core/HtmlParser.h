#pragma once
#include <string>

class HtmlParser {
public:
    static std::string stripTags(const std::string& html);
};
