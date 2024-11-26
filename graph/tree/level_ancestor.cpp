//wip
class level_ancestor_chan
{
public:
    int n;
    vector<int> tin;
    vector<int> tout;
    vector<int> heavy;

    level_ancestor_chan(int n, int r, const vector<vector<int>> &tree) : 
    n(n), heavy(n + 1), tin(n + 1), tout(n + 1)
    {
        auto adj = tree;
        vector<int> sz(n + 1, 0);
        auto dfs1 = [&](int u, int p, auto &&dfs) -> void
        {
            sz[u] = 1;
            if(p != 0)
                adj[u].erase(find(adj[u].begin(), adj[u].end(), p));
            for(auto v : adj[u])
                dfs(v, u, dfs), sz[u] += sz[v];
            if(!adj[u].empty())
            {
                auto mit = *max_element(adj[u].begin(), adj[u].end(), [&](int v1, int v2) {return sz[v1] < sz[v2];});
                swap(*adj[u].begin(), mit);
            }
        };
        dfs1(r, 0, dfs1);

        int timer = 0;
        auto dfs2 = [&](int u, int p, auto &&dfs) -> void
        {
            tin[]
        };
    };

    bool is_anc(int a, int u)
    {
        return tin[a] <= tin[u] and tout[u] <= tout[a];
    }

    int lca(int u, int v)
    {

    }

    int get_ancestor(int u, int k)
    {

    }
};