struct Condenser
{
    /*
    tc: O(n + m)
    ml: O(n + m)

    info:
        SCC u occurs before SCC v (u < v) in some topological ordering of SCCs 

    warning: 
        1-indexed i/o    
        there may be multiple edges between different SCCs

    var:
        [n -> number of nodes]
        [c -> number of SCCs]
        [comp[u] -> component number of node u]
        [scc[u] -> outedge list for scc u]
        [grp[u] -> node list for scc u]
    */

    int n, c;
    vector<bool> vis;
    vector<int> stak, comp;
    vector<vector<int>> adj, rdj, scc, grp;

    Condenser(int n) :  n(n), c(0), 
                        vis(n + 1, false), 
                        adj(n + 1), rdj(n + 1), scc(n + 1), grp(n + 1),
                        comp(n + 1, -1) {};

    void add_edge(int u, int v)
    {
        adj[u].push_back(v);
        rdj[v].push_back(u);
    }

    void condense()
    {
        auto dfs1 = [&](int u, auto &&dfs1) -> void 
        {
            vis[u] = true;
            for(auto v : adj[u])
                if(!vis[v])
                    dfs1(v, dfs1);
            stak.push_back(u);
        };
        for(int u = 1; u <= n; u ++)
            if(!vis[u])
                dfs1(u, dfs1);

        auto dfs2 = [&](int u, auto &&dfs2) -> void 
        {
            comp[u] = c;
            for(auto v : rdj[u])
                if(comp[v] == -1)
                    dfs2(v, dfs2);
        };

        reverse(stak.begin(), stak.end());
        for(auto u : stak)
            if(comp[u] == -1)
                ++ c, dfs2(u, dfs2);

        for(int u = 1; u <= n; u ++)
            for(auto v : adj[u])
                if(comp[u] != comp[v])
                    scc[comp[u]].push_back(comp[v]);
            
        for(int u = 1; u <= n; u ++)
            grp[comp[u]].push_back(u);
    }

    void fix()      //remove multiple edges [O(m log(m))]
    {
        for(auto &v : scc)
        {
            sort(v.begin(), v.end());
            v.erase(unique(v.begin(), v.end()), v.end());
        }
    }
};