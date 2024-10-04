template<const int C>
class StaticNodeChan
{
public:
    int sub = 0;
    int leaf = 0;
    array<int, C> next;
};
class DynamicNodeChan
{
public: 
    int sub = 0;
    int leaf = 0;
    map<int, int> next;
};

template<typename S, typename T, typename I>
class TrieChan
{
public:
    int r = 0, p = 0;
    vector<T> t;
    I ind;

    TrieChan(int n, I ind) : t(n), ind(ind) {};

    void Insert(const vector<S> &s)
    {
        int v = r;
        for(auto x : s)
        {
            ++ t[v].sub;

            int f = ind(x);
         
            if(t[v].next[f] == 0)
                t[v].next[f] = ++ p;
            v = t[v].next[f];
        }
        ++ t[v].sub, ++ t[v].leaf;
    }
};
