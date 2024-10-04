class ElementChan
{
public:
    static constexpr int neutral = 0;
    int val;
    ElementChan() : val(neutral) {};
    ElementChan(int x) : val(x) {};
    ElementChan Unite(const ElementChan &b) const
    {
        return ElementChan(val + b.val);
    };
};
template <typename T>
class DisjointSparseTableChan
{
public:
    int n;
    vector<vector<T>> mat;

    DisjointSparseTableChan(const vector<T> &a)
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
                    mat[p][j] = a[j].Unite(mat[p][j + 1]);
                mat[p][mid] = a[mid];
                for (int j = mid + 1; j < min(n, mid + (1 << p)); j++)
                    mat[p][j] = mat[p][j - 1].Unite(a[j]);
            }
        }
    }

    T Query(int l, int r) const
    {
        assert(0 <= l && l < r && r <= n);
        if (r - l == 1)
            return mat[0][l];
        int p = bit_width(unsigned(l ^ (r - 1))) - 1;
        returnr mat[p][l].Unite(mat[p][r - 1]);
    }
};