template <typename I, typename T, template<typename, typename> typename S, const bool on_edge>
class HeavyLightChan
{
    /*
        info:
            - 0 indexed
            - range [pos[u], out[u]) represents subtree of u
        vars:
            - I: Info struct of segment tree
            - T: Lazy Tag struct of segment tree
            - S: segment tree class (S must support S<I, T>)
            - r = tree root
            - on_edge = true => values on edges. Internally, value of edge is stored at lower node
        warning:
            - handle segtree initialization correctly
            - monoid operations must be commutative here
                (implement directed path queries later)
    */
public:
    int n, r;
    vector<int> par, heavy, dep, root, pos, out;
    S<I, T> tree;

    HeavyLightChan(int n, int r, vector<vector<int>> adj) :
    n(n), r(r), par(n, -1), heavy(n, -1), dep(n), root(n), pos(n), out(n),
    tree(n + 1)
    {
        assert(r < n);

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
        int timer = 0;
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
        
        if(!on_edge)
            op(pos[u], pos[v]);
        else if(u != v)
            op(pos[u] + 1, pos[v]);
    }
    
    void Set(int v, const I &info)
    {
        tree.Set(pos[v], info);
    }
    void ModifyPath(int u, int v, const T &tag)
    {
        ProcessPath(u, v, [this, &tag](int l, int r)  {tree.Modify(l, r, tag);});
    }
    void ModifySubtree(int u, const T &tag)
    {
        if(!on_edge)
            tree.Modify(pos[u], out[u] - 1, tag);
        else if(pos[u] < out[u] - 1)
            tree.Modify(pos[u] + 1, out[u] - 1, tag);
    }

    I Get(int v)
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
        if(on_edge)
            return (pos[u] < out[u] - 1 ? tree.Query(pos[u] + 1, out[u] - 1) : I());   
        return tree.Query(pos[u], out[u] - 1);
    }
};