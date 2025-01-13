class element_chan
{
public:
};
class block_chan
{
public:
};

template<typename E, typename T, const int B>
class block_decomposition_chan
{
public:
    static int ceil_div(int x, int y)
    {
        return (x + y - 1)/y;
    }
    static int block_id(int i)
    {
        return i/B;
    }
    static int lb(int bid)
    {
        return bid * B;
    }
    static int rb(int bid)
    {
        return min(n, (bid + 1) * B - 1);
    }

public:
    int n;
    vector<E> element;
    vector<T> block;

    block_decomposition_chan(int n, vector<E> a, vector<T> b) : n(n), element(a), block(b)
    {
    };

    void process(int l, int r, auto block_brute, auto block_quick)
    {
        assert(1 <= l and l <= r and r <= n);
        int bl = block_id(l), br = block_id(r);
        if(bl == br)
            block_brute(l, r);
        else
        {
            block_brute(l, rb(bl));
            for(int b = bl + 1; b < br; b ++)
                block_quick(b);
            block_brute(lb(br), r);
        }
    }
};