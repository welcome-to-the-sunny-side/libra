template <typename Info, typename Tag>
class LazySegmentTree
{
public:
    int n;
    vector<Info> infos;
    vector<Tag> tags;
    seg_tree::in_order_layout layout;

    void Apply(seg_tree::point a, const Tag &t)
    {
        auto [l, r] = layout.get_node_bounds(a);
        if (!t.ApplyTo(infos[a], l, r - 1))     //r - 1 to make inclusive
        {
            assert(a < n);
            DowndateNode(a);
            Apply(a.c(0), t);
            Apply(a.c(1), t);
            UpdateNode(a);
            return;
        }
        if (a < n)
        {
            t.ApplyTo(tags[a]);
        }
    }

    void DowndateNode(seg_tree::point a)
    {
        if (!tags[a].Empty())
        {
            Apply(a.c(0), tags[a]);
            Apply(a.c(1), tags[a]);
            tags[a] = Tag();
        }
    }

    void UpdateNode(seg_tree::point a)
    {
        infos[a] = infos[a.c(0)].Unite(infos[a.c(1)]);
    }

    LazySegmentTree() : LazySegmentTree(0) {}
    LazySegmentTree(int n_) : LazySegmentTree(vector<Info>(n_)) {}
    LazySegmentTree(const vector<Info> &a) : n(int(a.size()))
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
            UpdateNode(seg_tree::point(i));
        }
    }

    void Modify(int l, int r, const Tag &t)
    {
        ++ r;
        auto rng = layout.get_range(l, r);
        rng.for_parents_down([&](seg_tree::point a)
                            { DowndateNode(a); });
        rng.for_each([&](seg_tree::point a)
                     { Apply(a, t); });
        rng.for_parents_up([&](seg_tree::point a)
                            { UpdateNode(a); });
    }

    void Set(int p, const Info &v)
    {
        auto pt = layout.get_point(p);
        pt.for_parents_down([&](seg_tree::point a)
                            { DowndateNode(a); });
        infos[pt] = v;
        pt.for_parents_up([&](seg_tree::point a)
                          { UpdateNode(a); });
    }

    Info Query(int l, int r)
    {
        ++ r;
        auto rng = layout.get_range(l, r);
        rng.for_parents_down([&](seg_tree::point a)
                             { DowndateNode(a); });
        Info res;
        rng.for_each_l_to_r([&](seg_tree::point a)
                            { res = res.Unite(infos[a]); });
        return res;
    }

    Info Get(int p)
    {
        auto pt = layout.get_point(p);
        pt.for_parents_down([&](seg_tree::point a)
                            { DowndateNode(a); });
        return infos[pt];
    }

    //returns max point r such that f(sum[l, r]) = true given that f is monotonic as r increases 
    //if (r > n), then f(sum[l, n]) = true 
    //if (r < l), then f(sum[l, l]) = false
    template <typename F>
    int MaxRight(int l, F f)
    {
        auto rng = layout.get_range(l, n);
        rng.for_parents_down([&](seg_tree::point a) { DowndateNode(a); });
        
        int res = n;
        Info sum;
        rng.for_each_l_to_r([&](seg_tree::point a)
        {
            if (res != n)
            {
                return;
            }
            auto new_sum = sum.Unite(infos[a]);
            if (f(new_sum)) 
            {
                sum = new_sum;
                return;
            }
            while (a < n)
            {
                DowndateNode(a);
                new_sum = sum.Unite(infos[a.c(0)]);
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
    int MinLeft(int r, F f)
    {
        ++ r;
        auto rng = layout.get_range(0, r);
        rng.for_parents_down([&](seg_tree::point a) { DowndateNode(a); });
        
        int res = 0;
        Info sum;
        rng.for_each_r_to_l([&](seg_tree::point a)
        {
            if (res != 0) 
            {
                return;
            }
            auto new_sum = infos[a].Unite(sum);
            if (f(new_sum))
            {
                sum = new_sum;
                return;
            }
            while (a < n)
            {
                DowndateNode(a);
                new_sum = infos[a.c(1)].Unite(sum);
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
