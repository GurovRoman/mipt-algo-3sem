#include <iostream>
#include <vector>
#include <string>

using std::vector;

class SuffixArrayWithLCP {
private:
    const size_t alphabet_size = 255;  //maximum ascii value of a character in used strings
    std::string string;
    vector<size_t> suffixArray;
    vector<size_t> lcpArray;

    struct RankedSuffix {
        size_t index;
        int rank;
        int rank_next;
    };

    void constructSuffix();
    void constructLCP();

public:
    SuffixArrayWithLCP() = default;
    explicit SuffixArrayWithLCP(std::string input);

    size_t operator[](size_t pos) const;
    size_t lcp(size_t pos) const;
};


void SuffixArrayWithLCP::constructSuffix() {
    const size_t size = string.size();
    vector<RankedSuffix> suffixes(size);
    for (size_t i = 0; i < size; ++i) {
        suffixes[i].index = i;
    }
    for (size_t cycle_size = 2; cycle_size < 2 * size; cycle_size <<= 1) {
        if (cycle_size == 2) {
            for (size_t i = 0; i < size; ++i) {
                suffixes[i].rank = string[i];
                suffixes[i].rank_next = i + 1 < size ? string[i + 1] : -1;
            }
        }
        else {
            int prev_rank = 0;
            vector<size_t> index_to_suffix(size);
            for (size_t i = 0; i < size; ++i) {
                if (i == 0) {
                    prev_rank = suffixes[i].rank;
                    suffixes[i].rank = 0;
                }
                else {
                    if (suffixes[i].rank == prev_rank &&
                        suffixes[i].rank_next == suffixes[i - 1].rank_next)
                    {
                        suffixes[i].rank = suffixes[i - 1].rank;
                    }
                    else {
                        prev_rank = suffixes[i].rank;
                        suffixes[i].rank = suffixes[i - 1].rank + 1;
                    }
                    index_to_suffix[suffixes[i].index] = i;
                }
            }
            for (size_t i = 0; i < size; ++i) {
                size_t next_index = suffixes[i].index + cycle_size / 2;
                suffixes[i].rank_next = next_index < size
                                        ? suffixes[index_to_suffix[next_index]].rank
                                        : -1;
            }
        }

        for (int rank_type = 0; rank_type < 2; ++rank_type) {
            vector<RankedSuffix> buffer(size);
            vector<size_t> counter(std::max<size_t>(size + 1, alphabet_size));
            for (int i = 0; i < size; ++i) {
                int value = rank_type ? suffixes[i].rank : suffixes[i].rank_next;
                ++counter[value + 1];
            }
            for (int i = 1; i < counter.size(); ++i) {
                counter[i] += counter[i - 1];
            }
            for (size_t i = size; i > 0; --i) {
                int value = rank_type ? suffixes[i - 1].rank : suffixes[i - 1].rank_next;
                buffer[counter[value + 1] - 1] = suffixes[i - 1];
                counter[value + 1]--;
            }
            for (size_t i = 0; i < size; i++) {
                suffixes[i] = buffer[i];
            }
        }

        suffixArray.resize(size);
        for (int i = 0; i < size; ++i) {
            suffixArray[i] = suffixes[i].index;
        }
    }
}

void SuffixArrayWithLCP::constructLCP() {
    const size_t size = suffixArray.size();
    lcpArray.resize(size);
    vector<size_t> index_to_suffix(size);
    for (size_t i = 0; i < size; ++i) {
        index_to_suffix[suffixArray[i]] = i;
    }
    size_t prev_lcp = 0;
    for (int current = 0; current < size; ++current) {
        if (index_to_suffix[current] == size - 1) {
            prev_lcp = 0;
        }
        else {
            if (prev_lcp > 0) {
                --prev_lcp;
            }
            size_t next = suffixArray[index_to_suffix[current] + 1];
            while (current + prev_lcp < size
                   && next + prev_lcp < size
                   && string[current + prev_lcp] == string[next + prev_lcp]) {
                ++prev_lcp;
            }
        }
        lcpArray[index_to_suffix[current]] = prev_lcp;
    }
}

SuffixArrayWithLCP::SuffixArrayWithLCP(std::string input) : string(std::move(input)) {
    constructSuffix();
    constructLCP();
}

size_t SuffixArrayWithLCP::operator[](size_t pos) const {
    return suffixArray[pos];
}

size_t SuffixArrayWithLCP::lcp(size_t pos) const {
    return lcpArray[pos];
}


class TwoSuffixTrie {
private:
    struct Node {
        size_t parent;
        bool type;
        size_t left;
        size_t right;
        Node(size_t  a, bool b, size_t c, size_t d) : parent(a), type(b), left(c), right(d) {}
    };
    SuffixArrayWithLCP suf;
    vector<Node> trie;
    vector<vector<size_t>> children;

    vector<size_t> lex_index;
    size_t free_lex = 0;

    void printLexOrderRec(size_t node_id) {
        if (node_id != 0 && lex_index[node_id] == 0) {
            lex_index[node_id] = free_lex++;
        }
        if (node_id != 0) {
            Node node = trie[node_id];
            std::cout << lex_index[node.parent] << ' ' << node.type << ' ' << node.left << ' ' << node.right << '\n';
        }
        for (auto child_id : children[node_id]) {
            printLexOrderRec(child_id);
        }
    }

public:
    TwoSuffixTrie(const std::string& first, const std::string& second) {
        std::string string = first + second;
        suf = SuffixArrayWithLCP(string);
        trie.emplace_back(0, 0, 0, 0);
        size_t depth = 0;
        size_t current_node = 0;
        for (size_t i = 0; i < string.size(); ++i) {
            size_t lcp = i > 0 ? suf.lcp(i - 1) : 0;
            size_t last_node;
            while (depth > lcp) {
                depth -= trie[current_node].right - trie[current_node].left;
                last_node = current_node;
                current_node = trie[current_node].parent;
            }
            if (depth == lcp) {
                size_t val = suf[i] + lcp;
                bool type = suf[i] >= first.size();
                size_t left = val - first.size() * type;
                size_t right = type ? second.size() : first.size();
                trie.emplace_back(current_node, type, left, right);
                current_node = trie.size() - 1;
                depth += trie[current_node].right - trie[current_node].left;
            }
            else {
                bool type = trie[last_node].type;
                size_t left = trie[last_node].left;
                size_t right = trie[last_node].right;
                size_t middle = left - depth + lcp;
                trie.emplace_back(current_node, type, left, middle);
                trie[last_node] = Node(trie.size() - 1, type, middle, right);
                current_node = trie.size() - 1;
                depth += trie[current_node].right - trie[current_node].left;

                size_t val = suf[i] + lcp;
                type = suf[i] >= first.size();
                left = val - first.size() * type;
                right = type ? second.size() : first.size();
                trie.emplace_back(trie.size() - 1, type, left, right);
                current_node = trie.size() - 1;
                depth += trie[current_node].right - trie[current_node].left;

            }
        }
        children.resize(trie.size(), vector<size_t>());
        for (size_t i = 1; i < trie.size(); ++i) {
            children[trie[i].parent].push_back(i);
        }
    }

    void printLexOrder(size_t node_id) {
        lex_index.resize(trie.size(), 0);
        free_lex = 1;
        printLexOrderRec(node_id);
    }

    size_t getNodeCount() const {
        return trie.size();
    }
};

int main() {
    std::string first, second;
    std::cin >> first >> second;
    TwoSuffixTrie trie(first, second);
    std::cout << trie.getNodeCount() << '\n';
    trie.printLexOrder(0);
    return 0;
}
