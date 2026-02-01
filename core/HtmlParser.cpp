#include "HtmlParser.h"
#include <regex>

std::string HtmlParser::stripTags(const std::string& html) {
    std::string text;
    bool inside = false;
    for (char c : html) {
        if (c == '<') inside = true;
        else if (c == '>') inside = false;
        else if (!inside) text += c;
    }
    return text;
}


