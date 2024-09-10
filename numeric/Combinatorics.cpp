template<typename T>
struct Combinatorics
{
    int n;
    vector<T> inv, fac, ifac;
    Combinatorics (int n)
    {
        this->n = n;

        inv.assign(n + 1, 0);
        fac.assign(n + 1, 0);
        ifac.assign(n + 1, 0); 

        fac[0] = inv[0] = ifac[0] = 1;
 
        for(int i = 1; i <= n; i ++)
            inv[i] = 1/(T)i, fac[i] = fac[i - 1] * i, ifac[i] = ifac[i - 1] * inv[i];
    }

    T ncr(int n, int r)
    {
        if(n < r or r < 0)
            return 0;
        return fac[n] * ifac[r] * ifac[n - r];
    }
};
const int NC = 3e5+5;
Combinatorics<mint> comb(NC);