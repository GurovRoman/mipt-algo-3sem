#include <iostream>
#include <iomanip>
#include <sstream>
#include <random>
#include <chrono>
#include "biginteger.h"

using namespace std;

int main() {
    const size_t tries = 13;
    const size_t size = 10000;

    mt19937_64 rd(chrono::system_clock::now().time_since_epoch().count());
    uniform_int_distribution<char> ds_digit('0', '9');

    long long karatsuba_time = 0, naive_time = 0;

    for (int i = 0; i < tries; ++i) {
        string iss_str;
        auto delimiter = size / 2;
        for (size_t j = 0; j < size; ++j) {
            if (j == delimiter) {
                iss_str.push_back(' ');
            }
            if ((j == delimiter || j == 0) && rd() % 2) {
                iss_str.push_back('-');
            }
            iss_str.push_back(ds_digit(rd));
        }
        istringstream iss(iss_str);
        BigInteger a, b, c, d;
        iss >> a >> b;
        c = a;
        d = b;

        auto time_start = chrono::steady_clock::now();
        a *= b;
        auto time_end = chrono::steady_clock::now();

        karatsuba_time += (time_end - time_start).count();

        time_start = chrono::steady_clock::now();
        a.naiveMultiply(b);
        time_end = chrono::steady_clock::now();

        naive_time += (time_end - time_start).count();
    }
    cout << setw(14) << left << "Time"        << setw(14) << "Overall"      << "Average"              << '\n';
    cout << setw(14) << left << "Karatsuba's" << setw(14) << karatsuba_time << karatsuba_time / tries << '\n';
    cout << setw(14) << left << "Naive"       << setw(14) << naive_time     << naive_time / tries     << '\n';
    cout << '\n';
    cout << "Karatsuba's " << (double)naive_time / karatsuba_time << " times faster." << '\n';

    return 0;
}
