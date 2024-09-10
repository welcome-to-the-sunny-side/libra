struct Info
{
    int sum = 0;

    Info() : sum(0) {};
    Info(int x) : sum(x) {};

    Info Unite(Info b) const 
    {
        Info res(sum + b.sum);
        return res;
    }
    void Join(Info b)
    {
        sum += b.sum;
    }

    static Info GetDefault([[maybe_unused]] int l, [[maybe_unused]] int r)
    {
        return Info();
    }
};
struct Tag
{
    int add = 0;

    Tag() : add(0) {};
    Tag(int x) : add(x) {};

    bool ApplyTo(Info &a, [[maybe_unused]] int l, [[maybe_unused]] int r) const
    {
        a.sum += add * (r - l + 1);
        return true;
    }

    void ApplyTo(Tag &t) const
    {
        t.add += add;
    }

    bool Empty() const
    {
        return add == 0;
    }
};

template <typename Info, typename Tag>
struct SegmentTree
{
    int n;
    bool lazy = false;
    vector<Info> infos;
    vector<Tag> tags;

    SegmentTree() : SegmentTree(false) {};
    SegmentTree(bool lazy) : SegmentTree(lazy, 0) {};
    SegmentTree(bool lazy, int n) : SegmentTree(lazy, vector<Info> (n)) {};
    SegmentTree(bool lazy, const vector<Info> &a) : n((int)a.size())
    {
        this->lazy = lazy;
        infos.resize(4 * n + 5);
        if(lazy)
            tags.resize(4 * n + 5);

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
        if(!lazy or tags[v].Empty())
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
        auto rec = [&](int v, int l, int r, auto &&rec) -> void
        {
            Propagate(v, l, r);
            if(l > r or r < lb or rb < l)
                return;
            if(lb <= l and r <= rb)
            {
                tag.ApplyTo(tags[v]);
                Propagate(v, l, r);
                return;
            }
            int m = (l + r)/2;
            rec(2 * v, l, m, rec);
            rec(2 * v + 1, m + 1, r, rec);
            infos[v] = infos[2 * v].Unite(infos[2 * v + 1]);
        };
        rec(1, 0, n - 1, rec);
    }

    void Set(int p, const Info &info)
    {
        auto rec = [&](int v, int l, int r, auto &&rec) -> void
        {
            Propagate(v, l, r);
            if(l == r)
            {
                infos[v] = info;
                return;
            }
            int m = (l + r)/2;
            if(p <= m)
                rec(2 * v, l, m, rec), Propagate(2 * v + 1, m + 1, r);
            else
                rec(2 * v + 1, m + 1, r, rec), Propagate(2 * v, l, m);  
            infos[v] = infos[2 * v].Unite(infos[2 * v + 1]);
        };
        rec(1, 0, n - 1, rec);
    }

    void Add(int p, const Tag &tag)
    {
        auto rec = [&](int v, int l, int r, auto &&rec) -> void
        {
            Propagate(v, l, r);
            if(l == r)
            {
                tag.ApplyTo(tags[v]);
                Propagate(v, l, r);
                return;
            }
            int m = (l + r)/2;
            if(p <= m)
                rec(2 * v, l, m, rec), Propagate(2 * v + 1, m + 1, r);
            else
                rec(2 * v + 1, m + 1, r, rec), Propagate(2 * v, l, m);  
            infos[v] = infos[2 * v].Unite(infos[2 * v + 1]);
        };
        rec(1, 0, n - 1, rec);
    }

    Info Query(int lb, int rb)
    {
        Info res = Info();
        auto rec = [&](int v, int l, int r, auto &&rec) -> void
        {
            Propagate(v, l, r);
            if(l > r or r < lb or rb < l)
                return;
            
            if(lb <= l and r <= rb)
            {
                res = res.Unite(infos[v]);
                return;
            }
            int m = (l + r)/2;
            rec(2 * v, l, m, rec);
            rec(2 * v + 1, m + 1, r, rec);
        };
        rec(1, 0, n - 1, rec);
        return res;
    }

    Info Get(int p)
    {
        auto rec = [&](int v, int l, int r, auto &&rec) -> Info
        {
            Propagate(v, l, r);      
            if(l == r)
                return infos[v];
            int m = (l + r)/2;
            if(p <= m)
                return rec(2 * v, l, m, rec);
            else
                return rec(2 * v + 1, m + 1, r, rec);
        };
        return rec(1, 0, n - 1, rec);
    }
};
