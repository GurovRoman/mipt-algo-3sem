#include <iostream>
#include <vector>
#include <string>

using std::vector;

class AhoCorasickTrie {
private:
    struct Node {
        vector<int> children;
        int parent = -1;
        char char_from_parent = 0;
        bool terminal = false;
        vector<int> terminal_pattern_positions;
        int failure_link = -1;
        int output_link = -1;
    };

    vector<Node> trie;
    int pattern_count = 0;
    int pattern_size = 0;
    vector<int> occurrences;
    static const size_t ALPHABET_SIZE = 26;

public:
    void constructFromPattern(const std::string &pattern);
    int getNextNode(int node_id, char c);
    int getFailureLink(int node_id);
    int getOutputLink(int node_id);
    void traverse(const std::string& string);
    void printOccurrences();
};


void AhoCorasickTrie::constructFromPattern(const std::string &pattern) {
    trie.reserve(pattern.size());
    trie.emplace_back();

    int current = 0;
    for (int i = 0; i < pattern.size(); ++i) {
        char c = pattern[i];
        bool is_last_letter = (i + 1 == pattern.size()) || pattern[i + 1] == '?';
        if (c != '?') {
            if (!trie[current].children[c]) {
                trie.emplace_back();
                trie.back().children = vector<int>(ALPHABET_SIZE, 0);
                trie.back().parent = current;
                trie.back().char_from_parent = c;
                trie[current].children[c] = trie.size() - 1;
            }
            current = trie[current].children[c];
            if (is_last_letter) {
                trie[current].terminal = true;
                trie[current].terminal_pattern_positions.push_back(i);
                ++pattern_count;
            }
        } else {
            current = 0;
        }
    }

    pattern_size = pattern.size();
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
    Node& node = trie[node_id];
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
    Node& node = trie[node_id];
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

void AhoCorasickTrie::traverse(const std::string &string) {
    occurrences.clear();
    occurrences.resize(string.size(), 0);

    int current_node = 0;
    for (int i = 0; i < string.size(); ++i) {
        current_node = getNextNode(current_node, string[i]);

        int current_output_node = current_node;
        if (!trie[current_output_node].terminal) {
            current_output_node = getOutputLink(current_output_node);
        }

        while (current_output_node) {
            for (auto pos : trie[current_output_node].terminal_pattern_positions) {
                if (i >= pos) {
                    occurrences[i - pos] += 1;
                }
            }
            current_output_node = getOutputLink(current_output_node);
        }
    }
}

void AhoCorasickTrie::printOccurrences() {
    for (int i = 0; i < occurrences.size(); ++i) {
        if (occurrences[i] == pattern_count && i + pattern_size <= occurrences.size()) {
            std::cout << i << ' ';
        }
    }
}

int main() {
    std::string string;
    std::string pattern;

    std::cin >> pattern >> string;

    AhoCorasickTrie trie;
    trie.constructFromPattern(pattern);
    trie.traverse(string);
    trie.printOccurrences();
}
