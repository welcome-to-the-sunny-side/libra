class binary_lifter_chan
{
    /*
    tc: O(n logn) preprocessing, O(logn) query
    ml: O(n logn)

    info:
        1 indexed
        n -> number of nodes
        dep[u] -> depth of node u
        tin[u], tout[u] -> intime, outtime of node u
        up[u][i] -> stores 2^ith ancestor of u
    */
public:
    int n, L, timer;
    vector<int> dep, tin, tout;
    vector<vector<int>> up;

    binary_lifter_chan(int n, int r, const vector<vector<int>> &adj) : 
    n(n), L(ceil(log2(n)) + 1), timer(0), dep(n), tin(n), tout(n), up(n, vector<int> (L, r))
    {
        timer = 0;
        dep[r] = 0;
        dfs(r, r, adj);
    }

    void dfs(int u, int p, const vector<vector<int>> &adj)
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

    int get_kth(int v, int k)
    {
        if(k != 0)
            for(int i = L - 1; i >= 0 and v > 0; i --)
                if((1 << i) <= k)
                    k -= (1 << i), v = up[v][i];
        return v;
    }

    bool is_anc(int anc, int v)
    {
        return tin[anc] <= tin[v] and tout[v] <= tout[anc];
    }

    int lca(int u, int v)
    {
        if (is_anc(u, v))
            return u;
        if (is_anc(v, u))
            return v;
        for (int i = L - 1; i >= 0; --i) 
            if (!is_anc(up[u][i], v))
                u = up[u][i];
        return up[u][0];
    }
};
