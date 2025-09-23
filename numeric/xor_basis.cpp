template<typename T>
class xor_basis_chan
{
    /*
        - use an unsigned type for T
        - msb xor basis
        - d is the maximum mask size
        - sz is the size of the basis 
        - pcnt[i] = the number of things in our basis with msb <= i
    */
private:
    T pw(int i)
    {
        return T(1) << i;
    }
    bool on(T x, int i)
    {
        return x & pw(i);
    }
public:
    int d, sz;
    vector<T> basis;
    vector<int> pcnt;

    xor_basis_chan() : xor_basis_chan(0) {};
    xor_basis_chan(int _d) : d(_d), sz(0), basis(d), pcnt(d) {};

    bool insert(T x)
    {
        for(int i = d - 1; i >= 0; i --)
            if(on(x, i))
            {
                if(basis[i] == 0)
                {
                    basis[i] = x, ++ sz;
                    for(int j = d - 1; j >= i; j --)    
                        pcnt[j] ++;
                    return true;
                }
                x ^= basis[i];                
            }
        return false;
    }

    //the number of elements strictly smaller than x
    T order_of_element(T x)
    {
        T cnt = 0;
        T val = 0;

        //val < x
        for(int i = d - 1; i >= 0; i --)
        {
            bool xon = on(x, i), valon = on(val, i);
            if(basis[i])
            {
                if(xon and valon)
                {
                    //take this and make val smaller here
                    cnt += (i == 0 ? T(1) : pw(pcnt[i - 1]));
                }
                else if(xon and !valon)
                {
                    //dont take this and make val smaller here
                    cnt += (i == 0 ? T(1) : pw(pcnt[i - 1]));
                    val ^= basis[i];
                }
                else if(!xon and valon)
                {
                    //must take here
                    val ^= basis[i];
                }
                else if(!xon and !valon)
                {
                    //do nothing here
                }
            }
            else
            {
                if(xon == valon)
                    continue;
    
                if(xon)
                    cnt += (i == 0 ? T(1) : pw(pcnt[i - 1]));

                break;
            }
        }

        return cnt;
    }

    //element with exactly idx elements smaller than it
    T find_by_order(T idx)
    {
        assert(idx < (T(1) << sz));

        T val = 0;
        for(int i = d - 1; i >= 0; i --)
            if(basis[i])
            {
                T either = (i == 0 ? T(1) : pw(pcnt[i - 1]));
                bool valon = on(val, i);
                if(either <= idx)
                {
                    idx -= either;
                    if(!valon)
                        val ^= basis[i];
                }
                else
                {
                    if(valon)
                        val ^= basis[i];
                }
            }
        return val;
    }

    //maximum element
    T max()
    {
        T val = 0;
        for(int i = d - 1; i >= 0; i --)
            if(basis[i] and !on(val, i))
                val ^= basis[i];
        return val;
    }
};
