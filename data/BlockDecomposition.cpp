class ElementChan
{
public:
};
class BlockChan
{
public:
};

template<typename E, typename T, const int B>
class BlockDecompositionChan
{
public:
    int ceil_div(int x, int y)
    {
        return (x + y - 1)/y;
    }
    int block_id(int i)
    {
        return i/B;
    }
    int lb(int bid)
    {
        return bid * B;
    }
    int rb(int bid)
    {
        return min(n, (bid + 1) * B - 1);
    }

public:
    int n;
    vector<E> element;
    vector<T> block;

    BlockDecompositionChan(int n, vector<E> a, vector<T> b) : n(n), element(a), block(b)
    {

    };

    void Process(int l, int r, auto block_brute, auto block_quick)
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