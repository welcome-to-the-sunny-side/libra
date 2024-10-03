template<typename T, const int B>
class BitsetChan
{
public:
    using T_T = T;
    static_assert(sizeof(T) * 8 == B, "check block width");
    static_assert(std::is_same<T, uint64_t>::value, "modify popcnt(), ctz, clz");

//static helper
public:
    static inline constexpr bool on(int i, T x) noexcept
    {
        return ((T(1) << i) & x) != 0;
    }
    
    static constexpr int popcnt(T x) noexcept
    {
        // return _mm_popcnt_u64(x);
        return __builtin_popcountll(x);
    }
    
    static inline constexpr T prefix(int i) noexcept
    {
        return (i >= B) ? ~T(0) : ((T(1) << i) - T(1));
    }
    static inline constexpr T suffix(int i) noexcept
    {
        return ~prefix(B - i);
    }
    static inline constexpr T range(int l, int r) noexcept
    {
        return prefix(r) ^ prefix(l - 1);
    }
 
    static inline constexpr int block_id(int i) noexcept
    {
        return i / B;
    }
 
//helper
public:
    inline T submask(int l, int r) const noexcept
    {
        int bx = block_id(l);
        assert(bx == block_id(r));
        return (b[bx] & range(l - bx * B + 1, r - bx * B + 1)); 
    }
    inline void trim() noexcept
    {
        b.back() &= prefix(n % B == 0 ? B : n % B);
    }
 
//main
public:
    int n, m;
    std::vector<T> b;
 
    BitsetChan(int n) : BitsetChan(n, false) {};
    BitsetChan(int n, bool init) : n(n), m((n + B - 1)/B), b(m, init ? ~T(0) : T(0)) 
    {
        trim();
    };
 
    inline void Set(int i, bool val) noexcept
    {
        assert(0 <= i and i < n);
        if(val)
            b[i/B] |= (T(1) << (i % B));
        else
            b[i/B] &= ~(T(1) << (i % B));
    }
 
    inline bool Get(int i) const noexcept
    {
        assert(0 <= i and i < n);
        return (b[i/B] & (T(1) << (i % B))) != 0;
    }
 
    void Reset() noexcept
    {
        std::fill(b.begin(), b.end(), T(0));
    }
 
    //bitwise operations
    void operator &= (const BitsetChan &other)
    {
        for(int i = 0; i < std::min(m, other.m); i ++)
            b[i] &= other.b[i];
        if(m > other.m)
            std::fill(b.begin() + other.m, b.begin() + m, T(0));
        // trim();
    }
 
    void operator |= (const BitsetChan &other)
    {
        for(int i = 0; i < std::min(m, other.m); i ++)
            b[i] |= other.b[i];
        trim();
    }
 
    void operator ^= (const BitsetChan &other)
    {
        for(int i = 0; i < std::min(m, other.m); i ++)
            b[i] ^= other.b[i];
        trim();
    }
 
    void operator <<= (int x)
    {
        if(x == 0)
            return;
 
        if(x >= n)
        {
            Reset();
            return;
        }
 
        const int s = x/B, d = x % B, r = B - d;
 
        if(d > 0)
        {
            for(int i = m - 1 - s; i > 0; i --)
                b[i + s] = (b[i] << d) | (b[i - 1] >> r);
            b[s] = b[0] << d;
        }
        else
        {
            for(int i = m - 1 - s; i > 0; i --)
                b[i + s] = b[i];
            b[s] = b[0];
        }
 
        std::fill(b.begin(), b.begin() + s, T(0));
 
        trim();
    }
 
    void operator >>= (int x)
    {
        if(x == 0)
            return;
     
        if(x >= n)
        {
            Reset();
            return;
        }
 
        const int s = x/B, d = x % B, l = B - d;
 
        if(d > 0)
        {
            for(int i = s; i < m - 1; i ++)
                b[i - s] = (b[i] >> d) | (b[i + 1] << l); 
            b[m - 1 - s] = b[m - 1] >> d;
        }
        else
            for(int i = s; i < m; i ++)
                b[i - s] = b[i];
 
        std::fill(b.begin() + m - s, b.end(), T(0));        
 
        // trim();
    }
 
    bool operator == (const BitsetChan &other)
    {
        return ((n == other.n) and b == other.b); 
    }
 
    bool operator != (const BitsetChan &other)
    {
        return !(*this == other);
    }
 
    //extended
    BitsetChan operator & (const BitsetChan &other)
    {
        BitsetChan result(*this);
        result &= other;
        return result;
    }
 
    BitsetChan operator | (const BitsetChan &other)
    {
        BitsetChan result(*this);
        result |= other;
        return result;
    }
 
    BitsetChan operator ^ (const BitsetChan &other)
    {
        BitsetChan result(*this);
        result ^= other;
        return result;
    }
 
    BitsetChan operator >> (int x)
    {
        BitsetChan result(*this);
        result >>= x;
        return result;
    }
 
    BitsetChan operator << (int x)
    {
        BitsetChan result(*this);
        result <<= x;
        return result;
    }
 
    BitsetChan operator ~()
    {
        BitsetChan result(*this);
        for(auto &v : result)
            v = ~v;
        result.trim();
        return result;
    }
 
    //custom operations
    int Count() const noexcept
    {
        return std::accumulate(b.begin(), b.end(), 0, [](int sum, T value) { return sum + popcnt(value); });
    }
     
    int FindFirst()
    {
        int pos = -1;

        for(int bi = 0; bi < m; bi ++)
        {
            if(b[bi] == T(0))
                continue;
            
            pos = __builtin_ctzll(b[bi]) + bi * B;
            break;
        }

        return pos;
    }

    int FindLast()
    {
        int pos = -1;

        for(int bi = m - 1; bi >= 0; bi --)
        {
            if(b[bi] == T(0))
                continue;
            
            pos = B - __builtin_clzll(b[bi]) - 1 + bi * B;
            break;
        }

        return pos;
    }

    void RangeProcess(int l, int r, auto block_brute, auto block_quick)
    {
        assert(0 <= l and l <= r and r < n);
 
        int bl = block_id(l), br = block_id(r);
 
        if(bl == br)
            block_brute(l, r);
        else
        {
            block_brute(l, (bl + 1) * B - 1);
            for(int bi = bl + 1; bi < br; bi ++)
                block_quick(bi);
            block_brute(br * B, r);
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
 
    int Count(int l, int r)
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
 
    int FindFirst (int l, int r)
    {
        int pos = -1;
        auto block_brute = [&](int l, int r) -> void
        {
            for(int i = l; i <= r and pos == -1; i ++)
                if(Get(i))
                    pos = i;    
        };
        auto block_quick = [&](int bi) -> void
        {
            if(b[bi] == T(0) or pos != -1)
                return;
 
            pos = __builtin_ctzll(b[bi]) + bi * B;
        };
 
        RangeProcess(l, r, block_brute, block_quick);
        return pos;
    }
    
    int FindLast(int l, int r)
    {
        int pos = -1;
        auto block_brute = [&](int l, int r) -> void
        {
            for(int i = l; i <= r; i ++)
                if(Get(i))
                    pos = i;    
        };
        auto block_quick = [&](int bi) -> void
        {
            if(b[bi] == T(0))
                return;
 
            pos = B - __builtin_clzll(b[bi]) - 1 + bi * B;
        };
 
        RangeProcess(l, r, block_brute, block_quick);
        return pos;
    }
};
template <typename T, const int B>
std::ostream &operator<<(std::ostream &os, const BitsetChan<T, B> &bitset)
{
    for (int i = bitset.m - 1; i >= 0; --i)
        os << std::bitset<B>(bitset.b[i]);
    os << '\n';
    return os;
}
 
using BitsetChan64 = BitsetChan<uint64_t, 64>;
