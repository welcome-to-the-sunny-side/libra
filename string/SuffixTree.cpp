class SuffixTree
{
public:
    int n, m, root;
    vector<int> dep;
    vector<vector<int>> adj;

    SuffixTree(int n, vector<int> sa, vector<int> lcp) : 
    n(n), m(n - 1), dep(2 * n + 1), adj(2 * n + 1)
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
                // assert(!(dep[sa[i]] == d and dep[sa[i + 1]] == d));
                int u;
                if(dep[sa[i]] == d)
                    u = sa[i], adj[u].push_back(sa[i + 1]);
                else if(dep[sa[i + 1]] == d)
                    u = sa[i + 1], adj[u].push_back(sa[i]);
                else
                    u = ++ m, adj[u] = {sa[i], sa[i + 1]};

                dep[u] = d;
                l[r[i + 1]] = l[i], r[l[i]] = r[i + 1];
                sa[l[i]] = sa[r[i + 1]] = u;
            }

        root = sa[0];
        if(dep[root] != 0)
        {
            root = ++ m;
            dep[root] = 0, adj[root] = {sa[0]};
        }
    };
};