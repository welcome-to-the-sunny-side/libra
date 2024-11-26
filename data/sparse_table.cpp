template <typename T>
class sparse_table_chan
{
public:
    int n;
    vector<vector<T>> mat;
    sparse_table_chan(const vector<T> &a)
    {
        n = a.size();
        int max_log = 32 - __builtin_clz(n);
        mat.resize(max_log);
        mat[0] = a;
        for (int j = 1; j < max_log; j++)
        {
            mat[j].resize(n - (1 << j) + 1);
            for (int i = 0; i <= n - (1 << j); i++)
                mat[j][i] = mat[j - 1][i].Unite(mat[j - 1][i + (1 << (j - 1))]);
        }
    }
    T query(int from, int to) const
    {
        assert(0 <= from && from <= to && to <= n - 1);
        int lg = 32 - __builtin_clz(to - from + 1) - 1;
        return mat[lg][from].Unite(mat[lg][to - (1 << lg) + 1]);
    }
};