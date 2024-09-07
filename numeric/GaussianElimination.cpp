struct GaussianElimination
{
    int d, sz;
    vector<int> basis;

    void init(int d)
    {
        this->d = d;
        sz = 0;
        basis.assign(d, 0);
    }

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
        
    }
};
