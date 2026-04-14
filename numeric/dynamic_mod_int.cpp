template<int ID = 0>
struct dynamic_modular_int
{
    inline static int MOD = 1'000'000'007;

    static void set_mod(int m)
    {
        assert(m > 0);
        MOD = m;
    }
    
    int val;

    dynamic_modular_int(int64_t v = 0)
    {
        if (v < 0)
            v = v % MOD + MOD;
        if (v >= MOD)
            v %= MOD;
        val = int(v);
    }

    dynamic_modular_int(uint64_t v)
    {
        if (v >= MOD)
            v %= MOD;
        val = int(v);
    }

    dynamic_modular_int(int v) : dynamic_modular_int(int64_t(v)) {}
    dynamic_modular_int(unsigned v) : dynamic_modular_int(uint64_t(v)) {}

    explicit operator int() const { return val; }
    explicit operator unsigned() const { return val; }
    explicit operator int64_t() const { return val; }
    explicit operator uint64_t() const { return val; }
    explicit operator double() const { return val; }
    explicit operator long double() const { return val; }

    dynamic_modular_int &operator+=(const dynamic_modular_int &other)
    {
        val -= MOD - other.val;
        if (val < 0)
            val += MOD;
        return *this;
    }

    dynamic_modular_int &operator-=(const dynamic_modular_int &other)
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

    dynamic_modular_int &operator*=(const dynamic_modular_int &other)
    {
        val = fast_mod(uint64_t(val) * other.val);
        return *this;
    }

    dynamic_modular_int &operator/=(const dynamic_modular_int &other)
    {
        return *this *= other.inv();
    }

    friend dynamic_modular_int operator+(const dynamic_modular_int &a, const dynamic_modular_int &b) { return dynamic_modular_int(a) += b; }
    friend dynamic_modular_int operator-(const dynamic_modular_int &a, const dynamic_modular_int &b) { return dynamic_modular_int(a) -= b; }
    friend dynamic_modular_int operator*(const dynamic_modular_int &a, const dynamic_modular_int &b) { return dynamic_modular_int(a) *= b; }
    friend dynamic_modular_int operator/(const dynamic_modular_int &a, const dynamic_modular_int &b) { return dynamic_modular_int(a) /= b; }

    dynamic_modular_int &operator++()
    {
        val = val == MOD - 1 ? 0 : val + 1;
        return *this;
    }

    dynamic_modular_int &operator--()
    {
        val = val == 0 ? MOD - 1 : val - 1;
        return *this;
    }

    dynamic_modular_int operator++(int)
    {
        dynamic_modular_int before = *this;
        ++*this;
        return before;
    }
    dynamic_modular_int operator--(int)
    {
        dynamic_modular_int before = *this;
        --*this;
        return before;
    }

    dynamic_modular_int operator-() const
    {
        return val == 0 ? 0 : MOD - val;
    }

    friend bool operator==(const dynamic_modular_int &a, const dynamic_modular_int &b) { return a.val == b.val; }
    friend bool operator!=(const dynamic_modular_int &a, const dynamic_modular_int &b) { return a.val != b.val; }
    friend bool operator<(const dynamic_modular_int &a, const dynamic_modular_int &b) { return a.val < b.val; }
    friend bool operator>(const dynamic_modular_int &a, const dynamic_modular_int &b) { return a.val > b.val; }
    friend bool operator<=(const dynamic_modular_int &a, const dynamic_modular_int &b) { return a.val <= b.val; }
    friend bool operator>=(const dynamic_modular_int &a, const dynamic_modular_int &b) { return a.val >= b.val; }

    dynamic_modular_int inv() const
    {
        dynamic_modular_int product = 1;
        int v = val;

        do
        {
            product *= MOD - MOD / v;
            v = MOD % v;
        } while (v > 1);

        return product;
    }

    dynamic_modular_int pow(int64_t p) const
    {
        if (p < 0)
            return inv().pow(-p);

        dynamic_modular_int a = *this, result = 1;

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

    friend ostream &operator<<(ostream &os, const dynamic_modular_int &m)
    {
        return os << m.val;
    }
};
using dmint = dynamic_modular_int<0>;
void __print(dmint x) { cerr << x; }
