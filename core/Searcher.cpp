#include "Searcher.h"
#include "../utils/Tokenizer.h"
#include <algorithm>
#include <cmath>

Searcher::Searcher(const Indexer& i) : idx(i) {}

std::vector<SearchResult> Searcher::search(const std::string& q) {
    auto terms = Tokenizer::tokenize(q);
    std::unordered_map<int,double> scores;

    for (auto& term : terms) {
        auto it = idx.getIndex().find(term);
        if (it == idx.getIndex().end()) continue;
        for (auto& p : it->second)
            scores[p.docId] += p.freq;
    }

    std::vector<SearchResult> res;
    for (auto& [d,s] : scores) res.push_back({d,s});
    std::sort(res.begin(), res.end(), [](auto&a,auto&b){return a.score>b.score;});
    return res;
}
