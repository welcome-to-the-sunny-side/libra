class SuffixTree
{
public:
    int n, m;
    vector<int> dep;
    vector<vector<int>> adj;

    SuffixTree(int n, vector<int> sa, vector<int> lcp) : n(n), m(n - 1), dep(2 * n + 1), adj(2 * n + 1) 
    {
        vector<int> l(n), r(n), p(n);
        for(int i = 0; i < n; i ++) 
            l[i] = r[i] = i, p[i] = sa[i], dep[i] = n - i;

        vector<vector<int>> q(n);
        for(int i = 0; i < n - 1; i ++)
            q[lcp[i]].push_back(i);

        for(int d = n - 1; d >= 0; d --)
            for(auto i : q[d])
            {
                int u = ++ m;
                dep[u] = d, adj[u] = {p[i], p[i + 1]};
                l[r[i + 1]] = l[i], r[l[i]] = r[i + 1];
                p[l[i]] = p[r[i + 1]] = u;
            }
    };

    void Populate()
    {
        
    }
};