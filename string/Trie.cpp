template<int C>
class TrieNode
{
public:
    int leaf;
    vector<int> next;

    TrieNode() : leaf(0), next(C, -1) {}; 
};
template<typename S, typename T>
class Trie
{
public:
    int n;
    int r = 0, p = 0;
    S base;
    vector<T> t;

    Trie(int n, S base) : t(n), base(base) {};

    void insert(const vector<S> &s)
    {
        int v = r;
        for(auto x : s)
        {
            int f = x - base;
            if(t[v].next[f] == -1)
                t[v].next[f] = ++ p;
            v = t[v].next[f];
        }
        ++ t[v].leaf;
    }
};