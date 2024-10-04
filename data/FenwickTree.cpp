class NodeChan
{
public:
    int v = 0;

    inline void operator+=(NodeChan &other)
    {
        v += other.v;
    }
    inline bool operator<(NodeChan &other)
    {
        return v < other.v;
    }
};
template <typename T>
class FenwickChan
{
public:
    vector<T> fenw;
    int n;
    int pw;

    FenwickChan() : n(0) {}
    FenwickChan(int n_) : n(n_)
    {
        fenw.resize(n);
        pw = (n == 0 ? 0 : 1ULL << (63 - __builtin_clzll(unsigned(n))));
    }

    // a[x] += v;
    void Modify(int x, T v)
    {
        assert(0 <= x && x < n);
        while (x < n)
        {
            fenw[x] += v;
            x |= x + 1;
        }
    }

    /// sum of prefix [0, .. x] 
    T Query(int x)
    {
        ++ x;
        assert(0 <= x && x <= n);
        T v{};
        while (x > 0)
        {
            v += fenw[x - 1];
            x &= x - 1;
        }
        return v;
    }

    // Returns the length of the longest prefix (0 indexed) with sum <= c
    int MaxPrefix(T c)
    {
        T v{};
        int at = 0;
        for (int len = pw; len > 0; len >>= 1)
        {
            if (at + len <= n)
            {
                auto nv = v;
                nv += fenw[at + len - 1];
                if (!(c < nv))
                {
                    v = nv;
                    at += len;
                }
            }
        }
        assert(0 <= at && at <= n);
        return at;
    }
};