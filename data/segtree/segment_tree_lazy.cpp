template <typename info, typename tag>
class lazy_segment_tree_chan
{
public:
    int n;
    vector<info> infos;
    vector<tag> tags;
    seg_tree::in_order_layout layout;

    void apply(seg_tree::point a, const tag &t)
    {
        auto [l, r] = layout.get_node_bounds(a);
        if (!t.apply_to(infos[a], l, r - 1))     //r - 1 to make inclusive
        {
            assert(a < n);
            downdate_node(a);
            apply(a.c(0), t);
            apply(a.c(1), t);
            update_node(a);
            return;
        }
        if (a < n)
        {
            t.apply_to(tags[a]);
        }
    }

    void downdate_node(seg_tree::point a)
    {
        if (!tags[a].empty())
        {
            apply(a.c(0), tags[a]);
            apply(a.c(1), tags[a]);
            tags[a] = tag();
        }
    }

    void update_node(seg_tree::point a)
    {
        infos[a] = infos[a.c(0)].unite(infos[a.c(1)]);
    }

    lazy_segment_tree_chan() : lazy_segment_tree_chan(0) {}
    lazy_segment_tree_chan(int n_) : lazy_segment_tree_chan(vector<info>(n_)) {}
    lazy_segment_tree_chan(const vector<info> &a) : n(int(a.size()))
    {
        infos.resize(2 * n);
        tags.resize(n);
        layout = seg_tree::in_order_layout(n);
        for (int i = 0; i < n; i++)
        {
            infos[layout.get_point(i)] = a[i];
        }
        for (int i = n - 1; i >= 1; i--)
        {
            update_node(seg_tree::point(i));
        }
    }

    void modify(int l, int r, const tag &t)
    {
        ++ r;
        auto rng = layout.get_range(l, r);
        rng.for_parents_down([&](seg_tree::point a)
                            { downdate_node(a); });
        rng.for_each([&](seg_tree::point a)
                     { apply(a, t); });
        rng.for_parents_up([&](seg_tree::point a)
                            { update_node(a); });
    }

    void set(int p, const info &v)
    {
        auto pt = layout.get_point(p);
        pt.for_parents_down([&](seg_tree::point a)
                            { downdate_node(a); });
        infos[pt] = v;
        pt.for_parents_up([&](seg_tree::point a)
                          { update_node(a); });
    }

    info query(int l, int r)
    {
        ++ r;
        auto rng = layout.get_range(l, r);
        rng.for_parents_down([&](seg_tree::point a)
                             { downdate_node(a); });
        info res;
        rng.for_each_l_to_r([&](seg_tree::point a)
                            { res = res.unite(infos[a]); });
        return res;
    }

    info get(int p)
    {
        auto pt = layout.get_point(p);
        pt.for_parents_down([&](seg_tree::point a)
                            { downdate_node(a); });
        return infos[pt];
    }

    //returns max point r such that f(sum[l, r]) = true given that f is monotonic as r increases 
    //if (r > n), then f(sum[l, n]) = true 
    //if (r < l), then f(sum[l, l]) = false
    template <typename F>
    int max_right(int l, F f)
    {
        auto rng = layout.get_range(l, n);
        rng.for_parents_down([&](seg_tree::point a) { downdate_node(a); });
        
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
                downdate_node(a);
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
        return min(res, n);
    }

    //returns min point l such that f(sum[l, r]) = true given that f is monotonic as l decreases
    //if (l == 0), then f(sum[0, n]) = true
    //if (l > r), then f(sum[r, r]) = false
    template <typename F>
    int min_left(int r, F f)
    {
        ++ r;
        auto rng = layout.get_range(0, r);
        rng.for_parents_down([&](seg_tree::point a) { downdate_node(a); });
        
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
                downdate_node(a);
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
