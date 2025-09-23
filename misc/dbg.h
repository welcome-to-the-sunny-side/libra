#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <string>
#include <vector>
#include <bitset>

void PPRINT(int x) { std::cerr << x; }
void PPRINT(long x) { std::cerr << x; }
void PPRINT(long long x) { std::cerr << x; }
void PPRINT(unsigned x) { std::cerr << x; }
void PPRINT(unsigned long x) { std::cerr << x; }
void PPRINT(unsigned long long x) { std::cerr << x; }
void PPRINT(float x) { std::cerr << x; }
void PPRINT(double x) { std::cerr << x; }
void PPRINT(long double x) { std::cerr << x; }
void PPRINT(char x) { std::cerr << '\'' << x << '\''; }
void PPRINT(const char *x) { std::cerr << '\"' << x << '\"'; }
void PPRINT(const std::string &x) { std::cerr << '\"' << x << '\"'; }
void PPRINT(const bool& x) { std::cerr << (x ? "true" : "false"); }

template<typename T, typename V>
void PPRINT(const std::pair<T, V> &x) {
    std::cerr << '{';
    PPRINT(x.first);
    std::cerr << ',';
    PPRINT(x.second);
    std::cerr << '}';
}

template<typename T>
void PPRINT(const T &x) {
    int f = 0;
    std::cerr << '{';
    for (auto &i : x) {
        std::cerr << (f++ ? "," : "");
        PPRINT(i);
    }
    std::cerr << "}";
}

void PPRINT(const std::vector<bool> &x) {
    int f = 0;
    std::cerr << '{';
    for (size_t i = 0; i < x.size(); ++i) {
        std::cerr << (f++ ? "," : "");
        PPRINT(static_cast<bool>(x[i])); // <-- cast the proxy to bool
    }
    std::cerr << "}";
}

template<size_t N>
void PPRINT(const std::bitset<N>& x) { std::cerr << x; }

void _print() {
    std::cerr << "]\n";
}

template <typename T, typename... V>
void _print(T t, V... v) {
    PPRINT(t);
    if (sizeof...(v)) std::cerr << ", ";
    _print(v...);
}

#define debug(x...) { std::cerr << "[" << #x << "] = ["; _print(x); }

#endif // DEBUG_H
