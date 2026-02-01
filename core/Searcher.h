#pragma once
#include "Indexer.h"
#include <vector>
#include <string>

struct SearchResult {
    int docId;
    double score;
};

class Searcher {
public:
    explicit Searcher(const Indexer& index);

    std::vector<SearchResult> search(const std::string& query);

private:
    const Indexer& idx;
};
