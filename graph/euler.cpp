/*
- 0-indexed
- pass edge list with all node indices in [0, n)
- given graph is CONNECTED
- returns {euler path by node index list, euler path by edge index list}
- returns empty if path doesn't exist
*/
array<vector<int>, 2> euler_path(int n, vector<array<int, 2>> edge)
{
    array<vector<int>, 2> euler;

    vector<int> deg(n);
    vector<vector<array<int, 2>>> adj(n);
    int m = edge.size();
    vector<int> seen(m, false);
    for(int i = 0; i < m; i ++)
    {
        auto [u, v] = edge[i];
        ++ deg[u], ++ deg[v];
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
    }

    vector<int> ep;
    for(int u = 0; u < n; u ++)
        if(deg[u] % 2)
            ep.push_back(u);

    if(!ep.empty() and ep.size() != 2)
        return array<vector<int>, 2> {};

    int st = (!ep.empty() ? ep.front() : 0);
    
    auto dfs = [&](int u, auto &&dfs) -> void
    {
        while(!adj[u].empty())
        {
            auto [v, i] = adj[u].back();
            adj[u].pop_back();

            if(seen[i])
                continue;

            seen[i] = true;
            dfs(v, dfs);
            euler[1].push_back(i);
        }

        euler[0].push_back(u);
    };
    dfs(st, dfs);

    if(euler[1].size() != m)
        return array<vector<int>, 2> {};

    return euler;
}