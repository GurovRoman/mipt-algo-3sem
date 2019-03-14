#include <iostream>
#include <vector>
#include <string>

using std::vector;

class AhoCorasickTrie {
private:
    struct Node {
        vector<int> children = vector<int>(1 << (sizeof(char) * 8 - 1), 0);
        int parent = -1;
        char char_from_parent = 0;
        bool terminal = false;
        vector<int> terminal_pattern_ids;
        int failure_link = -1;
        int output_link = -1;
    };

    vector<Node> trie;

    int current_traversal_node = 0;

public:
    AhoCorasickTrie();

    void addPatternToTrie(const std::string &pattern, int pattern_id);

    int getNextNode(int node_id, char c);

    int getFailureLink(int node_id);

    int getOutputLink(int node_id);

    std::vector<int> traverseLetter(char letter);

    void resetTraversal();

};


AhoCorasickTrie::AhoCorasickTrie() {
    trie.emplace_back();
}

void AhoCorasickTrie::addPatternToTrie(const std::string &pattern, int pattern_id) {
    int current = 0;
    for (int i = 0; i < pattern.size(); ++i) {
        char c = pattern[i] - 'a';
        if (!trie[current].children[c]) {
            trie.emplace_back();
            trie.back().parent = current;
            trie.back().char_from_parent = c;
            trie[current].children[c] = trie.size() - 1;
        }
        current = trie[current].children[c];
        if (i == pattern.size() - 1) {
            trie[current].terminal = true;
            trie[current].terminal_pattern_ids.push_back(pattern_id);
        }
    }
}


int AhoCorasickTrie::getNextNode(int node_id, char c) {
    if (trie[node_id].children[c]) {
        return trie[node_id].children[c];
    } else if (node_id == 0) {
        return 0;
    } else {
        return getNextNode(getFailureLink(node_id), c);
    }
}

int AhoCorasickTrie::getFailureLink(int node_id) {
    Node &node = trie[node_id];
    if (node.failure_link == -1) {
        if (node.parent <= 0) {
            node.failure_link = 0;
        } else {
            node.failure_link = getNextNode(getFailureLink(node.parent),
                                            node.char_from_parent);
        }
    }
    return node.failure_link;
}

int AhoCorasickTrie::getOutputLink(int node_id) {
    Node &node = trie[node_id];
    if (node.output_link == -1) {
        int failure_link = getFailureLink(node_id);
        if (trie[failure_link].parent == -1) {
            node.output_link = 0;
        } else if (trie[failure_link].terminal) {
            node.output_link = failure_link;
        } else {
            node.output_link = getOutputLink(failure_link);
        }
    }
    return node.output_link;
}

std::vector<int> AhoCorasickTrie::traverseLetter(char letter) {
    current_traversal_node = getNextNode(current_traversal_node, letter - 'a');

    int current_output_node = current_traversal_node;
    if (!trie[current_output_node].terminal) {
        current_output_node = getOutputLink(current_output_node);
    }

    std::vector<int> output;
    while (current_output_node) {
        for (auto id : trie[current_output_node].terminal_pattern_ids) {
            output.push_back(id);
        }
        current_output_node = getOutputLink(current_output_node);
    }

    return output;
}

void AhoCorasickTrie::resetTraversal() {
    current_traversal_node = 0;
}


void printWildcardPatternOccurrences(const std::string &pattern, const std::string &string) {
    AhoCorasickTrie trie;

    int pattern_count = 0;
    vector<int> pattern_end_positions;

    std::string pattern_part;
    for (int i = 0; i < pattern.size(); ++i) {
        char c = pattern[i];
        if (c == '?') {
            pattern_part.clear();
            continue;
        }
        pattern_part += c;
        bool is_last_letter = (i + 1 == pattern.size()) || pattern[i + 1] == '?';
        if (is_last_letter) {
            pattern_end_positions.push_back(i);
            trie.addPatternToTrie(pattern_part, pattern_end_positions.size() - 1);
            ++pattern_count;
        }
    }

    vector<int> occurrences;
    occurrences.resize(string.size(), 0);

    for (int i = 0; i < string.size(); ++i) {
        auto occurrence_ids = trie.traverseLetter(string[i]);
        for (auto id : occurrence_ids) {
            auto pos = pattern_end_positions[id];
            if (i >= pos) {
                occurrences[i - pos] += 1;
            }
        }
    }

    for (int i = 0; i < occurrences.size(); ++i) {
        if (occurrences[i] == pattern_count && i + pattern.size() <= occurrences.size()) {
            std::cout << i << ' ';
        }
    }
}

int main() {
    std::string string;
    std::string pattern;

    std::cin >> pattern >> string;

    printWildcardPatternOccurrences(pattern, string);
}
