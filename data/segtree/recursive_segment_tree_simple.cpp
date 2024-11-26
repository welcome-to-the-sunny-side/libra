template <typename info>
class segment_tree_chan
{
public:
    int n;
    vector<info> infos;

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
    segment_tree_chan(int n) : segment_tree_chan(vector<info> (n)) {};
    segment_tree_chan(const vector<info> &a) : 
    n((int)a.size()), infos(4 * n + 5)
    {
        auto build = [&](int v, int l, int r, auto &&build) -> void
        {
            if(l > r)
                return;
            if(l == r)
            {
                infos[v] = info(a[l]);
                return;
            }
            int m = (l + r)/2;
            build(v * 2, l, m, build);
            build(v * 2 + 1, m + 1, r, build);
            infos[v] = infos[v * 2].unite(infos[v * 2 + 1]);
        };
        build(1, 0, n - 1, build);
    };

    void set(int p, const info &info)
    {
        recurse(p, p, true, [&](int v, int l, int r)
        {
            infos[v] = info;
        });
    }
    void add(int p, const info &info)
    {
        recurse(p, p, true, [&](int v, int l, int r)
        {
            infos[v] = infos[v].unite(info);
        });
    }
    info query(int lb, int rb)
    {
        info res = info();
        recurse(lb, rb, false, [&](int v, int l, int r)
        {
            res = res.unite(infos[v]);
        });
        return res;
    }
    info get(int p)
    {
        info res = info();
        recurse(p, p, false, [&](int v, int l, int r)
        {
            res = infos[v];
        });
        return res;
    }
};