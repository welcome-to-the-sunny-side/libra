template <typename T>
vector<int> manachers_chan(const T &s)
{
    /*
        tc: O(|s|)
        mc: O(|s|)

        info:
            0-indexed
            res[2 * i]  = odd RADIUS in position i
            res[2 * i + 1] = even RADIUS between positions i and i + 1
        
        ex:
            s = "abaa" -> res = {0, 0, 1, 0, 0, 1, 0};
        
        author notes:
            in other words, for every z from 0 to 2 * n - 2:
            calculate i = (z + 1) >> 1 and j = z >> 1
            now there is a palindrome from i - res[z] to j + res[z]
            (watch out for i > j and res[z] = 0)
    */
    int n = (int)s.size();
    if (n == 0)
        return vector<int>();
        
    vector<int> res(2 * n - 1, 0);
    int l = -1, r = -1;
    for (int z = 0; z < 2 * n - 1; z ++)
    {
        int i = (z + 1) >> 1, j = z >> 1;
        int p = (i >= r ? 0 : min(r - i, res[2 * (l + r) - z]));

        while (j + p + 1 < n and i - p - 1 >= 0 and (s[j + p + 1] == s[i - p - 1]))
            p ++;

        if (j + p > r)
            l = i - p, r = j + p;
        res[z] = p;
    }

    return res;
}