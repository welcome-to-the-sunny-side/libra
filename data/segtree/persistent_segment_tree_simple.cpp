template <typename info, typename info_ptr>
class p_segment_tree_chan
{
public:
    int n, r, U;
    map<int, int> roots;
    vector<info> infos;
    vector<info_ptr> info_ptrs;

    template<typename O>
    void recurse(int root, int lb, int rb, bool update, O op)
    {
        auto rec = [&](int v, int l, int r, auto &&rec) -> void
        {
            if(l > r or v == 0)
                return;

            if(lb <= l and r <= rb)
            {
                op(v, l, r);
                return;
            }

            int m = (l + r)/2;

            if(m >= lb)
            {
                if(update)
                {
                    int old_lc = info_ptrs[v].lc;
                    info_ptrs[v].lc = ++ U;
                    infos[info_ptrs[v].lc] = infos[old_lc];
                    info_ptrs[info_ptrs[v].lc] = info_ptrs[old_lc];
                }
                rec(info_ptrs[v].lc, l, m, rec);
            }
            if(m + 1 <= rb)
            {
                if(update)
                {
                    int old_rc = info_ptrs[v].rc;
                    info_ptrs[v].rc = ++ U;
                    infos[info_ptrs[v].rc] = infos[old_rc]; 
                    info_ptrs[info_ptrs[v].rc] = info_ptrs[old_rc];
                }
                rec(info_ptrs[v].rc, m + 1, r, rec);
            }
            
            if(update)
                infos[v] = infos[info_ptrs[v].lc].unite(infos[info_ptrs[v].rc]);
        };
        rec(root, 0, n - 1, rec);
    };

    p_segment_tree_chan() : p_segment_tree_chan(0, 0) {};
    p_segment_tree_chan(int n, int r) : p_segment_tree_chan(n, r, vector<info> (r)) {};
    p_segment_tree_chan(int n, int r, const vector<info> &a) : 
    n(n), r(r), infos(r + 1), info_ptrs(r + 1), U(0)
    {
        auto build = [&](int v, int l, int r, auto &&build) -> void
        {
            if(!v or l > r)
                return;

            if(l == r)
            {
                infos[v] = info(a[l]);
                return;
            }
            int m = (l + r)/2;
            if(l <= m)
            {
                info_ptrs[v].lc = ++ U;
                build(info_ptrs[v].lc, l, m, build);
            }
            if(m + 1 <= r)
            {
                info_ptrs[v].rc = ++ U;
                build(info_ptrs[v].rc, m + 1, r, build);
            }
            infos[v] = infos[info_ptrs[v].lc].unite(infos[info_ptrs[v].rc]);
        };
        int root = ++ U;
        roots[0] = root;
        build(U, 0, n - 1, build);
    };

    pair<int, int> resolve_roots (int new_root, int old_root)
    {
        auto it = roots.find(old_root);
        assert(it != roots.end());
        old_root = (*it).second;

        assert(roots.find(new_root) == roots.end());
        roots[new_root] = ++ U;
        new_root = U;
        infos[new_root] = infos[old_root];
        info_ptrs[new_root] = info_ptrs[old_root];

        return {new_root, old_root};
    };

    void set(int new_root, int old_root, int p, const info &infonode)
    {
        tie(new_root, old_root) = resolve_roots(new_root, old_root);

        recurse(new_root, p, p, true, [&](int v, int l, int r)
        {
            infos[v] = infonode;
        });
    }
    void add(int new_root, int old_root, int p, const info &infonode)
    {
        tie(new_root, old_root) = resolve_roots(new_root, old_root);

        recurse(new_root, p, p, true, [&](int v, int l, int r)
        {
            infos[v] = infos[v].unite(infonode);
        });
    }
    info query(int root, int lb, int rb)
    {
        auto it = roots.find(root);
        assert(it != roots.end());
        root = (*it).second;
        info res = info();
        recurse(root, lb, rb, false, [&](int v, int l, int r)
        {
            res = res.unite(infos[v]);
        });
        return res;
    }
    info get(int root, int p)
    {
        auto it = roots.find(root);
        assert(it != roots.end());
        root = (*it).second;
        info res = info();
        recurse(root, p, p, false, [&](int v, int l, int r)
        {
            res = infos[v];
        });
        return res;
    }
};
