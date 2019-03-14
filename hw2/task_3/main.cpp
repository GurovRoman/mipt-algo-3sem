#include <iostream>
#include <vector>
#include <string>

using std::vector;

class SuffixArrayWithLCP {
private:
    static const size_t ALPHABET_SIZE = 255;  //maximum ascii value of a character in used strings
    std::string string;
    vector<size_t> suffix_array;
    vector<size_t> lcp_array;

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


const size_t SuffixArrayWithLCP::ALPHABET_SIZE;

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
            vector<size_t> counter(std::max<size_t>(size + 1, ALPHABET_SIZE));
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

        suffix_array.resize(size);
        for (int i = 0; i < size; ++i) {
            suffix_array[i] = suffixes[i].index;
        }
    }
}

void SuffixArrayWithLCP::constructLCP() {
    const size_t size = suffix_array.size();
    lcp_array.resize(size);
    vector<size_t> index_to_suffix(size);
    for (size_t i = 0; i < size; ++i) {
        index_to_suffix[suffix_array[i]] = i;
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
            size_t next = suffix_array[index_to_suffix[current] + 1];
            while (current + prev_lcp < size
                   && next + prev_lcp < size
                   && string[current + prev_lcp] == string[next + prev_lcp]) {
                ++prev_lcp;
            }
        }
        lcp_array[index_to_suffix[current]] = prev_lcp;
    }
}

SuffixArrayWithLCP::SuffixArrayWithLCP(std::string input) : string(std::move(input)) {
    constructSuffix();
    constructLCP();
}

inline size_t SuffixArrayWithLCP::operator[](size_t pos) const {
    return suffix_array[pos];
}

inline size_t SuffixArrayWithLCP::lcp(size_t pos) const {
    return lcp_array[pos];
}


std::string getKthCommonSubstring(const std::string& first, const std:: string& second, unsigned long long k) {
    std::string string = first + "#" + second;
    SuffixArrayWithLCP suf(string);
    size_t current = 0;
    size_t prev_lcp = 0;
    size_t i;
    for (i = 1; i < string.size() - 1 && current < k; ++i) {
        if ((suf[i] < first.size()) != (suf[i + 1] < first.size())) {
            if (suf.lcp(i) > prev_lcp) {
                current += suf.lcp(i) - prev_lcp;
            }
            prev_lcp = suf.lcp(i);
        }
        if (suf.lcp(i) < prev_lcp) {
            prev_lcp = suf.lcp(i);
        }
    }
    if (current < k) {
        return "-1";
    }
    return string.substr(suf[i - 1], k - (current - prev_lcp));
}

int main() {
    std::string first, second;
    unsigned long long k;
    std::cin >> first >> second >> k;
    std::cout << getKthCommonSubstring(first, second, k);
    return 0;
}
