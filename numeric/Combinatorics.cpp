template<typename T, const int P>
class combinatorics
{
public:
    int n;
    vector<T> inv, fac, ifac, pw;
    combinatorics (int n) : n(n), inv(n), fac(n), ifac(n), pw(n)
    {
        fac[0] = inv[0] = ifac[0] = pw[0] = T(1);
 
        for(int i = 1; i <= n; i ++)
            inv[i] = T(1)/T(i), fac[i] = fac[i - 1] * T(i), ifac[i] = ifac[i - 1] * inv[i], pw[i] = pw[i - 1] * T(P);
    }

    T ncr(int n, int r)
    {
        if(n < r or r < 0)
            return 0;
        return fac[n] * ifac[r] * ifac[n - r];
    }
};