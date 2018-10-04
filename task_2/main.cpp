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

std::vector<int> prefixToZ(const std::vector<int>& prefix_function) {
    std::vector<int> z_function(prefix_function.size(), 0);
    z_function[0] = z_function.size();
    for (int i = 1; i < prefix_function.size(); ++i) {
        z_function[i - prefix_function[i] + 1] = prefix_function[i];
    }
    for (int i = 1; i < z_function.size();) {
        if (z_function[i] == 0) {
            ++i;
            continue;
        }
        int skip = 0;
        for (int j = 1; j < z_function[i]; ++j) {
            if (z_function[j] < z_function[i + j]) {
                break;
            }
            z_function[i + j] = std::min(z_function[j], z_function[i] - j);
            skip = j;
        }
        i += skip + 1;
    }
    return z_function;
}

std::string zToString(const std::vector<int>& z_function) {
    std::string result;
    int j = 0;
    int prefix_length = 0;
    std::vector<int> prefix_z_values;
    char alphabet_size = 26;
    char available_char = 0;
    for (int i = 0; i < z_function.size(); ++i) {
        int z_value = i ? z_function[i] : 0;
        if (!z_value && !prefix_length) {
            if (j && available_char < alphabet_size - 1) {
                j = 0;
                std::vector<int> alphabet_mask(alphabet_size, 0);
                for (int prefix_z_value : prefix_z_values) {
                    alphabet_mask[result[prefix_z_value] - 'a'] = 1;
                }
                for (char k = 1; k < alphabet_size; ++k) {
                    if (!alphabet_mask[k]) {
                        available_char = k;
                        break;
                    }
                }
            }
            result.push_back('a' + available_char);

            available_char = 1;
        }
        if (z_value > prefix_length) {
            prefix_length = z_value;
            prefix_z_values.clear();
            j = 0;
        }
        if (z_value == prefix_length && prefix_length) {
            prefix_z_values.push_back(z_value);
        }
        if (prefix_length) {
            result += result[j];
            ++j;
            --prefix_length;
        }
    }
    return result;
}

std::vector<int> stringToZ(const std::string& string) {
    return prefixToZ(stringToPrefix(string));
}

std::vector<int> zToPrefix(const std::vector<int>& z) {
    return stringToPrefix(zToString(z));
}

std::string prefixToString(const std::vector<int>& prefix) {
    return zToString(prefixToZ(prefix));
}

int main() {
    std::string qwe = "acbacbbababcbababc";//"abcbabaabbbcccccabcabc";
    std::cout << qwe << '\n';

    auto p = stringToPrefix(qwe);
    auto z = prefixToZ(p);
    z[0] = 0;
    qwe = zToString(z);

    for (auto i : p) {
        std::cout << i;
    }
    std::cout << '\n';

    for (auto i : z) {
        std::cout << i;
    }
    std::cout << '\n';

    std::cout << qwe << '\n';


    p = stringToPrefix(qwe);
    z = prefixToZ(p);
    z[0] = 0;

    for (auto i : p) {
        std::cout << i;
    }
    std::cout << '\n';

    for (auto i : z) {
        std::cout << i;
    }
    std::cout << '\n';

}