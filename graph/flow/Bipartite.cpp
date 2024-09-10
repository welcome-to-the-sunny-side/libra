struct Bipartite
{
    /*
        tc: O(E sqrt(V))
        mc: O(V + E)

        vars:
            n, m: size of left and right partitions
            adj: left to right edges ONLY
        info:
            Both partitions are individually 1 indexed
            After calling MaxMatching():
                - matching = size of max matching
                - l[u] = matched right node for u (l[u] = -1 => unmatched)
                - r[u] = matched left node for u (r[u] = -1 => unmatched)
            MVC is found by:
                - orienting matched edges from right to left, unmatched opposite
                - running dfs from all unmatched left side nodes
                - (unvis nodes on the left) + (visited nodes on right) = mvc
            MIS is complement of MVC
    */
    int n, m;
    int matching = 0;
    vector<vector<int>> adj;
    vector<int> l, r, lvl;
    Bipartite(int _n, int _m, const vector<vector<int>> &_adj) : 
        n(_n), m(_m), l(n + 1, -1), r(m + 1, -1), adj(_adj) {};

    // void Add(int u, int v)  { adj[u].push_back(v); }
    bool Dfs(int u)
    {
        int t = exchange(lvl[u], -1) + 1;
        for (int v : adj[u])
            if (r[v] == -1 or (lvl[r[v]] == t and Dfs(r[v])))
                return l[u] = v, r[v] = u, 1;
        return 0;
    }

    int MaxMatching()
    {
        matching = 0;   vector<int> q(n + 1);
        for (int s = 0, t = 0;; s = t = 0)
        {
            lvl = vector<int>(n + 1);   bool f = 0;
            for(int i = 1; i <= n; i ++) if (l[i] == -1) lvl[i] = 1, q[t ++] = i;
            while (s < t)
            {
                int u = q[ s++];
                for (int v : adj[u])
                {
                    int x = r[v];
                    if (x == -1)
                        f = 1;
                    else if (!lvl[x])
                        lvl[x] = lvl[u] + 1, q[t++] = x;
                }
            }
            if (!f)
                break;
            for(int i = 1; i <= n; i ++) if (l[i] == -1) matching += Dfs(i);
        }
        return matching;
    }

    vector<pair<int, int>> MaxMatchingEdges()
    {
        vector<pair<int, int>> mme;
        for(int u = 1; u <= n; u ++)
            if(l[u] != -1)
                mme.push_back(make_pair(u, l[u]));
        return mme;
    }
    
    pair<vector<int>, vector<int>> MinVertexCover()
    {
        vector<bool> lv(n + 1, false), rv(m + 1, false);
        auto dfs = [&](int u, auto &&dfs) -> void
        {
            lv[u] = true;
            for(auto v : adj[u])
                if(r[v] != u and !rv[v])
                {
                    rv[v] = true;
                    if(r[v] != -1 and !lv[r[v]])
                        dfs(r[v], dfs);
                }
        };
        for(int u = 1; u <= n; u ++)
            if(l[u] == -1 and !lv[u])
                dfs(u, dfs);

        vector<int> lc, rc;
        for(int u = 1; u <= n; u ++)
            if(!lv[u])
                lc.push_back(u);
        for(int u = 1; u <= m; u ++)
            if(rv[u])
                rc.push_back(u);
        assert(matching == (int)lc.size() + (int)rc.size());
        return {lc, rc};
    }
};