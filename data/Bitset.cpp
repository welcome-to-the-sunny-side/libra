template<typename T, const int B>
class BitsetChan
{
//helper
public:
    using T_T = T;

    static inline bool on(int i, T x)
    {
        return ((T(1) << i) & x);
    }
    
    static constexpr int popcnt(T x)
    {
        return __builtin_popcountll(x);
    }
    
    static inline T prefix(int i)
    {
        if(i >= B)
            return ~T(0);
        return (i <= 0 ? T(0) : ((T(1) << i) - T(1)));  
    }
    static inline T suffix(int i)
    {
        return ~(prefix(B - i));
    }
    static inline T range(int l, int r)
    {
        return prefix(r) ^ prefix(l - 1);
    }

    static inline int block_id(int i)
    {
        return i/B;
    }

    T submask(int l, int r)
    {
        int bx = block_id(l);
        assert(bx == block_id(r));
        return (b[bx] & range(l - bx * B + 1, r - bx * B + 1)); 
    }

public:
    int n, m;
    vector<T> b;

    BitsetChan(int n) : BitsetChan(n, false) {};
    BitsetChan(int n, bool init) : n(n), m((n + B - 1)/B), b(m, init ? ~T(0) : T(0)) 
    {
        static_assert(sizeof(T) * 8 == B, "check block width");
        static_assert(is_same<T, uint64_t>::value, "modify popcnt()");
        Trim();
    };

    inline void Trim()
    {
        b.back() &= prefix(n % B == 0 ? B : n % B);
    }

    void Set(int i, bool val)
    {
        assert(0 <= i and i < n);
        if(val)
            b[i/B] |= (T(1) << (i % B));
        else
            b[i/B] &= ~(T(1) << (i % B));
    }

    bool Get(int i)
    {
        assert(0 <= i and i < n);
        return b[i/B] & (T(1) << (i % B));
    }

    BitsetChan operator & (const BitsetChan &other)
    {
        BitsetChan result(max(n, other.n), false);
        for(int i = 0; i < min(m, other.m); i ++)
            result.b[i] = b[i] & other.b[i];
        Trim();
        return result;
    }

    void operator &= (const BitsetChan &other)
    {
        for(int i = 0; i < min(m, other.m); i ++)
            b[i] &= other.b[i];
        if(m > other.m)
            fill(b.begin() + other.m, b.begin() + m, T(0));
        Trim();
    }

    BitsetChan operator | (const BitsetChan &other)
    {
        BitsetChan result((n > other.n ? *this : other));
        const BitsetChan* overlap = (m > other.m ? &other : this);
        for(int i = 0; i < min(m, other.m); i ++)
            result.b[i] |= overlap->b[i];
        Trim();
        return result;
    }

    void operator |= (const BitsetChan &other)
    {
        for(int i = 0; i < min(m, other.m); i ++)
            b[i] |= other.b[i];
        Trim();
    }

    BitsetChan operator ^ (const BitsetChan &other)
    {
        BitsetChan result((n > other.n ? *this : other));
        const BitsetChan* overlap = (m > other.m ? &other : this);
        for(int i = 0; i < min(m, other.m); i ++)
            result.b[i] ^= overlap->b[i];
        Trim();
        return result;
    }

    void operator ^= (const BitsetChan &other)
    {
        for(int i = 0; i < min(m, other.m); i ++)
            b[i] ^= other.b[i];
        Trim();
    }

    BitsetChan operator << (int x)
    {
        if(x == 0)
            return BitsetChan(*this);

        BitsetChan result(n);

        if(x >= n)
            return result;

        int s = x/B, d = x % B;

        copy(b.begin(), b.begin() + m - s, result.b.begin() + s);

        if(d > 0)
        {
            result.b[m - 1] <<= d;
            for(int i = m - 2; i >= 0; i --)
            {
                result.b[i + 1] |= (result.b[i] >> (B - d));
                result.b[i] <<= d;
            }
        }

        Trim();
        return result;
    }

    void operator <<= (int x)
    {
        if(x == 0)
            return;

        if(x >= n)
        {
            fill(b, b + m, T(0));
            return;
        }

        int s = x/B, d = x % B;
        
        for(int i = m - 1; i >= s; i --)
            b[i] = b[i - s];

        if(d > 0)
        {
            b[m - 1] <<= d;
            for(int i = m - 2; i >= 0; i --)
            {
                b[i + 1] |= (b[i] >> (B - d));
                b[i] <<= d;
            }
        }

        Trim();
    }

    BitsetChan operator >> (int x)
    {
        if(x == 0)
            return BitsetChan(*this);

        BitsetChan result(n);

        if(x >= n)
            return result;

        int s = x/B, d = x % B;

        copy(b.begin() + s, b.end(), result.b.begin());

        if(d > 0)
        {
            result.b[0] >>= d;
            for(int i = 1; i < m; i ++)
            {
                result.b[i - 1] |= (result.b[i] << (B - d));
                result.b[i] >>= d;
            }
        }
        Trim();
        return result;
    }

    void operator >>= (int x)
    {
        if(x == 0)
            return;

        if(x >= n)
        {
            fill(b.begin(), b.end(), T(0));
            return;
        }

        int s = x/B, d = x % B;

        for(int i = 0; i < m - s; i ++)
            b[i] = b[i + s];

        if(d > 0)
        {
            b[0] >>= d;
            for(int i = 1; i < m; i ++)
            {
                b[i - 1] |= (b[i] << (B - d));
                b[i] >>= d;
            }
        }

        Trim();
    }

    BitsetChan operator ~()
    {
        BitsetChan result(*this);
        for(auto &v : result)
            v = ~v;
        result.Trim();
        return result;
    }

    int Count()
    {
        int cnt = 0;
        for(const auto &v : b)
            cnt += popcnt(v);
        return cnt;
    }

    void RangeProcess(int l, int r, auto block_brute, auto block_quick)
    {
        assert(0 <= l and l <= r and r < n);

        int bl = block_id(l), br = block_id(r);

        if(bl == br)
            block_brute(l, r);
        else
        {
            block_brute(l, (bl + 1) * B);
            for(int bi = bl + 1; bi < br; bi ++)
                block_quick(bi);
        }
    }

    void RangeSet(int l, int r, bool val)
    {
        auto block_brute = [&](int l, int r) -> void
        {
            int bi = block_id(l);
            T mask = range(l - bi * B + 1, r - bi * B + 1);
            if(val)
                b[bi] |= mask;
            else
                b[bi] &= ~mask;
        };
        auto block_quick = [&](int bi) -> void
        {
            b[bi] = (val ? ~T(0) : T(0));
        };
        RangeProcess(l, r, block_brute, block_quick);
    }

    int RangeCount(int l, int r)
    {
        int cnt = 0;
        auto block_brute = [&](int l, int r) -> void
        {
            cnt += popcnt(submask(l, r));
        };
        auto block_quick = [&](int bi) -> void
        {
            cnt += popcnt(b[bi]);
        };
        RangeProcess(l, r, block_brute, block_quick);
        return cnt;
    }

    void Show()
    {
        for(int i = m - 1; i >= 0; i --)
            cerr << bitset<B> (b[i]);
        cerr << endl;
    }
};
using BitsetChan64 = BitsetChan<uint64_t, 64>;