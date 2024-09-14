template <typename Info, typename Tag>
class SegmentTree
{
public:
    int n;
    bool lazy = false;
    vector<Info> infos;
    vector<Tag> tags;

    template<typename O>
    void Recurse(int lb, int rb, bool update, O op)
    {
        auto rec = [&](int v, int l, int r, auto &&rec) -> void
        {
            if(lazy)
                Propagate(v, l, r);

            if(l > r)
                return;
            
            if(lb <= l and r <= rb)
            {
                op(v, l, r);
                return;
            }
            
            int m = (l + r)/2;
            
            if(m >= lb)
                rec(2 * v, l, m, rec);
            else if(lazy and update)
                Propagate(2 * v, l, m);

            if(m + 1 <= rb)
                rec(2 * v + 1, m + 1, r, rec);
            else if(lazy and update)
                Propagate(2 * v + 1, m + 1, r);
            
            if(update)
                infos[v] = infos[2 * v].Unite(infos[2 * v + 1]);
        };
        rec(1, 0, n - 1, rec);
    };

    SegmentTree() : SegmentTree(0) {};
    SegmentTree(int n) : SegmentTree(n, true) {};
    SegmentTree(int n, bool lazy) : SegmentTree(vector<Info> (n), lazy) {};
    SegmentTree(const vector<Info> &a, bool lazy) : 
    n((int)a.size()), lazy(lazy), infos(4 * n + 5), tags(lazy ? 4 * n + 5 : 0)
    {
        auto build = [&](int v, int l, int r, auto &&build) -> void
        {
            if(l > r)
                return;
            if(l == r)
            {
                infos[v] = Info(a[l]);
                return;
            }
            int m = (l + r)/2;
            build(v * 2, l, m, build);
            build(v * 2 + 1, m + 1, r, build);
            infos[v] = infos[v * 2].Unite(infos[v * 2 + 1]);
        };
        build(1, 0, n - 1, build);
    };

    void Propagate(int v, int l, int r)
    {
        if(tags[v].Empty())
            return;
        tags[v].ApplyTo(infos[v], l, r);
        if(l != r)
        {
            tags[v].ApplyTo(tags[2 * v]);
            tags[v].ApplyTo(tags[2 * v  + 1]);
        }
        tags[v] = Tag();
    }

    void Modify(int lb, int rb, const Tag &tag)
    {
        assert(lazy);
        Recurse(lb, rb, true, [&](int v, int l, int r)
        {
            tag.ApplyTo(tags[v]);
            Propagate(v, l, r);
        });
    }
    void Set(int p, const Info &info)
    {
        Recurse(p, p, true, [&](int v, int l, int r)
        {
            infos[v] = info;
        });
    }
    void Add(int p, const Info &info)
    {
        Recurse(p, p, true, [&](int v, int l, int r)
        {
            infos[v] = infos[v].Unite(info);
            Propagate(v, l, r);
        });
    }
    Info Query(int lb, int rb)
    {
        Info res = Info();
        Recurse(lb, rb, false, [&](int v, int l, int r)
        {
            res = res.Unite(infos[v]);
        });
        return res;
    }
    Info Get(int p)
    {
        Info res = Info();
        Recurse(p, p, false, [&](int v, int l, int r)
        {
            res = infos[v];
        });
        return res;
    }
};