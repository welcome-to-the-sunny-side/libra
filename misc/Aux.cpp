namespace aux
{
    namespace bitwise
    {
        int64_t p(int i)
        {
            return 1LL << i;
        }
    }
    namespace math
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
}