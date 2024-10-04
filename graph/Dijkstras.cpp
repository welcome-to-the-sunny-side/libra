template<typename T, T zero, T inf>
vector<T> DijkstrasChan(int source, const vector<vector<pair<int, T>>> &adj)
{
    int n = adj.size();
    vector<T> dis(n, inf);
    dis[source] = zero;

    priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>> q;
    q.emplace(dis[source], source);

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