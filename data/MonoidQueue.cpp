#include<bits/stdc++.h>
using namespace std;

#ifdef natural_selection
#include "../misc/dbg.h"
#else
#define debug(...)
#define endl "\n"
#endif

class MonoidChan
{
public:
    int sum = 0;

    MonoidChan() : sum(0) {};
    MonoidChan(int x) : sum(x) {};

    MonoidChan Unite(MonoidChan b) const 
    {
        MonoidChan res(sum + b.sum);
        return res;
    }
    static MonoidChan GetDefault([[maybe_unused]] int l, [[maybe_unused]] int r)
    {
        return MonoidChan();
    }
};

template<typename T>
class MonoidStackChan
{
public:
    vector<pair<T, T>> stk;
    int Size()
    {
        return stk.size();
    }

    void Push(T x)
    {
        stk.push_back({x, x.Unite(stk.empty() ? T() : stk.back().second)});
    }

    void Pop()
    {
        assert(!stk.empty());
        stk.pop_back();
    }

    T Top()
    {
        assert(!stk.empty());
        return stk.back().first;
    }

    T Sum(int i = -1)
    {
        if(i == -1) 
            i = Size() - 1;
        assert(0 <= i and i <= Size() - 1);
        return stk[i].second;
    }

    void Swap(MonoidStackChan &other)
    {
        swap(stk, other.stk);
    }
};

template<template<typename T> typename S>
class MonoidQueueChan
{
public:
    S<T> l, r;

    void Push(T x)
    {
        r.Push();
    }

    void Pop()
    {

    }
};

signed main()
{
    ios_base::sync_with_stdio(false), cin.tie(NULL);
    int t = 1;
    cin >> t;
    while(t --)
    {
        
    }
}