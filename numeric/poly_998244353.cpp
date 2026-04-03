namespace poly
{
    static_assert(MOD == 998244353, "duh");
    const int G = 3;    //primitive root 

    void ntt(vector<mint> &a, bool invert)
    {
        int n = int(a.size());

        static vector<int> rev;
        static vector<mint> roots{0, 1};

        if ((int)rev.size() != n)
        {
            int lg = __builtin_ctz(n);
            rev.assign(n, 0);
            for (int i = 0; i < n; i++)
                rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (lg - 1));
        }

        if ((int)roots.size() < n)
        {
            int k = __builtin_ctz((int)roots.size());
            roots.resize(n);
            while ((1 << k) < n)
            {
                mint e = mint(G).pow((MOD - 1) >> (k + 1));
                for (int i = 1 << (k - 1); i < (1 << k); i++)
                {
                    roots[2 * i] = roots[i];
                    roots[2 * i + 1] = roots[i] * e;
                }
                k++;
            }
        }

        for (int i = 0; i < n; i++)
            if (i < rev[i])
                swap(a[i], a[rev[i]]);

        for (int len = 1; len < n; len <<= 1)
        {
            for (int i = 0; i < n; i += 2 * len)
            {
                for (int j = 0; j < len; j++)
                {
                    mint u = a[i + j];
                    mint v = a[i + j + len] * roots[len + j];
                    a[i + j] = u + v;
                    a[i + j + len] = u - v;
                }
            }
        }

        if (invert)
        {
            reverse(a.begin() + 1, a.end());
            mint inv_n = mint(n).inv();
            for (mint &x : a)
                x *= inv_n;
        }
    }

    vector<mint> multiply(const vector<mint> &a, const vector<mint> &b)
    {
        if (a.empty() || b.empty())
            return {};

        int need = int(a.size() + b.size() - 1);
        int n = 1;
        while (n < need)
            n <<= 1;

        vector<mint> fa(a.begin(), a.end()), fb(b.begin(), b.end());
        fa.resize(n);
        fb.resize(n);

        ntt(fa, false);
        ntt(fb, false);

        for (int i = 0; i < n; i++)
            fa[i] *= fb[i];

        ntt(fa, true);
        fa.resize(need);
        return fa;
    }
}