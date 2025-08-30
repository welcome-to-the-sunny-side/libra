template<typename T, typename C>
vector<vector<int>> cartesian_tree(const vector<T> &p, C c = C())
{
    int n = p.size();

    vector<int> l(n, -1);

    vector<int> stk;
    for(int i = n - 1; i >= 0; i --)
    {
        while(!stk.empty() and c(p[i], p[stk.back()]))
            l[stk.back()] = i, stk.pop_back();
        stk.push_back(i);
    }
    stk.clear();

    vector<int> r(n, n);
    for(int i = 0; i < n; i ++)
    {
        while(!stk.empty() and c(p[i], p[stk.back()]))
            r[stk.back()] = i, stk.pop_back();
        stk.push_back(i);
    }
    stk.clear();

    vector<vector<int>> adj(n);
    for(int i = 0; i < n; i ++)
    {
        int parent = -1;
        if(l[i] != -1)
            parent = l[i];
        
        if(r[i] != -1)
            if(parent == -1 or !c(p[parent], p[r[i]]))
                parent = r[i];
        
        if(parent != -1)
            adj[parent].push_back(i);
    }

    return adj;
}