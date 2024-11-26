class connectivity_chan
{
public:
    int n;
    vector<vector<int>> adj;
    int timer;
    vector<int> tin, low;

    vector<pair<int, int>> bridge;

    int a;
    vector<bool> cut;

    int m;
    vector<vector<int>> bcc;

    int b;
    vector<int> block_node_id;
    vector<int> cut_node_id;
    vector<vector<int>> bct_comp;    
    vector<vector<int>> bct;

    connectivity_chan(const vector<vector<int>> &adj) : n(adj.size()), m(n), adj(adj), timer(0), tin(n, -1), low(n, -1), cut(n), bcc(n) 
    {
        compute_connectivity();
        make_block_cut_tree();
    };

    void compute_connectivity()
    {
        vector<bool> vis(n);
        vector<int> stk;

        auto dfs = [&](int u, int p, auto &&dfs) -> void
        {
            tin[u] = low[u] = timer ++;
            vis[u] = true;
            stk.push_back(u);

            int ptimes = 0;
            int children = 0;
            for(auto v : adj[u])
            {
                if(v == p)
                {
                    ++ ptimes;
                    continue;
                }

                if(vis[v])
                    low[u] = min(low[u], tin[v]);
                else
                {
                    ++ children;
                    int s = stk.size();
                    dfs(v, u, dfs);
                    low[u] = min(low[u], low[v]);

                    if((p == -1 and children > 1) or (p != -1 and tin[u] <= low[v]))
                    {
                        cut[u] = true;
                        bcc.push_back(vector<int> ());
                        bcc[m].push_back(u);
                        while(stk.size() > s)
                        {
                            bcc[m].push_back(stk.back());
                            stk.pop_back();
                        }
                        ++ m;
                    }
                }
            
            }

            if(p != -1 and ptimes <= 1 and low[u] > tin[p])
                bridge.emplace_back(min(u, p), max(u, p));
        };
        for(int u = 0; u < n; u ++)
            if(!vis[u])
            {
                dfs(u, -1, dfs);
                bcc.push_back(vector<int> ());
                while(!stk.empty())
                {
                    bcc[m].push_back(stk.back());
                    stk.pop_back();
                }
                ++ m;
            }
        a = count(cut.begin(), cut.end(), true);
    }

    void make_block_cut_tree()
    {
        b = (m - n) + a;
        bct_comp.assign(b, vector<int>());

        for(int i = n; i < m; i ++)
        {
            bct_comp[i - n] = bcc[i];
            for(auto u : bct_comp[i - n])
                block_node_id[u] = i - n;
        }

        vector<int> cut_list;
        for(int i = 0; i < n; i ++)
            if(cut[i])
                cut_list.push_back(i);
            
        for(int i = m - n; i < m - n + a; i ++)
        {
            bct_comp[i] = {cut_list[i - (m - n)]};
            for(auto u : bct_comp[i])
                cut_node_id[u] = i;
        }

        for(int i = 0; i < m - n; i ++)
            for(auto u : bct_comp[i - n])
                if(cut[u])
                    bct[i].push_back(cut_node_id[u]), bct[cut_node_id[u]].push_back(i);
    }
};