template <typename I, typename T, template<typename, typename> typename S>
struct HeavyLight
{
    /*
        info:
            1 indexed
        vars:
            - I: Info struct of segment tree
            - T: Lazy Tag struct of segment tree
            - S: segment tree class (S must support S<I, T>)
            - range [pos[u], out[u]) represents subtree of u
        warning:
            - handle segtree initialization correctly
            - monoid operations must be commutative here
              (implement directed path queries later)
    */
    int n, r;
    vector<int> par, heavy, dep, root, pos, out;
    S<I, T> tree;

    template<typename... Args>
    HeavyLight(int n, int r, vector<vector<int>> adj, Args&&... args) :
    n(n), r(r), par(n + 1), heavy(n + 1, -1), dep(n + 1), root(n + 1), pos(n + 1), out(n + 1),
    tree(forward<Args>(args)...)
    {
        auto dfs_sz = [&](int u, auto &&dfs) -> int
        {
            int ss = 1, m = 0;
            for(auto &v : adj[u])
                if(v != par[u])
                {
                    par[v] = u, dep[v] = dep[u] + 1;
                    int s = dfs(v, dfs);
                    ss += s;
                    if(s > m)
                        heavy[u] = v, m = s, swap(adj[u][0], v);
                }
            return ss;
        };
        int timer = 1;
        auto dfs_hld = [&](int u, auto &&dfs) -> void
        {
            pos[u] = timer ++;
            for(auto v : adj[u])
                if(v != par[u])
                    root[v] = (heavy[u] == v ? root[u] : v), dfs(v, dfs);
            out[u] = timer;
        };
        par[r] = -1, dep[r] = 0, root[r] = r;
        dfs_sz(r, dfs_sz), dfs_hld(r, dfs_hld);
    };

    int LCA(int u, int v)
    {
        for (; root[u] != root[v]; v = par[root[v]])
            if (dep[root[u]] > dep[root[v]])
                swap(u, v);
        return (dep[u] < dep[v] ? u : v);
    }

    template <typename O>
    void ProcessPath(int u, int v, O op)
    {
        for (; root[u] != root[v]; v = par[root[v]])
        {
            if (dep[root[u]] > dep[root[v]])
                swap(u, v);
            op(pos[root[v]], pos[v]);
        }
        if (dep[u] > dep[v])
            swap(u, v);
        op(pos[u], pos[v]);
    }
    
    void SetVertex(int v, const I &info)
    {
        tree.Set(pos[v], info);
    }
    void ModifyPath(int u, int v, const T &tag)
    {
        ProcessPath(u, v, [this, &tag](int l, int r)  {tree.Modify(l, r, tag); });
    }
    void ModifySubtree(int u, const T&tag)
    {
        tree.Modify(pos[u], out[u] - 1, tag);
    }

    I GetVertex(int v)
    {
        return tree.Get(pos[v]);
    }
    I QueryPath(int u, int v)
    {
        I res = I();
        ProcessPath(u, v, [this, &res](int l, int r)    {res.Join(tree.Query(l, r));});
        return res;
    }
    I QuerySubtree(int u)
    {
        return tree.Query(pos[u], out[u] - 1);
    }
};