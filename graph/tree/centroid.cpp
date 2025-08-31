
class centroid_decomposition_chan
{
    /*
        info:
            - 0-indexed
            - roots the tree at `root`
            - finally, par and cadj define the centroid tree
            - parent of root is -1
    */
public:
    int n;
    int root;

    vector<int> par;
    vector<vector<int>> cadj;

    centroid_decomposition_chan(int n, const vector<vector<int>> &adj) : n(n), par(n, -1), cadj(n) 
    {
        vector<bool> mark(n, false);
        vector<int> sub(n);
        auto find_centroid = [&](int u) -> int
        {
            assert(!mark[u]);
            int cnt = 0;
            vector<int> seen;
            auto dfs = [&](int u, int p, auto &&dfs) -> void
            {
                ++ cnt;
                sub[u] = 1;
                for(auto v : adj[u])
                    if(v != p and !mark[v])
                        dfs(v, u, dfs), sub[u] += sub[v];
            };
            dfs(u, 0, dfs);

            int cen = -1;
            auto dfs2 = [&](int u, int p, auto &&dfs2) -> void
            {
                int mx = cnt - sub[u];
                for(auto v : adj[u])
                    if(v != p and !mark[v])
                    {
                        dfs2(v, u, dfs2);
                        mx = max(mx, sub[v]);
                    }
                if(mx <= cnt/2)
                    cen = u;
            };
            dfs2(u, 0, dfs2);

            assert(cen != -1);
            return cen;
        };

        auto dfs = [&](int u, auto &&dfs) -> void
        {
            for(auto v : adj[u])
                if(!mark[v])
                {
                    int c = find_centroid(v);
                    cadj[u].push_back(c);
                    par[c] = u;
                    mark[c] = true;
                    dfs(c, dfs);
                }
        };
        root = find_centroid(0);
        mark[root] = true;
        dfs(root, dfs);
    };
};
