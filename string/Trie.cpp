template<const int C>
class static_node_chan
{
public:
    int sub = 0;
    int leaf = 0;
    array<int, C> next;
};
class dynamic_node_chan
{
public: 
    int sub = 0;
    int leaf = 0;
    map<int, int> next;
};

template<typename S, typename T, typename I>
class trie_chan
{
public:
    int r = 0, p = 0;
    vector<T> t;
    I ind;

    trie_chan(int n, I ind) : t(n), ind(ind) {};

    void insert(const vector<S> &s)
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
