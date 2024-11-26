template <typename info>
class segment_tree_chan
{
public:
    int n;
    vector<info> infos;
    seg_tree::in_order_layout layout;

    void update_node(seg_tree::point a)
    {
        infos[a] = infos[a.c(0)].unite(infos[a.c(1)]);
    }

    segment_tree_chan(int n_) : segment_tree_chan(vector<info>(n_)) {}

    segment_tree_chan(const vector<info> &a) : n(int(a.size()))
    {
        assert(n > 0);
        infos.resize(2 * n);
        layout = seg_tree::in_order_layout(n);
        for (int i = 0; i < n; i++)
        {
            infos[layout.get_point(i)] = a[i];
        }
        for (int i = n - 1; i >= 1; i--)
        {
            infos[i] = infos[2 * i].unite(infos[2 * i + 1]);
        }
    }

    void set(int p, const info &v)
    {
        auto pt = layout.get_point(p);
        infos[pt] = v;
        pt.for_parents_up([&](seg_tree::point a)
                          { update_node(a); });
    }

    info query(int l, int r)
    {
        ++ r;
        auto rng = layout.get_range(l, r);
        info res;
        rng.for_each_l_to_r([&](seg_tree::point a)
                            { res = res.unite(infos[a]); });
        return res;
    }

    info get(int p)
    {
        auto pt = layout.get_point(p);
        return infos[pt];
    }

    //returns max `r` such that `f(sum[l, r]) = true` given that `f` is monotonic as `r` increases 
    //if `(r > n)`, then `f(sum[l, n]) = true` 
    //if `(r < l)`, then `f(sum[l, l]) = false`
    template <typename F>
    int max_right(int l, F f)
    {
        auto rng = layout.get_range(l, n);
        int res = n;
        info sum;
        rng.for_each_l_to_r([&](seg_tree::point a)
        {
            if (res != n) 
            {
                return;
            }
            auto new_sum = sum.unite(infos[a]);
            if (f(new_sum))
            {
                sum = new_sum;
                return;
            }
            while (a < n) 
            {
                new_sum = sum.unite(infos[a.c(0)]);
                if (f(new_sum)) 
                {
                    sum = new_sum;
                    a = a.c(1);
                }
                else
                {
                    a = a.c(0);
                }
            }
            res = layout.get_node_bounds(a)[0];
        });
        -- res;
        return min(n, res);
    }

    //returns min point l such that f(sum[l, r]) = true given that f is monotonic as l decreases
    //if (l == 0), then f(sum[0, n]) = true
    //if (l > r), then f(sum[r, r]) = false
    template <typename F>
    int min_left(int r, F f)
    {
        ++ r;
        auto rng = layout.get_range(0, r);
        int res = 0;
        info sum;
        rng.for_each_r_to_l([&](seg_tree::point a)
        {
            if (res != 0) 
            {
                return;
            }
            
            auto new_sum = infos[a].unite(sum);
            if (f(new_sum))
            {
                sum = new_sum;
                return;
            }
            while (a < n)
            {
                new_sum = infos[a.c(1)].unite(sum);
                if (f(new_sum)) 
                {
                    sum = new_sum;
                    a = a.c(0);
                }
                else
                {
                    a = a.c(1);
                }
            }
            res = layout.get_node_bounds(a)[1];
        });
        return res;
    }
};
