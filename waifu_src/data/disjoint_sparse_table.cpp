template <typename T>
class disjoint_sparse_table_chan
{
public:
    int n;
    vector<vector<T>> mat;

    disjoint_sparse_table_chan(const vector<T> &a)
    {
        n = static_cast<int>(a.size());
        mat.push_back(a);
        for (int p = 1; (1 << p) < n; p++)
        {
            mat.emplace_back(n);
            for (int mid = 1 << p; mid < n; mid += 1 << (p + 1))
            {
                mat[p][mid - 1] = a[mid - 1];
                for (int j = mid - 2; j >= mid - (1 << p); j--)
                    mat[p][j] = a[j].unite(mat[p][j + 1]);
                mat[p][mid] = a[mid];
                for (int j = mid + 1; j < min(n, mid + (1 << p)); j++)
                    mat[p][j] = mat[p][j - 1].unite(a[j]);
            }
        }
    }

    T query(int l, int r) const
    {
        assert(0 <= l && l < r && r <= n);
        if (r - l == 1)
            return mat[0][l];
        int p = bit_width(unsigned(l ^ (r - 1))) - 1;
        return mat[p][l].unite(mat[p][r - 1]);
    }
};