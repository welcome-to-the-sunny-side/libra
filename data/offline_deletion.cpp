template<typename T, typename Q, typename D>
class offline_deletion_chan
{
public:
    int n;
    vector<Q> ans;
    vector<vector<T>> store;
    D ds;

    offline_deletion_chan(int n) : 
    n(n), ans(4 * n + 5), store(4 * n + 5) {};

    //put(L, R, life) puts element "life" into segment [L, R]
    void put(int L, int R, T life)
    {
        auto rec = [&](int v, int l, int r, auto &&rec) -> void
        {
            if(l > r or r < L or R < l)
                return;

            if(L <= l and r <= R)
            {
                store[v].push_back(life);
                return;
            }
            
            int mid = (l + r)/2;
            put(v * 2, l, mid, rec);
            put(v * 2 + 1, mid + 1, r, rec);
        };
        rec(1, 1, n, rec);
    }

    void compute()
    {
        auto dfs = [&](int v, int l, int r, auto &&dfs) -> void
        {
            if(l > r)
                return;

            for(auto point : store[v])
                ds.Insert(point);

            if(l == r)
                ans[l] = ds.Query();
            else
            {
                int mid = (l + r)/2;
                dfs(v * 2, l, mid, dfs);
                dfs(v * 2 + 1, mid + 1, r, dfs);
            }
            
            for(auto point : store[v])
                ds.Rollback();
        };
        dfs(1, 1, n, dfs);
    }
};
