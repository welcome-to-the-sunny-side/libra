namespace Aux
{
    namespace Bitwise
    {
        int64_t p(int i)
        {
            return 1LL << i;
        }
    }
    namespace Math
    {
        int ceil_div(int x, int y)
        {
            return (x + y - 1)/y;
        }
        int64_t ceil_div(int64_t x, int64_t y)
        {
            return (x + y - 1)/y;
        }
    }
    namespace String
    {
        vector<int> convert(string s, char base)
        {
            vector<int> a(s.length());
            for(int i = 0; i < s.length(); i ++)
                a[i] = s[i] - base;
            return a;
        }
    }
}