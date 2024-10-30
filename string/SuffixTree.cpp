class SuffixTreeChan
{
    /*
    tc: O(n)
    mc: O(n)

    info:
        0 indexed
        pass it 0 indexed suffix array and lcp array
        constructs suffix tree with the following properties:
            1. node i (< n) represents suffix i (length = n - 1)
            2. for every suffix there exists node in the suffix tree (even if it has only child)
            3. if u > v, then u cannot be in the subtree of v
            4. adj[u] contains downward edges from node u, sorted lexicography in order of strings on their labels
            5. `root` is root node
            6. dep[u] contains length of path from root to node u, used for edge labels
            7. rep[u] = some i such that leaf for suffix i is in subtree of u 
            8. m is the total number of nodes in the suffix tree (so m - n is the number of new internal nodes)

        warning:
            sa gets modified so dont pass by reference
    */
public:
    int n, m, root;
    vector<int> dep, rep;
    vector<vector<int>> adj;

    SuffixTreeChan(int n, vector<int> sa, vector<int> lcp) : 
    n(n), m(n), dep(2 * n + 1), rep(2 * n + 1), adj(2 * n + 1)
    {
        vector<int> l(n), r(n);
        for(int i = 0; i < n; i ++) 
            l[i] = r[i] = rep[i] = i, dep[i] = n - i;
        
        vector<vector<int>> q(n);
        for(int i = 0; i < n - 1; i ++)
            q[lcp[i]].push_back(i);
        
        for(int d = n - 1; d >= 0; d --)
            for(auto i : q[d])
            {
                int u;
                if(dep[sa[i]] == d)
                    u = sa[i], adj[u].push_back(sa[i + 1]);
                else if(dep[sa[i + 1]] == d)
                    u = sa[i + 1], adj[u].push_back(sa[i]);
                else
                    u = m ++, rep[u] = rep[sa[i]], adj[u] = {sa[i], sa[i + 1]};
                
                dep[u] = d;
                l[r[i + 1]] = l[i], r[l[i]] = r[i + 1];
                sa[l[i]] = sa[r[i + 1]] = u;
            }

        root = sa[0];
        if(dep[root] != 0)
            root = m ++, dep[root] = 0, adj[root] = {sa[0]};
    };
};