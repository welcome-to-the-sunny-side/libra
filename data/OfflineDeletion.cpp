template<typename T, typename Q, typename D>
class OfflineDetection
{
public:
    int n;
    vector<Q> ans;
    vector<vector<T>> store;
    D ds;

    OfflineDetection(int n) : n(n), ans(4 * n + 5), store(4 * n + 5) {};

    //Put(L, R, life) puts element "life" into segment [L, R]
    void Put(int L, int R, T life)
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

    void Compute()
    {
        auto dfs = [&](int v, int l, int r, auto &&dfs) -> void
        {
            if(l > r)
                return;

            for(auto point : store[v])
                ds.Insert(point);           //insert segment into data structure

            if(l == r)
                ans[l] = ds.Query();        //query the data structure
            else
            {
                int mid = (l + r)/2;
                dfs(v * 2, l, mid, dfs);
                dfs(v * 2 + 1, mid + 1, r, dfs);
            }
            
            for(auto point : store[v])
                ds.Rollback();              //rollback the change
        };
        dfs(1, 1, n, dfs);
    }
};
