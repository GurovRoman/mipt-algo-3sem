#include <iostream>
#include <string>
#include <vector>

std::vector<int> stringToPrefix(const std::string& string) {
    std::vector<int> prefixFunction;
    prefixFunction.push_back(0);
    for (int i = 1; i < string.size(); ++i) {
        int candidate = prefixFunction.back();
        while (candidate > 0 && string[i] != string[candidate]) {
            candidate = prefixFunction[candidate - 1];
        }
        if (string[i] == string[candidate]) {
            ++candidate;
        }
        prefixFunction.push_back(candidate);
    }
    return prefixFunction;
}

std::vector<int> findOccurrences(const std::string& string, const std::string& pattern) {
    std::vector<int> occurrences;
    std::vector<int> prefixFunction = stringToPrefix(pattern);
    int candidate = 0;
    for (int i = 0; i < string.size(); ++i) {
        while (candidate > 0 && string[i] != pattern[candidate]) {
            candidate = prefixFunction[candidate - 1];
        }
        if (string[i] == pattern[candidate]) {
            ++candidate;
        }
        if (candidate == pattern.size()) {
            occurrences.push_back(i - pattern.size() + 1);
            candidate = prefixFunction[candidate - 1];
        }
    }
    return occurrences;
}

int main() {
    std::string pattern;
    std::string string;

    std::cin >> pattern >> string;

    pattern;

    auto occurrences = findOccurrences(string, pattern);

    for (auto i : occurrences) {
        std::cout << i << ' ';
    }
}
