template<template<typename> typename S, typename T>
class monoid_queue_chan
{
public:
    S<T> l, r;

    int size()
    {
        return l.size() + r.size();
    }

    bool empty()
    {
        return (l.empty() and r.empty());
    }

    void rebalance()
    {
        while(!r.empty())
            l.push(r.top()), r.pop();
    }

    void push(T x)
    {
        r.push(x);
    }

    void pop()
    {
        assert(!empty());
        if(l.empty())   rebalance();
        l.pop();
    }

    T sum()
    {
        return l.sum().unite(r.sum());
    }

    T front()
    {
        assert(!empty());
        if(l.empty())   rebalance();
        return l.top();
    }
    
    T back()
    {
        assert(!empty());
        if(r.empty())
            return l.top(0);
        return r.top();
    }
};