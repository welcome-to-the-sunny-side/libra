template <typename Info>
class segment_tree_chan
{
public:
    int n;
    vector<Info> infos;

    template<typename O>
    void recurse(int lb, int rb, bool update, O op)
    {
        auto rec = [&](int v, int l, int r, auto &&rec) -> void
        {
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
            if(m + 1 <= rb)
                rec(2 * v + 1, m + 1, r, rec);
            
            if(update)
                infos[v] = infos[2 * v].unite(infos[2 * v + 1]);
        };
        rec(1, 0, n - 1, rec);
    };

    segment_tree_chan() : segment_tree_chan(0) {};
    segment_tree_chan(int n) : segment_tree_chan(vector<Info> (n)) {};
    segment_tree_chan(const vector<Info> &a) : 
    n((int)a.size()), infos(4 * n + 5)
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
            infos[v] = infos[v * 2].unite(infos[v * 2 + 1]);
        };
        build(1, 0, n - 1, build);
    };

    void set(int p, const Info &info)
    {
        recurse(p, p, true, [&](int v, int l, int r)
        {
            infos[v] = info;
        });
    }
    void add(int p, const Info &info)
    {
        recurse(p, p, true, [&](int v, int l, int r)
        {
            infos[v] = infos[v].unite(info);
        });
    }
    Info query(int lb, int rb)
    {
        info res = info();
        recurse(lb, rb, false, [&](int v, int l, int r)
        {
            res = res.unite(infos[v]);
        });
        return res;
    }
    Info get(int p)
    {
        info res = Info();
        recurse(p, p, false, [&](int v, int l, int r)
        {
            res = infos[v];
        });
        return res;
    }
};