class gauss_chan
{
public:
    int d, sz;
    vector<int> basis;

    gauss_chan (int d) : d(d), sz(0), basis(d) {};

    void insert(int mask)
    {
        for(int i = 0; i < d; i ++) if(mask & (1 << i))
        {
            if(basis[i] == 0)
            {
                basis[i] = mask;
                ++ sz;
                break;
            }
            mask ^= basis[i];
        }
    }

    void query()
    {
        //
    }
};
