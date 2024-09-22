class TrieNode
{
public: 
    int leaf;
    map<int, int> next;
    TrieNode() : leaf(0) {};
};
template<typename S, typename T, typename C>
class Trie
{
public:
    int r = 0, p = 0;
    vector<T> t;
    C c;

    Trie(int n, C c) : t(n), c(c) {};

    void Insert(const vector<S> &s)
    {
        int v = r;
        for(auto x : s)
        {
            int f = c(x);
        
            if(t[v].next[f] == 0)
                t[v].next[f] = ++ p;
            v = t[v].next[f];
        }
        ++ t[v].leaf;
    }
};
