class SuffixTree
{
public:
    int n, m;
    vector<int> dep;
    vector<vector<int>> adj;

    SuffixTree(int n, vector<int> sa, vector<int> lcp) : 
    n(n), m(n - 1), dep(2 * n), adj(2 * n) 
    {
        vector<int> l(n), r(n);
        for(int i = 0; i < n; i ++) 
            l[i] = r[i] = i, dep[i] = n - i;
        
        vector<vector<int>> q(n);
        for(int i = 0; i < n - 1; i ++)
            q[lcp[i]].push_back(i);

        for(int d = n - 1; d >= 0; d --)
            for(auto i : q[d])
            {
                int u = ++ m;
                dep[u] = d, adj[u] = {sa[i], sa[i + 1]};
                l[r[i + 1]] = l[i], r[l[i]] = r[i + 1];
                sa[l[i]] = sa[r[i + 1]] = u;
            }

        vector<bool> rem(m + 1, false);
        for(int u = m; u >= n; u --)
            if(!rem[u])
            {
                vector<int> c = {u};
                auto dfs = [&](int u, auto &&dfs) -> void
                {
                    for(auto v : adj[u])
                        if(!rem[v] and dep[u] == dep[v])
                            rem[v] = true, c.push_back(v), dfs(v, dfs);
                };
                dfs(u, dfs);
                adj[u].clear();
                for(auto x : c)
                    for(auto v : adj[x])
                        if(!rem[v])
                            adj[u].push_back(v);
            }
    };
};