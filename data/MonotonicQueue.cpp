template<typename T>
class MonoQ
{
public:
    static constexpr int neutral = 0;

    deque<pair<T, T>> q;
    function<bool(const T&, constT&)> opt;
    int l, r;

    MonoQ(function<bool(const T&, constT&)> cmp) : l(0), r(0), opt(cmp) {};

    int Size()
    {
        return r - l;
    }
    int Opt()
    {
        return (Size() ? q.front().first : neutral);
    }
    void Push(T x)
    {
        while(!q.empty() and opt(x, q.back().first))
            q.pop_back();
        q.push_back({T, r ++});
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