template<typename T>
class monoid_stack_chan
{
public:
    vector<pair<T, T>> stk;
    int size()
    {
        return stk.size();
    }

    bool empty()
    {
        return stk.empty();
    }

    void push(T x)
    {
        stk.push_back({x, x.unite(sum())});
    }

    void pop()
    {
        assert(!stk.empty());
        stk.pop_back();
    }

    T top()
    {
        assert(!stk.empty());
        return stk.back().first;
    }

    T sum(int i = -1)
    {
        if(i == -1) 
            i = size() - 1;
        if(i == -1)
            return T();
        return stk[i].second;
    }

    void rswap(monoid_stack_chan &other)
    {
        swap(stk, other.stk);
    }
};
