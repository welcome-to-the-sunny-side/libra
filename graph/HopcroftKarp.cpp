// source : https://judge.yosupo.jp/submission/52112

struct BipartiteFlow 
{
    //1 indexed, runs in O(E.sqrt(V))

    int n_left, n_right, flow = 0;
    vector<vector<int>> g;
    vector<int> match_from_left, match_from_right;

    //_n_left = number of nodes on left partition, _n_right = number of nodes on right partition
    bipartite_matching(int _n_left, int _n_right)
        : n_left(_n_left),
          n_right(_n_right),
          g(_n_left + 1),
          match_from_left(_n_left + 1, -1),
          match_from_right(_n_right + 1, -1),
          dist(_n_left + 1) {}

    //add left to right edges
    void add(int u, int v) { g[u].push_back(v); }

    vector<int> dist;

    void bfs() 
    {
        queue<int> q;
        for (int u = 1; u <= n_left; ++u) 
        {
            if (!~match_from_left[u])
                q.push(u), dist[u] = 0;
            else
                dist[u] = -1;
        }
        while (!q.empty()) 
        {
            int u = q.front();
            q.pop();
            for (auto v : g[u])
                if (~match_from_right[v] && !~dist[match_from_right[v]]) 
                {
                    dist[match_from_right[v]] = dist[u] + 1;
                    q.push(match_from_right[v]);
                }
        }
    }

    bool dfs(int u) 
    {
        for (auto v : g[u])
            if (!~match_from_right[v]) 
            {
                match_from_left[u] = v, match_from_right[v] = u;
                return true;
            }
        for (auto v : g[u])
            if (dist[match_from_right[v]] == dist[u] + 1 and dfs(match_from_right[v])) 
            {
                match_from_left[u] = v, match_from_right[v] = u;
                return true;
            }
        return false;
    }

    //returns size of maximum matching
    int get_max_matching() 
    {
        while (true) 
        {
            bfs();
            int augment = 0;
            for (int u = 1; u <= n_left; ++u)
                if (!~match_from_left[u]) augment += dfs(u);
            if (!augment) break;
            flow += augment;
        }
        return flow;
    }

    //returns nodes in a minimum vertex cover, nodes from first part in L, second in R
    //mis is complement of mvc
    pair<vector<int>, vector<int>> minimum_vertex_cover() 
    {
        vector<int> L, R;
        for (int u = 1; u <= n_left; ++u) 
        {
            if (!~dist[u])
                L.push_back(u);
            else if (~match_from_left[u])
                R.push_back(match_from_left[u]);
        }
        return {L, R};
    }

    //returns list of 1-indexed edges in maximum matching, call only after having called get_max_matching()
    vector<pair<int, int>> get_edges() 
    {
        vector<pair<int, int>> ans;
        for (int u = 1; u <= n_left; ++u)
            if (match_from_left[u] != -1)
                ans.emplace_back(u, match_from_left[u]);
        return ans;
    }
};