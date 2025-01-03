class dsu_chan
{
    /*
        tc: 
            O(n a(n)) amortized
            O(n log(n)) for rollbackable

        info:
            par[u] stores leader of u
            siz[u] stores size of component with leader u

        modify:
            to make rollbackable, we do not make modifications in get() [which makes the get() function O(log(N))] 
            and store change info in unite()
    */
public:
    int n;
    vector<int> par;
    vector<int> siz;
    dsu_chan(int n) : n(n), par(n), siz(n, 1)
    {
        iota(par.begin(), par.end(), 0);
    };
    
    int get(int x)
    {
        return (par[x] == x ? x : par[x] = get(par[x]));
    }

    void unite(int x, int y)
    {
        x = get(x), y = get(y);
        if(x == y)
            return;
        if(siz[x] > siz[y]) swap(x, y);
        par[x] = y, siz[y] += siz[x];
    }

    vector<vector<int>> group()
    {
        vector<vector<int>> g(n);
        for(int u = 0; u < n; u ++)
            g[get(u)].push_back(u);
        return g;
    }
};