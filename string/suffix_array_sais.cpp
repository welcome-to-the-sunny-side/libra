class sais_chan
{
public:
    static constexpr uint32_t emp = 1 << 30;
    static constexpr uint32_t lim = 1 << 26;

    uint32_t sz = 0;
    uint32_t al = 0;
    vector<uint8_t> type;       // L, S, LMS <=> {0, 1, 2}
    vector<uint32_t> t;         //intermediate string rep
    vector<uint32_t> tsa;       //intermediate sa 
    vector<uint32_t> cnt;       //for blocks
    vector<uint32_t> lms;       //lms[i] = index of i'th lms

    void prepare(uint32_t n, uint32_t m)
    {
        if(sz < 2 * n)
        {
            sz = 2 * n;
            type.resize(2 * n);
            t.resize(2 * n);
            tsa.resize(2 * n);
            lms.resize(2 * n);
        }
        if(al < 2 * m)
        {
            al = 2 * m;
            cnt.resize(2 * m);
        }
    }

    template<typename T>
    void sa(const uint32_t n, const T* s, uint32_t* sa)
    {
        static_assert(is_scalar_v<T>);
        static_assert(sizeof(T) <= sizeof(uint32_t));

        if(n == 0)
            return;
        assert(n < lim);

        uint32_t mn = *min_element(s, s + n), mx = *max_element(s, s + n);

        prepare(n + 2, max(n, mx - mn + 1) + 2);

        for(uint32_t i = 0; i < n; i ++)
            t[i] = s[i] - mn + 1;
        t[n] = 0;

        mx -= mn;
        mx += 1;
        mn = 1;

        auto recurse = [&](uint32_t n, uint32_t m, uint32_t* s, uint32_t *sa, uint8_t *type, uint32_t* cnt, uint32_t* lms, auto &&recurse) -> void
        {
            //conditions: 
            //1. n > 1
            //2. s[n - 1] == 0
            //3. s[i] in [1, m] for i < n - 1

            assert(n > 1);

            if(n == 2)
            {
                sa[0] = 1, sa[1] = 0;
                return; 
            }

            type[n - 1] = 2;
            for(uint32_t i = n - 2; ; i --)
            {
                if(s[i] == s[i + 1])
                    type[i] = type[i + 1];
                else if(s[i] < s[i + 1])
                    type[i] = 1;
                else if(s[i] > s[i + 1])
                    type[i] = 0;
                if(i == 0)
                    break;
            }

            fill(cnt, cnt + m + 1, 0);
            for(uint32_t i = 0; i < n; i ++)
                cnt[s[i]] ++;
            for(uint32_t x = 1; x <= m; x ++)
                cnt[x] += cnt[x - 1];
            uint32_t* dcnt = cnt + m + 1;

            fill(sa, sa + n, emp);

            //identify lms types, place them in the SA
            copy(cnt, dcnt, dcnt);
            for(uint32_t i = 1; i + 1 < n; i ++)
                if(type[i - 1] == 0 and type[i] == 1)
                    type[i] = 2, sa[--dcnt[s[i]]] = i;
            sa[--dcnt[s[n - 1]]] = n - 1;

            //induce L types
            auto induce_l = [&]() -> void
            {
                copy(cnt, cnt + m, dcnt + 1);
                //dcnt[0] is never read here
                for(uint32_t i = 0; i < n; i ++)
                    if(sa[i] != emp and sa[i] != 0 and type[sa[i] - 1] == 0)
                    {
                        uint32_t j = sa[i] - 1;
                        sa[dcnt[s[j]] ++] = j;
                    }
            };
            //induce S types
            auto induce_s = [&]() -> void
            {
                copy(cnt, dcnt, dcnt);
                for(uint32_t i = n - 1; ; i --)
                {
                    if(sa[i] != 0 and type[sa[i] - 1] != 0)
                    {
                        //assert(sa[i] != emp)
                        uint32_t j = sa[i] - 1;
                        sa[--dcnt[s[j]]] = j;
                    }
                    if(i == 0)
                        break;
                }
            };
            //check for violation of LMS equality
            auto diff = [&](uint32_t i, uint32_t j) -> bool
            {
                if(s[i] != s[j])
                    return true;

                for(uint32_t o = 1; ; o ++)
                {
                    if(s[i + o] != s[j + o])
                        return true;
                    if(type[i + o] != type[j + o])
                        return true;
                    if(type[i + o] == 2)
                        return false;
                }
                return false;
            };

            induce_l();
            induce_s();

            //reorder inside lms
            uint32_t lcnt = 0;
            for(uint32_t i = 0; i < n; i ++)
                if(type[sa[i]] == 2)
                    lms[lcnt ++] = sa[i];

            //hmm terminal
            assert(lcnt > 0);
                
            //reduce... and then place lms in true relative order at tails
            if(lcnt > 1)
            {
                uint32_t label = 0;

                //just reuse the sa here
                uint32_t *sl = sa;

                //smallest is necessarily the terminal character
                assert(lms[0] == n - 1);
                sl[lms[0]] = label;

                for(uint32_t i = 1; i < lcnt; i ++)
                {
                    label += diff(lms[i - 1], lms[i]);
                    sl[lms[i]] = label;
                }   

                assert(label <= lcnt and lcnt <= n/2);
                if(label + 1 < lcnt)
                {
                    //only now do we need to recurse hmmmm

                    //reconstruct the required string
                    uint32_t* ns = s + n;
                    for(uint32_t i = 0, idx = 0; i < n; i ++)
                        if(type[i] == 2)
                            lms[idx] = i, ns[idx ++] = sl[i];

                    recurse(lcnt, label, ns, sa, type + n, cnt, lms + n, recurse);
                    uint32_t* osa = sa + n;
                    copy(sa, sa + lcnt, osa);

                    //reset things that might get contaminated
                    fill(sa, sa + n, emp);
                    fill(cnt, cnt + m + 1, 0);
                    for(uint32_t i = 0; i < n; i ++)
                        cnt[s[i]] ++;
                    for(uint32_t x = 1; x <= m; x ++)
                        cnt[x] += cnt[x - 1];

                    copy(cnt, dcnt, dcnt);
                    for(uint32_t i = lcnt - 1; ; i --)
                    {
                        uint32_t j = lms[osa[i]];
                        sa[--dcnt[s[j]]] = j;
                        if(i == 0)
                            break;
                    }
                }
                else
                {
                    fill(sa, sa + n, emp);
                    copy(cnt, dcnt, dcnt);
                    for(uint32_t i = lcnt - 1; ; i --)
                    {
                        uint32_t j = lms[i];
                        sa[--dcnt[s[j]]] = j;
                        if(i == 0)
                            break;
                    }
                }                
            }
            else
                sa[0] = n - 1;

            induce_l();
            induce_s();
        };

        recurse(n + 1, mx, t.data(), tsa.data(), type.data(), cnt.data(), lms.data(), recurse);
        copy(tsa.data() + 1, tsa.data() + n + 1, sa);
    }
};
