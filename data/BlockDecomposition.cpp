class Element
{
public:

};
class Block
{
public:

};

template<typename E, typename T, const int N, const int B>
class BlockChan
{
private:
    int ceil_div(int x, int y)
    {
        return (x + y - 1)/y;
    }
    int block_id(int i)
    {
        return ceil_div(i, B);
    }
    int lb(int block_id)
    {
        return (block_id - 1) * B + 1;
    }
    int rb(int block_id)
    {
        return min(n, block_id * B);
    }

public:
    int n;
    const int X;
    vector<E> element;
    vector<T> block;

    BlockChan(int n, vector<E> a, vector<T> b) : n(n), element(a), block(b), X(N/B+5) 
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