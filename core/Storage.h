#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "Indexer.h"

class Storage {
public:
    static void saveIndex(const Indexer& idx, const std::string& path);
    static void loadIndex(Indexer& idx, const std::string& path);
};
