template <typename L>
vector<pair<int, int>> virtual_tree_chan(vector<int> key, const vector<int> &tin, const vector<int> &tout, L &lca)
{
    assert(!key.empty());
    sort(key.begin(), key.end(), [&](int a, int b)  { return tin[a] < tin[b]; });

    vector<int> l;
    for (int i = 0; i < key.size() - 1; i++)
        l.push_back(lca.LCA(key[i], key[i + 1]));
    
    for (auto p : l)
        key.push_back(p);
    sort(key.begin(), key.end(), [&](int a, int b)  { return tin[a] < tin[b]; });
    key.erase(unique(key.begin(), key.end()), key.end());
    reverse(key.begin(), key.end());

    // stk contains path from root to current node, key contains unvisited nodes
    stack<int> stk;
    vector<pair<int, int>> edge;

    stk.push(key.back());
    key.pop_back();

    while (!stk.empty())
    {
        int u = stk.top();
        if (!key.empty() and tin[u] <= tin[key.back()] and tout[key.back()] <= tout[u])
        {
            edge.push_back(make_pair(u, key.back()));
            stk.push(key.back());
            key.pop_back();
            continue;
        }
        stk.pop();
    }

    return edge;
}