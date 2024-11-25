template<template<typename> typename S, typename T>
class MonoidDequeChan
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
        bool f = false;

        if(r.Empty())
            f = true, l.Swap(r);
        
        int sz = r.Size() / 2;
        while(sz -- )
            t.Push(r.Top()), r.Pop();
        while(!r.Empty())
            l.Push(r.Top()), r.Pop();
        while(!t.Empty())
            r.Push(t.Top()), t.Pop();
        
        if(f)
            l.Swap(r);
    }

    void PushFront(T x)
    {
        l.Push(x);
    }
    void PushBack(T x)
    {
        r.Push(x);
    }

    void PopFront()
    {
        assert(!Empty());
        if(l.Empty())
            Rebalance();
        l.Pop();
    }

    void PopBack()
    {
        assert(!Empty());
        if(r.Empty())
            Rebalance();
        r.Pop();
    }

    T Sum()
    {
        return l.Sum().Unite(r.Sum());
    }

    T Front()
    {
        assert(!Empty());
        if(l.Empty())   
            Rebalance();
        return l.Top();
    }
    
    T Back()
    {
        assert(!Empty());
        if(r.Empty())
            return l.Top(0);
        return r.Top();
    }

    void Swap(MonoidDequeueChan &other)
    {
        l.Swap(other.l);
        r.Swap(other.r);
        t.Swap(other.t);
    }
};