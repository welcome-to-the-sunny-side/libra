template<typename T>
class Combinatorics
{
    static constexpr int P = 33;
public:
    int n;
    vector<T> inv, fac, ifac, pw;
    Combinatorics (int n) : n(n), inv(n), fac(n), ifac(n), pw(n)
    {
        fac[0] = inv[0] = ifac[0] = pw[0] = 1;
 
        for(int i = 1; i <= n; i ++)
            inv[i] = 1/(T)i, fac[i] = fac[i - 1] * i, ifac[i] = ifac[i - 1] * inv[i], pw[i] = pw[i - 1] * P;
    }

    T ncr(int n, int r)
    {
        if(n < r or r < 0)
            return 0;
        return fac[n] * ifac[r] * ifac[n - r];
    }
};