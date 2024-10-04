class DSUChan
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
    vector<int> par, siz;
    DSUChan(int n) : n(n), par(n), siz(n, 1)
    {
        iota(par.begin(), par.end(), 0);
    };
    
    int Get(int x)
    {
        return (par[x] == x ? x : par[x] = Get(par[x]));
    }

    void Unite(int x, int y)
    {
        x = Get(x), y = Get(y);
        if(x == y)
            return;
        if(siz[x] > siz[y]) swap(x, y);
        par[x] = y, siz[y] += siz[x];
    }

    vector<vector<int>> Groups()
    {
        vector<vector<int>> g(n);
        for(int u = 0; u < n; u ++)
            g[Get(u)].push_back(u);
        return g;
    }
};