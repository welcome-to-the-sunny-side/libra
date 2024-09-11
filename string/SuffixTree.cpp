class SuffixTree
{
public:
    int n;
    vector<int> sa, lcp, dep, rep;
    vector<vector<int>> adj;

    SuffixTree(int n, vector<int> sa, vector<int> lcp) : n(n), sa(sa), lcp(lcp), dep(2 * n), rep(2 * n), adj(2 * n) 
    {
        assert((int)sa.size() == n and (int)lcp.size() == n - 1);

        vector<int> l(n), r(n), p(n);
        for(int i = 0; i < n; i ++) 
            l[i] = r[i] = p[i] = i, dep[i] = n - i, rep[i] = i;
        
        vector<vector<int>> q(n);
        for(int i = 0; i < n - 1; i ++)
            q[lcp[i]].push_back(i);
        
        for(int d = n - 1, x = n - 1; d >= 0; d --)
            for(auto i : q[d])
            {
                int u = ++ x;
                dep[u] = d;

                int v1 = p[i], v2 = p[i + 1];
                adj[u].push_back(v1), adj[u].push_back(v2);

                l[r[i + 1]] = l[i];
                r[l[i]] = r[i + 1];
                p[l[i]] = p[r[i]] = u;
                rep[u] = rep[v1];
            }
    }
};