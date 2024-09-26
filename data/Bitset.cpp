template<typename T, const int B>
class Bitset
{
public:
    int n, m;
    vector<T> b;

    Bitset(int n) : Bitset(n, false) {};
    Bitset(int n, bool init) : n(n), m((n + B - 1)/B), b(m, init ? ~T(0) : T(0)) 
    {
        if(init and n % B)
            b.back() &= ((T(1) << (n % B)) - T(1));
    };

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

    Bitset operator & (const Bitset &other)
    {
        Bitset result(max(n, other.n), false);
        for(size_t i = 0; i < min(m, other.m); i ++)
            result.b[i] = b[i] & other.b[i];
        return result;
    }

    void operator &= (const Bitset &other)
    {
        for(size_t i = 0; i < min(m, other.m); i ++)
            b[i] &= other.b[i];
        if(m > other.m)
            fill(b.begin() + other.m, b.begin() + m, T(0));
    }

    Bitset operator | (const Bitset &other)
    {
        Bitset result((n > other.n ? *this : other));
        const Bitset* overlap = (m > other.m ? &other : this);
        for(size_t i = 0; i < min(m, other.m); i ++)
            result.b[i] |= overlap->b[i];
        return result;
    }

    void operator |= (const Bitset &other)
    {
        for(size_t i = 0; i < min(m, other.m); i ++)
            b[i] |= other.b[i];
    }

    Bitset operator << (int x)
    {
        if(x == 0)
            return *this;

        Bitset result(n);

        if(x >= n)
            return result;

        int s = x/B, d = x % B;

        copy(b.begin(), b.begin() + m - s, result.b.begin() + s);

        if(d > 0)
        {
            result.b[m - 1] <<= d;
            for(size_t i = m - 2; i >= 0; i --)
            {
                result.b[i + 1] |= (result.b[i] >> (B - d));
                result.b[i] <<= d;
            }
        }

        return result;
    }

    Bitset operator >> (int x)
    {
        if(x == 0)
            return *this;

        Bitset result(n);

        if(x >= n)
            return result;

        int s = x/B, d = x % B;

        copy(b.begin() + s, b.end(), result.b.begin());

        if(d > 0)
        {
            result.b[0] >>= d;
            for(size_t i = 1; i < m; i ++)
            {
                result.b[i - 1] |= (result.b[i] << (B - d));
                result.b[i] >>= d;
            }
        }
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

        if(s > 0)
            for(size_t i = 0; i < m - s; i ++)
                b[i] = b[i + s];

        if(d > 0)
        {
            b[0] >>= d;
            for(size_t i = 1; i < m; i ++)
            {
                b[i - 1] |= (b[i] << (B - d));
                b[i] >>= d;
            }
        }
    }

    void Show()
    {
        for(int i = m - 1; i >= 0; i --)
            cerr << bitset<B> (b[i]);
        cerr << endl;
    }
};

using Bitset8 = Bitset<uint8_t, 8>;
using Bitset32 = Bitset<uint32_t, 32>;
using Bitset64 = Bitset<uint64_t, 64>;