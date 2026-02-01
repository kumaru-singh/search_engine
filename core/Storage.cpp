#include "Storage.h"
#include <fstream>

void Storage::saveIndex(const Indexer& idx, const std::string& path) {
    std::ofstream out(path, std::ios::binary);
    const auto& index = idx.getIndex();

    size_t terms = index.size();
    out.write((char*)&terms, sizeof(terms));

    for (auto& [term, postings] : index) {
        size_t len = term.size();
        out.write((char*)&len, sizeof(len));
        out.write(term.data(), len);

        size_t pcount = postings.size();
        out.write((char*)&pcount, sizeof(pcount));
        out.write((char*)postings.data(), pcount * sizeof(Posting));
    }
}
