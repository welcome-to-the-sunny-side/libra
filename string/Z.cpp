template <typename T>
vector<int> ZChan(const T &s)
{
    int n = s.size();
    vector<int> z(n, n);
    int l = 0, r = 0;
    for (int i = 1; i < n; i++)
    {
        z[i] = (i > r ? 0 : min(r - i + 1, z[i - l]));
        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
            z[i]++;

        if (i + z[i] - 1 > r)
            l = i, r = i + z[i] - 1;
    }
    return z;
}