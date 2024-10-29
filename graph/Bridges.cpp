vector<pair<int, int>> BridgeChan(const vector<vector<int>> &adj)
{
    /*
    - pass adjacency list of adjacency graph with size = number of nodes
    - multi edges are allowed
    - returns list of unique bridge edges [u, v] in the form of pairs [min(u, v), max(u, v)]
    - O(n + m) time 
    */

    int n = adj.size();

    int timer = 0;
    vector<int> tin(n, -1), low(n, -1); 
    vector<pair<int, int>> b;

    auto dfs = [&](int u, int p, auto &&dfs) -> void
    {
        tin[u] = low[u] = ++ timer;
        bool p_skip = false;

        for(int v : adj[u])
        {
            if(v == p and !p_skip)      //multi-edge consideration
            {
                p_skip = true;
                continue;
            }
            
            if(tin[v] != -1)
                low[u] = min(low[u], tin[v]);
            else
            {
                dfs(v, u, dfs);
                low[u] = min(low[u], low[v]);
                if(low[v] > tin[u])
                    b.emplace_back(min(u, v), max(u, v));
            }
        }
    };
    for(int u = 0; u < n; u ++)
        if(tin[u] == -1)
            dfs(u, -1, dfs);
    
    return b;
}