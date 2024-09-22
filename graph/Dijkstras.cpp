template<typename T>
vector<T> Dijkstras(int source, const vector<vector<pair<int, T>>> &adj, T zer, T inf)
{
    int n = adj.size() - 1;
    vector<T> dis(n + 1, inf);
    dis[source] = zer;

    priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>> q;
    for(int u = 1; u <= n; u ++)
        q.emplace(dis[u], u);
    
    while(!q.empty())
    {
        auto [d, u] = q.top();
        q.pop();
        if(d > dis[u])
            continue;
        for(auto [v, w] : adj[u])
            if(d + w < dis[v])
                dis[v] = d + w, q.emplace(dis[v], v); 
    }

    return dis;
}