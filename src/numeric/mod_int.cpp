template <const int &MOD>
struct modular_int
{
    int val;

    modular_int(int64_t v = 0)
    {
        if (v < 0)
            v = v % MOD + MOD;
        if (v >= MOD)
            v %= MOD;
        val = int(v);
    }

    modular_int(uint64_t v)
    {
        if (v >= MOD)
            v %= MOD;
        val = int(v);
    }

    modular_int(int v) : modular_int(int64_t(v)) {}
    modular_int(unsigned v) : modular_int(uint64_t(v)) {}

    explicit operator int() const { return val; }
    explicit operator unsigned() const { return val; }
    explicit operator int64_t() const { return val; }
    explicit operator uint64_t() const { return val; }
    explicit operator double() const { return val; }
    explicit operator long double() const { return val; }

    modular_int &operator+=(const modular_int &other)
    {
        val -= MOD - other.val;
        if (val < 0)
            val += MOD;
        return *this;
    }

    modular_int &operator-=(const modular_int &other)
    {
        val -= other.val;
        if (val < 0)
            val += MOD;
        return *this;
    }

    static unsigned fast_mod(uint64_t x, unsigned m = MOD)
    {
#if !defined(_WIN32) || defined(_WIN64)
        return unsigned(x % m);
#endif
        // Optimized mod for Codeforces 32-bit machines.
        // x must be less than 2^32 * m for this to work, so that x / m fits in an unsigned 32-bit int.
        unsigned x_high = unsigned(x >> 32), x_low = unsigned(x);
        unsigned quot, rem;
        asm("divl %4\n"
            : "=a"(quot), "=d"(rem)
            : "d"(x_high), "a"(x_low), "r"(m));
        return rem;
    }

    modular_int &operator*=(const modular_int &other)
    {
        val = fast_mod(uint64_t(val) * other.val);
        return *this;
    }

    modular_int &operator/=(const modular_int &other)
    {
        return *this *= other.inv();
    }

    friend modular_int operator+(const modular_int &a, const modular_int &b) { return modular_int(a) += b; }
    friend modular_int operator-(const modular_int &a, const modular_int &b) { return modular_int(a) -= b; }
    friend modular_int operator*(const modular_int &a, const modular_int &b) { return modular_int(a) *= b; }
    friend modular_int operator/(const modular_int &a, const modular_int &b) { return modular_int(a) /= b; }

    modular_int &operator++()
    {
        val = val == MOD - 1 ? 0 : val + 1;
        return *this;
    }

    modular_int &operator--()
    {
        val = val == 0 ? MOD - 1 : val - 1;
        return *this;
    }

    modular_int operator++(int)
    {
        modular_int before = *this;
        ++*this;
        return before;
    }
    modular_int operator--(int)
    {
        modular_int before = *this;
        --*this;
        return before;
    }

    modular_int operator-() const
    {
        return val == 0 ? 0 : MOD - val;
    }

    friend bool operator==(const modular_int &a, const modular_int &b) { return a.val == b.val; }
    friend bool operator!=(const modular_int &a, const modular_int &b) { return a.val != b.val; }
    friend bool operator<(const modular_int &a, const modular_int &b) { return a.val < b.val; }
    friend bool operator>(const modular_int &a, const modular_int &b) { return a.val > b.val; }
    friend bool operator<=(const modular_int &a, const modular_int &b) { return a.val <= b.val; }
    friend bool operator>=(const modular_int &a, const modular_int &b) { return a.val >= b.val; }

    static const int SAVE_INV = int(1e6) + 5;
    static modular_int save_inv[SAVE_INV];

    static void prepare_inv()
    {
        // Ensures that MOD is prime, which is necessary for the inverse algorithm below.
        for (int64_t p = 2; p * p <= MOD; p += p % 2 + 1)
            assert(MOD % p != 0);

        save_inv[0] = 0;
        save_inv[1] = 1;

        for (int i = 2; i < SAVE_INV; i++)
            save_inv[i] = save_inv[MOD % i] * (MOD - MOD / i);
    }

    modular_int inv() const
    {
        if (save_inv[1] == 0)
            prepare_inv();

        if (val < SAVE_INV)
            return save_inv[val];

        modular_int product = 1;
        int v = val;

        do
        {
            product *= MOD - MOD / v;
            v = MOD % v;
        } while (v >= SAVE_INV);

        return product * save_inv[v];
    }

    modular_int pow(int64_t p) const
    {
        if (p < 0)
            return inv().pow(-p);

        modular_int a = *this, result = 1;

        while (p > 0)
        {
            if (p & 1)
                result *= a;

            p >>= 1;

            if (p > 0)
                a *= a;
        }

        return result;
    }

    friend ostream &operator<<(ostream &os, const modular_int &m)
    {
        return os << m.val;
    }
};
template <const int &MOD>
modular_int<MOD> modular_int<MOD>::save_inv[modular_int<MOD>::SAVE_INV];
const int MOD = 998244353;
using mint = modular_int<MOD>;
void __print(mint x) { cerr << x; }