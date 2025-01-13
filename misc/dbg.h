#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <string>
#include <vector>
#include <bitset>

void __print(int x) { std::cerr << x; }
void __print(long x) { std::cerr << x; }
void __print(long long x) { std::cerr << x; }
void __print(unsigned x) { std::cerr << x; }
void __print(unsigned long x) { std::cerr << x; }
void __print(unsigned long long x) { std::cerr << x; }
void __print(float x) { std::cerr << x; }
void __print(double x) { std::cerr << x; }
void __print(long double x) { std::cerr << x; }
void __print(char x) { std::cerr << '\'' << x << '\''; }
void __print(const char *x) { std::cerr << '\"' << x << '\"'; }
void __print(const std::string &x) { std::cerr << '\"' << x << '\"'; }
void __print(const bool& x) { std::cerr << (x ? "true" : "false"); }

template<typename T, typename V>
void __print(const std::pair<T, V> &x) {
    std::cerr << '{';
    __print(x.first);
    std::cerr << ',';
    __print(x.second);
    std::cerr << '}';
}

template<typename T>
void __print(const T &x) {
    int f = 0;
    std::cerr << '{';
    for (auto &i : x) {
        std::cerr << (f++ ? "," : "");
        __print(i);
    }
    std::cerr << "}";
}

void __print(const std::vector<bool> &x) {
    int f = 0;
    std::cerr << '{';
    for (size_t i = 0; i < x.size(); ++i) {
        std::cerr << (f++ ? "," : "");
        __print(x[i]);
    }
    std::cerr << "}";
}

template<size_t N>
void __print(const std::bitset<N>& x) { std::cerr << x; }

void _print() {
    std::cerr << "]\n";
}

template <typename T, typename... V>
void _print(T t, V... v) {
    __print(t);
    if (sizeof...(v)) std::cerr << ", ";
    _print(v...);
}

#define debug(x...) { std::cerr << "[" << #x << "] = ["; _print(x); }

#endif // DEBUG_H
