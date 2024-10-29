#include<bits/stdc++.h>
using namespace std;

#ifdef natural_selection
#include "../misc/dbg.h"
#else
#define debug(...)
#define endl "\n"
#endif

vector<int> ArticulationChan(const vector<vector<int>> &adj)
{
    int n = adj.size();

    int timer = 0;
    vector<int> tin(n, -1), low(n, -1);
    vector<int> a;

    auto dfs = [&](int u, int p, auto &&dfs) -> void
    {
        tin[u] = low[u] = ++ timer;

        int c = 0;
        bool cut = false;

        for(int v : adj[u])
        {
            if(v == p)
                continue;
            if(tin[v] != -1)
                low[u] = min(low[u], tin[v]);
            else
            {
                dfs(v, u, dfs);
                low[u] = min(low[u], low[v]);
                if(low[v] >= tin[u] and p != -1)
                    cut = true;
                ++ c;
            }
        }
        if(p == -1 and c > 1)
            cut = true;
        
        if(cut)
            a.push_back(u);
    };
    for(int u = 0; u < n; u ++)
        if(tin[u] == -1)
            dfs(u, -1, dfs);
        
    return a;
}

vector<vector<int>> BiconChan(const vector<vector<int>> &adj)
{
    int n = adj.size();

    auto articulation = ArticulationChan(adj);

    vector<bool> is_articulation(n, false);
    for(auto u : articulation)
        is_articulation[u] = true;
    
    vector<bool> vis(n, false);
    auto dfs = [&](int u, vector<int> &comp, auto &&dfs) -> void
    {
        vis[u] = true;
        comp.push_back(u);
        
        if(!is_articulation[u])
            for(auto v : adj[u])
                if(!vis[v] or is_articulation[v])
                    dfs(v, comp, dfs);
    };

    vector<vector<int>> bcc;
    for(int u = 0; u < n; u ++)
    {
        if(is_articulation[u])
        {
            for(auto v : adj[u])
                if(is_articulation[v] and u < v)
                    bcc.push_back({u, v});
        }
        else if(!vis[u])
        {
            vector<int> comp;
            dfs(u, comp, dfs);
            sort(comp.begin(), comp.end());
            comp.erase(unique(comp.begin(), comp.end()), comp.end());
        }
    }
    
    return bcc;
}

int32_t main()
{
    ios_base::sync_with_stdio(false), cin.tie(NULL);
    int t = 1;
    // cin >> t;
    while(t --)
    {
        int n, m;
        cin >> n >> m;

        vector<vector<int>> adj(n);
        for(int i = 0; i < m; i ++)
        {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v), adj[v].push_back(u);
        }

        auto bicon = BiconChan(adj);

        cout << bicon.size() << endl;
        for(auto b : bicon)
        {
            cout << b.size() << " ";
            for(auto u : b)
                cout << u << " ";
            cout << endl;
        }
        cout << endl;
    }
}