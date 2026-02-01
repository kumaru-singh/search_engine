#include "Tokenizer.h"
#include <sstream>
#include <cctype>

std::vector<std::string> Tokenizer::tokenize(const std::string& t) {
    std::string clean;
    for(char c:t) clean+=std::isalnum(c)?std::tolower(c):' ';
    std::istringstream iss(clean);
    std::vector<std::string> v; std::string w;
    while(iss>>w) if(w.size()>2) v.push_back(w);
    return v;
}

