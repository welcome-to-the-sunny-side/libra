class TrieNode
{
public: 
    // int sub = 0;
    int leaf = 0;

    // const int C = 26;
    // array<int, 26> next; 
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
            int f = ind(x);
        
            if(t[v].next[f] == 0)
                t[v].next[f] = ++ p;
            v = t[v].next[f];
        }
        ++ t[v].leaf;
    }
};
