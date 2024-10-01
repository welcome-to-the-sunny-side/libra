//slightly slower than std::bitset, but dynamic
//warning: not completely stress tested

template<typename T, const int B>
class BitsetChan
{
public:
    int n, m;
    vector<T> b;

    BitsetChan(int n) : BitsetChan(n, false) {};
    BitsetChan(int n, bool init) : n(n), m((n + B - 1)/B), b(m, init ? ~T(0) : T(0)) 
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

    BitsetChan operator & (const BitsetChan &other)
    {
        BitsetChan result(max(n, other.n), false);
        for(int i = 0; i < min(m, other.m); i ++)
            result.b[i] = b[i] & other.b[i];
        return result;
    }

    void operator &= (const BitsetChan &other)
    {
        for(int i = 0; i < min(m, other.m); i ++)
            b[i] &= other.b[i];
        if(m > other.m)
            fill(b.begin() + other.m, b.begin() + m, T(0));
    }

    BitsetChan operator | (const BitsetChan &other)
    {
        BitsetChan result((n > other.n ? *this : other));
        const BitsetChan* overlap = (m > other.m ? &other : this);
        for(int i = 0; i < min(m, other.m); i ++)
            result.b[i] |= overlap->b[i];
        return result;
    }

    void operator |= (const BitsetChan &other)
    {
        for(int i = 0; i < min(m, other.m); i ++)
            b[i] |= other.b[i];
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
                b[i + 1] |= (result.b[i] >> (B - d));
                b[i] <<= d;
            }
        }

        return result;
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
    }

    void Show()
    {
        for(int i = m - 1; i >= 0; i --)
            cerr << bitset<B> (b[i]);
        cerr << endl;
    }
};

using Bitset8 = BitsetChan<uint8_t, 8>;
using Bitset32 = BitsetChan<uint32_t, 32>;
using Bitset64 = BitsetChan<uint64_t, 64>;