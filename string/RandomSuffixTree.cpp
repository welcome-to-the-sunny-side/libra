template<typename C, typename L, typename G>
class RandomSuffixTree
{
    /*
    info: 
        - Generates suffix tree using the leader split technique for a set of strings numbered from 0 to n - 1
        - bool compare(int i, int j, int x) returns true if x-length prefix of strings i and j is equal, and false otherwise
        - int length(int i) returns length of ith string
        - auto get(int i, int x) returns the xth character of string i (1 based indexing)
    */
public:
    int n, root;
    vector<int> dep;
    vector<int> nid;
    vector<vector<int>> adj;
    vector<vector<int>> grp;    

    RandomSuffixTree(int n, C compare, L length, G get) : 
    n(n), dep(2 * n + 5), nid(2 * n + 5), adj(2 * n + 5), grp(2 * n + 5)
    {
        vector<int> alive(n);
        iota(alive.begin(), alive.end(), 0);

        Random rng;

        int timer = n - 1;

        auto construct = [&](vector<int> a, auto &&construct) -> int
        {
            assert(!a.empty());

            if(a.size() == 1)
                return a[0];
            
            int l = a[rng(a.size())];
            a.erase(find(a.begin(), a.end(), l));
            dis[l] = length(l);

            for(auto v : a)
            {
                dis[v] = 0;
                int s = 1, e = min(dis[l], length(v));
                while(s <= e)
                {
                    int mid = (s + e)/2;
                    if(compare(l, v, mid))
                        s = mid + 1, dis[v] = mid;
                    else
                        e = mid - 1;
                }
            }

            sort(a.begin(), a.end(), [&](int i, int j) {return dis[i] > dis[j];});
            a.insert(a.begin(), l);

            vector<int> nodes;

            while(!a.empty())
            {
                int len = dis[a.back()];
                vector<int> split;
                vector<int> common;

                while(!a.empty() and dis[a.back()] == len)
                {
                    if(length(a.back()) == len)
                        common.push_back(a.back());
                    else
                        split.push_back(a.back());
                    a.pop_back();
                }

                int node = -1;

                if(common.empty())
                    node = ++ timer;
                else
                {
                    node = common.back();
                    grp[node] = common;
                    for(auto x : grp[node])
                        nid[x] = node;
                }

                dep[node] = len;

                sort(split.begin(), split.end(), [&](int i, int j) {return get(i, len + 1) < get(j, len + 1);});
                for(int i = 0; i < split.size();)
                {
                    int j = i;
                    while(j < split.size() and get(split[i], len + 1) == get(split[j], len + 1))
                        ++ j;
                    
                    vector<int> hsplit;
                    for(int k = i; k < j; k ++)
                        hsplit.push_back(split[k]);
                    
                    int child = construct(hsplit, construct);
                    adj[node].push_back(child);

                    i = j;
                }
              
                nodes.push_back(node);
            }

            for(int i = 0; i < nodes.size() - 1; i ++)
            {
                adj[nodes[i]].push_back(nodes[i + 1]);
                //push it in while maintaining sorted ordere
            }

            return nodes.front();
        };
        int root = construct(alive, construct);

        if(dep[root] != 0)
        {
            int new_root = ++ timer;
            adj[new_root].push_back(root);
            root = new_root;
        }
    };
};