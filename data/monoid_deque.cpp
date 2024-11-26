template<template<typename> typename S, typename T>
class monoid_deque_chan
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
        bool f = false;

        if(r.empty())
            f = true, l.swap(r);
        
        int sz = r.size() / 2;
        while(sz -- )
            t.push(r.top()), r.pop();
        while(!r.empty())
            l.push(r.top()), r.pop();
        while(!t.empty())
            r.push(t.top()), t.pop();
        
        if(f)
            l.swap(r);
    }

    void push_front(T x)
    {
        l.push(x);
    }
    void push_back(T x)
    {
        r.push(x);
    }

    void pop_front()
    {
        assert(!empty());
        if(l.empty())
            rebalance();
        l.pop();
    }
    void pop_back()
    {
        assert(!empty());
        if(r.empty())
            rebalance();
        r.pop();
    }

    T sum()
    {
        return l.sum().unite(r.sum());
    }

    T front()
    {
        assert(!empty());
        if(l.empty())   
            rebalance();
        return l.top();
    }
    
    T back()
    {
        assert(!empty());
        if(r.empty())
            return l.top(0);
        return r.top();
    }

    void swap(monoid_deque_chan &other)
    {
        l.swap(other.l);
        r.swap(other.r);
        t.swap(other.t);
    }
};