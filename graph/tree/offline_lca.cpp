vector<int> offline_lca_chan(int n, int r, const vector<vector<int>> &adj, vector<pair<int, int>> query)
{
    //dsu
    vector<int> par(n), siz(n, 1);
    iota(par.begin(), par.end(), 0);
    auto get = [&](int u, auto &&get) -> int
    {
        return (par[u] == u ? u : par[u] = get(par[u], get));
    };
    auto unite = [&](int u, int v) -> void
    {
        u = get(u, get), v = get(v, get);
        if(u == v)
            return;
        if(siz[u] < siz[v])
            swap(u, v);
        par[v] = u, siz[u] += siz[v];
    };

    assert(!query.empty());
    int m = query.size();
    
    vector<int> ans(m);
    vector<bool> see(m);
    vector<vector<int>> store(n);

    for(int i = 0; i < m; i ++)
    {
        auto [u, v] = query[i];
        store[u].push_back(i), store[v].push_back(i);
    }

    auto dfs = [&](int u, int p, auto &&dfs) -> void
    {
        for(auto i : store[u])
        {
            if(see[i])
                ans[i] = get(query[i].first == u ? query[i].second : query[i].first, get);
            see[i] = true;
        }

        for(auto v : adj[u])
            if(v != p)
                dfs(v, u, dfs);

        if(p != 0)
            unite(u, p);
    };
    dfs(r, 0, dfs);
    
    return ans;
};