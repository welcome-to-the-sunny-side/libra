struct BinaryLifter
{
    /*
    tc: O(n logn) preprocessing, O(logn) query
    ml: O(n logn)

    info:
        1 indexed
        has virtual root node 0

    var:
        [n -> number of nodes]
        [dep[u] -> depth of node u]
        [tin[u], tout[u] -> intime, outtime of node u]
        [up[u][i] -> stores 2^ith ancestor of u]
    */

    int n, L, timer;

    vector<int> dep;
    vector<int> tin, tout;
    vector<vector<int>> up;

    void init(int x)
    {
        n = x;
        L = ceil(log2(n)) + 1;
        timer = 0;
        dep.assign(n + 1, 0);
        tin.assign(n + 1, 0);
        tout.assign(n + 1, 0);
        up.assign(n + 1, vector<int>(L));
    }

    void dfs(int u, int p, vector<vector<int>> &adj)
    {
        tin[u] = ++ timer;
        up[u][0] = p;
 
        for(int i = 1; i < L; ++i)
            up[u][i] = up[up[u][i - 1]][i - 1];
 
        for(auto v : adj[u])
            if (v != p)
                dep[v] = dep[u] + 1, dfs(v, u, adj);

        tout[u] = ++ timer;
    }

    void preprocess(int root, vector<vector<int>> &adj) 
    {
        tin[0] = 0, tout[0] = 1e9;
        timer = 0;
        dep[1] = 1;
        dfs(root, root, adj);
    }

    int get_kth_ancestor(int v, int k)
    {
        if(k != 0)
            for(int i = L - 1; i >= 0 and v > 0; i --)
                if((1LL << i) <= k)
                    k -= (1LL << i), v = up[v][i];
        return v;
    }

    bool is_ancestor(int anc, int v)
    {
        return tin[anc] <= tin[v] && tout[v] <= tout[anc];
    }

    int lca(int u, int v)
    {
        if (is_ancestor(u, v))
            return u;
        if (is_ancestor(v, u))
            return v;
 
        for (int i = L - 1; i >= 0; --i) 
            if (!is_ancestor(up[u][i], v))
                u = up[u][i];
 
        return up[u][0];
    }
};
