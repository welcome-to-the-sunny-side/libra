
template <typename I, typename T, template<typename, typename> typename S>
struct HeavyLight
{
    /*
        tc: O(log^2(n) * x) per query and update
        mc: O(n)

        info:
            1 indexed
        vars:
            I: Info struct of segment tree
            T: Lazy Tag struct of segment tree
            S: segment tree class (S must support S<I, T>)
        warning:
            handle segtree initialization correctly
            monoid operations must be commutative here
    */
    int n, r;
    vector<int> par, heavy, depth, root, pos;
    S<I, T> tree;

    template<typename... Args>
    HeavyLight(int n, int r, const vector<vector<int>> &graph, Args&&... args) :
    n(n), r(r), par(n + 1), heavy(n + 1, -1), depth(n + 1), root(n + 1), pos(n + 1),
    tree(forward<Args>(args)...)
    {
        par[r] = -1;
        depth[r] = 0;
        dfs(graph, r);

        for (int i = 1, currentPos = 1; i <= n; ++ i)
            if (par[i] == -1 or heavy[par[i]] != i)
                for (int j = i; j != -1; j = heavy[j])
                    root[j] = i, pos[j] = currentPos ++;        
    };

    int dfs(const vector<vector<int>> &graph, int u)
    {
        int size = 1, maxSubtree = 0;
        for (int v : graph[u])
            if (v != par[u])
            {
                par[v] = u;
                depth[v] = depth[u] + 1;
                int subtree = dfs(graph, v);
                if (subtree > maxSubtree)
                    heavy[u] = v, maxSubtree = subtree;
                size += subtree;
            }
        return size;
    }

    template <class BinaryOperation>
    void Process(int u, int v, BinaryOperation op)
    {
        for (; root[u] != root[v]; v = par[root[v]])
        {
            if (depth[root[u]] > depth[root[v]])
                swap(u, v);
            op(pos[root[v]], pos[v]);
        }
        if (depth[u] > depth[v])
            swap(u, v);
        op(pos[u], pos[v]);
    }
    void Set(int v, const I &info)
    {
        tree.Set(pos[v], info);
    }
    void Modify(int u, int v, const T &tag)
    {
        Process(u, v, [this, &tag](int l, int r)  {tree.Modify(l, r, tag); });
    }
    I Query(int u, int v)
    {
        I res = I();
        Process(u, v, [this, &res](int l, int r)    {res.Join(tree.Query(l, r));});
        return res;
    }
};