template <const int mode = 1>
class mo_chan
{
    /*
        info:
            - 0 indexed
            - mode = 1 for the regular query order, mode = 2 for the fancier one
        usage:
            - one provides external add(array index)/delete(array index)/query(query index) functions  
            - naturally, you'll have to maintain the window aggregate externally too
            - so for range sum queries, the external helpers are:
                int64_t sum = 0;
                auto add = [&](int i) -> void
                {
                    sum += a[i];
                };
                auto rem = [&](int i) -> void
                {
                    sum -= a[i];
                };
                vector<int64_t> ans(q);
                auto query = [&](int i) -> void
                {
                    ans[i] = sum;
                };
    */
public:
    int n, q, B;
    vector<array<int, 3>> qr;

    mo_chan(int n, const vector<pair<int, int>> &queries) : n(n), q(queries.size()), B(min(n, int(sqrt(n)) + 1)) 
    {
        for(int i = 0; i < q; i ++)
        {
            auto [l, r] = queries[i];
            qr.push_back({l, r, i});
        }

        auto lmb1 = [&](array<int, 3> a, array<int, 3> b) -> bool
        {
            a[0] /= B, b[0] /= B;
            return a < b;
        };
        auto lmb2 = [&](array<int, 3> a, array<int, 3> b) -> bool
        {
            a[0] /= B, b[0] /= B;
            if(a[0] == b[0])
            {
                if(a[0] & 1)
                    return a[1] > b[1];
                else
                    return b[1] > a[1];
            }
            return a[0] < b[0];
        };
        
        if(mode == 1)
            sort(qr.begin(), qr.end(), lmb1);
        else
            sort(qr.begin(), qr.end(), lmb2);
    };

    void process(auto add, auto rem, auto query)
    {
        int l = 0, r = -1;

        for(auto [nl, nr, qi] : qr)
        {
            while(l > nl)
                add(-- l);
            while(r < nr)
                add(++ r);
            while(l < nl)
                rem(l ++);
            while(r > nr)
                rem(r --);
            query(qi);
        }
    }
};
