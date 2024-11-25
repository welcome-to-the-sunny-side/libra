template<template<typename> typename S, typename T>
class MonoidQueueChan
{
public:
    S<T> l, r;

    int Size()
    {
        return l.Size() + r.Size();
    }

    bool Empty()
    {
        return (l.Empty() and r.Empty());
    }

    void Rebalance()
    {
        while(!r.Empty())
            l.Push(r.Top()), r.Pop();
    }

    void Push(T x)
    {
        r.Push(x);
    }

    void Pop()
    {
        assert(!Empty());
        if(l.Empty())   Rebalance();
        l.Pop();
    }

    T Sum()
    {
        return l.Sum().Unite(r.Sum());
    }

    T Front()
    {
        assert(!Empty());
        if(l.Empty())   Rebalance();
        return l.Top();
    }
    
    T Back()
    {
        assert(!Empty());
        if(r.Empty())
            return l.Top(0);
        return r.Top();
    }
};