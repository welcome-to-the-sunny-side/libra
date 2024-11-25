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
        stk.push_back({x, x.Unite(Sum())});
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
        if(i == -1)
            return T();
        return stk[i].second;
    }

    void Swap(MonoidStackChan &other)
    {
        swap(stk, other.stk);
    }
};
