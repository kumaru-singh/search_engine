#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include <chrono>
#include <iomanip>

namespace fs = std::filesystem;

std::unordered_map<std::string, std::unordered_map<std::string, int>> invertedIndex;
std::unordered_map<std::string, std::vector<std::string>> fileContents;
std::unordered_set<std::string> stopwords = {"the", "is", "and", "a", "an", "of", "in", "to", "on"};

std::string cleanWord(const std::string& word) {
    std::string result;
    for (char c : word) {
        if (std::isalpha(c)) result += std::tolower(c);
    }
    return result;
}

std::vector<std::string> tokenize(const std::string& line) {
    std::stringstream ss(line);
    std::string word;
    std::vector<std::string> tokens;
    while (ss >> word) {
        std::string cleaned = cleanWord(word);
        if (!cleaned.empty()) tokens.push_back(cleaned);
    }
    return tokens;
}

void indexFile(const std::string& path, const std::string& name) {
    std::ifstream file(path);
    std::string line;

    while (std::getline(file, line)) {
        fileContents[name].push_back(line);
        auto tokens = tokenize(line);
        for (const auto& token : tokens) {
            if (!stopwords.count(token)) {
                invertedIndex[token][name]++;
            }
        }
    }
}

void buildIndex(const std::string& folder) {
    for (const auto& entry : fs::directory_iterator(folder)) {
        if (entry.path().extension() == ".txt") {
            std::string fullPath = entry.path().string();
            std::string fileName = entry.path().filename().string();
            indexFile(fullPath, fileName);
        }
    }
}

void showTimestamp(const std::string& filePath) {
    auto ftime = fs::last_write_time(filePath);
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
    std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);
    std::cout << "  Last modified: " << std::put_time(std::localtime(&cftime), "%F %T") << "\n";
}

bool containsPhrase(const std::string& phrase, const std::string& line) {
    std::string lowerLine = line;
    std::string lowerPhrase = phrase;
    std::transform(lowerLine.begin(), lowerLine.end(), lowerLine.begin(), ::tolower);
    std::transform(lowerPhrase.begin(), lowerPhrase.end(), lowerPhrase.begin(), ::tolower);
    return lowerLine.find(lowerPhrase) != std::string::npos;
}

void search(const std::string& query, const std::string& folder) {
    if (query.size() >= 2 && query.front() == '"' && query.back() == '"') {
        std::string phrase = query.substr(1, query.size() - 2);
        std::cout << "\nSearching for exact phrase: '" << phrase << "'\n";
        for (const auto& [filename, lines] : fileContents) {
            for (const auto& line : lines) {
                if (containsPhrase(phrase, line)) {
                    std::cout << "\nFile: " << filename << "\n  Preview: " << line << "\n";
                    showTimestamp(folder + "/" + filename);
                    break;
                }
            }
        }
        return;
    }

    std::vector<std::string> terms = tokenize(query);
    std::unordered_map<std::string, int> fileScores;

    for (const auto& term : terms) {
        if (invertedIndex.count(term)) {
            for (const auto& [filename, count] : invertedIndex[term]) {
                fileScores[filename] += count;
            }
        }
    }

    if (fileScores.empty()) {
        std::cout << "\nNo results found.\n";
        return;
    }

    std::vector<std::pair<std::string, int>> ranked(fileScores.begin(), fileScores.end());
    std::sort(ranked.begin(), ranked.end(), [](const auto& a, const auto& b) {
        return b.second < a.second;
    });

    std::cout << "\nSearch results (ranked):\n";
    for (const auto& [file, score] : ranked) {
        std::cout << "\nFile: " << file << " (score: " << score << ")\n";
        for (const auto& line : fileContents[file]) {
            for (const auto& term : terms) {
                if (containsPhrase(term, line)) {
                    std::cout << "  Preview: " << line << "\n";
                    showTimestamp(folder + "/" + file);
                    goto nextFile;
                }
            }
        }
        nextFile:;
    }
}

int main() {
    std::string folderPath = "./texts";
    buildIndex(folderPath);

    std::string input;
    std::cout << "\nSearch Engine Ready. Type 'exit' to quit.\n";
    while (true) {
        std::cout << "\nEnter query (use quotes for phrase search): ";
        std::getline(std::cin, input);
        if (input == "exit") break;
        search(input, folderPath);
    }
    return 0;
}
