template<typename T, const int B>
class Bitset
{
public:
    int n, m;
    vector<T> b;

    Bitset(int n) : Bitset(n, false) {};
    Bitset(int n, bool init) : n(n), m((n + B - 1)/B), b(m, init ? ~T(0) : T(0)) 
    {
        if(init)
        {
            int ls = n % B;
            // for(int i = B - 1; i >= B - ls; i --)
                // b[m - 1][i] &= ~(T(1) << i);
        }
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
        for(int i = 0; i < min(m, other.m); i ++)
            result.b[i] = b[i] & other.b[i];
        return result;
    }

    void operator &= (const Bitset &other)
    {
        for(int i = 0; i < min(m, other.m); i ++)
            b[i] &= other.b[i];
        if(m > other.m)
            fill(b.begin() + other.m, b.begin() + m, T(0));
    }

    Bitset operator | (const Bitset &other)
    {
        Bitset result((n > other.n ? *this : other));
        const Bitset* overlap = (m > other.m ? &other : this);
        for(int i = 0; i < min(m, other.m); i ++)
            result.b[i] |= overlap->b[i];
        return result;
    }

    void operator |= (const Bitset &other)
    {
        for(int i = 0; i < min(m, other.m); i ++)
            b[i] |= other.b[i];
    }

    Bitset operator << (int x)
    {
        if(x == 0)
            return Bitset(*this);

        Bitset result(n);

        if(x >= n)
            return result;

        int s = x/B, d = x % B;

        if(s > 0)
        {
            for(int i = m - 1; i >= s; i --)
                b[i] = b[i - s];
            if(s != 0)
                fill(b.begin(), b.begin() + s, T(0));
        }
        if(d > 0)
        {
            b[m - 1] <<= d;
            for(int i = m - 2; i >= 0; i --)
            {
                b[i + 1] |= (b[i] >> (B - d));
                b[i] <<= d;
            }
        }

        return result;
    }

    Bitset operator >> (int x)
    {
        if(x == 0)
            return Bitset(*this);

        Bitset result(n);

        if(x >= n)
            return result;

        int s = x/B, d = x % B;

        if(s > 0)
        {
            for(int i = 0; i < m - s; i ++)
                b[i] = b[i + s];
            if(s != 0)
                fill(b.begin() + m - s, b.end(), T(0));
        }
        if(d > 0)
        {
            b[0] >>= d;
            for(int i = 1; i < m; i ++)
            {
                b[i - 1] |= (b[i] << (B - d));
                b[i] >>= d;
            }
        }
        return result;
    }

    void Show()
    {
        for(auto x : b)
        {
            string s = (bitset<B>(x).to_string());
            reverse(s.begin(), s.end());
            cerr << s;
        }
        cerr << endl;
    }
};

using Bitset8 = Bitset<uint8_t, 8>;
using Bitset32 = Bitset<uint32_t, 32>;
using Bitset64 = Bitset<uint64_t, 64>;
