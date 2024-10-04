class ElementChan
{
public:
    int x;
    ElementChan() : x(0) {};
    ElementChan(int v) : x(v) {};

    bool opt(const ElementChan &o) const
    {
        return x > o.x;
    }
};
template<typename T>
class MonoQChan
{
public:
    T neutral;

    deque<pair<T, int>> q;
    int l, r;

    MonoQChan() : l(0), r(0) {};

    int Size()
    {
        return r - l;
    }
    T Opt()
    {
        return (Size() ? q.front().first : T());
    }
    void Push(T x)
    {
        while(!q.empty() and x.opt(q.back().first))
            q.pop_back();
        q.push_back({x, r ++});
    }
    void Pop()
    {
        if (!q.empty() and q.front().second == l)
            q.pop_front();
        l ++;
    }
    void Clear()
    {
        q.clear();
        l = r = 0;
    }
};
