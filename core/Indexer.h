#pragma once
#include <unordered_map>
#include <vector>
#include <string>

struct Posting { int docId; int freq; };

class Indexer {
public:
    void addDocument(int docId, const std::string& text);
    const std::unordered_map<std::string,std::vector<Posting>>& getIndex() const;
private:
    std::unordered_map<std::string,std::vector<Posting>> index;
};
