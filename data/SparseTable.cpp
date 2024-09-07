struct SpNode
{
    static constexpr int neutral = inf;
    
    int val;
    SpNode() : val(neutral) {};
    SpNode(int x) : val(x) {};

    static SpNode combine(const SpNode &a, const SpNode &b)
    {
        return SpNode(min(a.val, b.val));
    };
};
template <typename T, typename F, typename E>
struct SparseTable
{
    int n;
    vector<vector<T>> mat;
    F func;

    SparseTable(const vector<E> &ad, const F &f) : func(f)
    {
        n = static_cast<int>(ad.size());

        vector<T> a(ad.size());
        for(int i = 0; i < n; i ++) a[i] = T(ad[i]);

        int max_log = 32 - __builtin_clz(n);
        mat.resize(max_log);
        mat[0] = a;
        for (int j = 1; j < max_log; j++)
        {
            mat[j].resize(n - (1 << j) + 1);
            for (int i = 0; i <= n - (1 << j); i++)
                mat[j][i] = func(mat[j - 1][i], mat[j - 1][i + (1 << (j - 1))]);
        }
    }

    T get(int from, int to) const
    {
        assert(0 <= from && from <= to && to <= n - 1);
        int lg = 32 - __builtin_clz(to - from + 1) - 1;
        return func(mat[lg][from], mat[lg][to - (1 << lg) + 1]);
    }
};

//declaration
#define SparseTable(name, vector) \
    SparseTable<SpNode, decltype(&SpNode::combine), decltype(vector)::value_type> name(vector, &SpNode::combine);