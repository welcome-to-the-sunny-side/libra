template<const int B>
class block_decomposition_chan
{
    /*
        info:
            - 0 indexed
            - B is the block size
            - for several queries, using process() will be rather awkward, so just implement your own external function  
    */
public:
    inline int ceil_div(int x, int y)
    {
        return (x + y - 1)/y;
    }
    inline int block_id(int i)
    {
        return i/B;
    }
    inline int lb(int bid)
    {
        return bid * B;
    }
    inline int rb(int bid)
    {
        return min(n - 1, (bid + 1) * B - 1);
    }

public:
    int n;

    block_decomposition_chan(int n) : n(n)
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
