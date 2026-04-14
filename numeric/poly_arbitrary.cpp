
template <int MOD>
struct arbitrary_poly
{
    using mint = modular_int<MOD>;

    static constexpr int P1 = 998244353;
    static constexpr int P2 = 1004535809;
    static constexpr int P3 = 469762049;

    using NTT1 = ntt_poly<P1, 3>;
    using NTT2 = ntt_poly<P2, 3>;
    using NTT3 = ntt_poly<P3, 3>;

    using m1 = modular_int<P1>;
    using m2 = modular_int<P2>;
    using m3 = modular_int<P3>;

    static mint garner3(int x1, int x2, int x3)
    {
        static const m2 inv_p1_mod_p2 = m2(P1).inv();
        static const m3 inv_p1p2_mod_p3 = (m3(P1) * m3(P2)).inv();

        int t1 = x1;
        int t2 = int(((m2(x2) - m2((int)t1)) * inv_p1_mod_p2).val);
        int t3 = int(((m3(x3) - m3((int)t1) - m3(P1) * m3((int)t2)) * inv_p1p2_mod_p3).val);

        mint res = mint(t1);
        res += mint(P1) * mint(t2);
        res += mint(1LL * P1 * P2 % MOD) * mint(t3);
        return res;
    }
    static vector<mint> multiply(const vector<mint> &a, const vector<mint> &b)
    {
        if (a.empty() || b.empty())
            return {};

        vector<m1> a1(a.size()), b1(b.size());
        vector<m2> a2(a.size()), b2(b.size());
        vector<m3> a3(a.size()), b3(b.size());

        for (int i = 0; i < (int)a.size(); i++)
        {
            int x = a[i].val;
            a1[i] = x;
            a2[i] = x;
            a3[i] = x;
        }
        for (int i = 0; i < (int)b.size(); i++)
        {
            int x = b[i].val;
            b1[i] = x;
            b2[i] = x;
            b3[i] = x;
        }

        auto c1 = NTT1::multiply(a1, b1);
        auto c2 = NTT2::multiply(a2, b2);
        auto c3 = NTT3::multiply(a3, b3);

        int n = (int)c1.size();
        vector<mint> res(n);
        for (int i = 0; i < n; i++)
            res[i] = garner3(c1[i].val, c2[i].val, c3[i].val);

        return res;
    }
};