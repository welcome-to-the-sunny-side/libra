struct Dsu
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

    vector<int> par, siz;
    void init(int n)
    {
        par.assign(n + 1, 0);
        siz.assign(n + 1, 1);
        iota(par.begin(), par.end(), 0);
    }
    
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

    vector<vector<int>> groups()
    {
        vector<vector<int>> g(n + 1);
        for(int u = 1; u <= n; u ++)
            g[get(u)].push_back(u);
        return g;
    }
};