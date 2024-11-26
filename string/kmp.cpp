template <typename T>
vector<int> kmp_table(const T &s)
{
    /*
        tc: O(|s|)
        mc: O(|s|)

        info:
            p[i] = longest suffix of substring s[0, 1, .. i] which is also a suffix 
    */
    int n = s.size();
    vector<int> p(n, 0);
    int k = 0;
    for (int i = 1; i < n; i ++)
    {
        while (k > 0 && !(s[i] == s[k]))
            k = p[k - 1];
        if (s[i] == s[k])
            k ++;
        p[i] = k;
    }
    return p;
}

template <typename T>
vector<int> kmp_search(const T &s, const T &w, const vector<int> &p)
{
    /*
        tc: O(|s| + |w|)
        mc: O(|w|)

        info:
            finds 0-indexed positions of occurences of s in w
            p is kmp table of s
    */
    int n = s.size(), m = w.size();
    assert(n >= 1 && (int)p.size() == n);
    vector<int> res;
    int k = 0;
    for (int i = 0; i < m; i ++)
    {
        while (k > 0 && (k == n || !(w[i] == s[k])))
            k = p[k - 1];
        if (w[i] == s[k])
            k ++;
        if (k == n)
            res.push_back(i - n + 1);
    }
    return res;
}