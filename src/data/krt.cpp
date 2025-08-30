class krt_chan
{
public:
    int n;

    vector<int> wg;
    vector<vector<int>> krt;

    krt_chan(int n, vector<array<int, 3>> edge, auto cmp) : n(n), wg(2 * n, -1), krt(2 * n)
    {
        int id = n;
        vector<int> par(2 * n);
        iota(par.begin(), par.end(), 0);

        auto find = [&](int u, auto &&find) -> int
        {
            return (par[u] == u ? u : par[u] = find(par[u], find));
        };
        auto unite = [&](int u, int v, int w) -> void
        {
            u = find(u, find), v = find(v, find);
            if(u == v)
                return;
            par[u] = par[v] = par[id] = id;
            krt[id] = {u, v};
            wg[id] = w;
            id ++;
        };

        sort(edge.begin(), edge.end(), cmp);
        for(auto [u, v, w] : edge)
            unite(u, v, w);
    };
};
