#include<bits/stdc++.h>
using namespace std;

#ifdef natural_selection
#include "../libra/misc/dbg.h"
#else
#define debug(...)
#define endl "\n"
#endif

class Node
{
public:
    Node *l, *r, *p;

    int key, val;

    //subtree aggregates
    int siz, sum;

    //lazy propagation
    int add;

    Node() : p(nullptr), l(nullptr), r(nullptr), key(0), val(0), sum(0), add(0) {};

    void Push()
    {
        if(l != nullptr)
            l->sum += add;
        if(r != nullptr)
            r->sum += add;
        add = 0;
    }
    void Pull()
    {
        siz = sum = 0;
        if(l != nullptr)
            siz += l->siz, sum += r->sum;
        siz += 1, sum += val;
        if(r != nullptr)
            siz += r->siz, sum += r->sum;
    }
};

// template<typename Node>
namespace SplayChan
{
    bool IsRoot(Node* u)
    {
        if(u == nullptr)
            return false;
        return (u->p == nullptr);
    }

    void Rotate(Node* u)
    {
        auto p = u->p;
        assert(p != nullptr);

        p->Push();
        u->Push();
        p->p = u->p;

        if(u->p != nullptr)
        {
            if(p->p->l == u)
                p->p->l = u;
            else if(p->p->r == u)
                p->p->r = u;
        }

        if(u == p->l)
        {
            p->l = u->r;
            u->r = p;
        }
        else
        {
            p->r = u->l;
            u->l = p;
        }

        p->Pull();
        u->Pull();
    }

    void Splay(Node *u)
    {
        if(u == nullptr)
            return;
        while(!IsRoot(u))
        {
            auto p = u->p;
            if(!IsRoot(p))
            {
                if((p->l == u) ^ (p->p->l == p))
                    Rotate(u);
                else
                    Rotate(p);
            }
            Rotate(u);
        }
    }

    pair<Node*, int> Access(Node* u, const function<int(Node*)> &go_to)
    {
        if(u == nullptr)
            return {u, 0};
        Splay(u);
        int d;

        while(1)
        {
            u->Push();
            d = go_to(u);
            if(d == 0)
                break;
            Node *v = (d == -1 ? u->l : u->r);
            if(v == nullptr)
                break;
            u = v;
        }
        Splay(u);
        return {u, d};
    }

    Node* GetLeftmost(Node* u)
    {
        return Access(u, [&](Node*) {return -1;}).first;
    }
    Node* GetRightmost(Node* u)
    {
        return Access(u, [&](Node*) {return 1;}).first;
    }

    Node* GetKth(Node* u, int k)        // 0-indexed
    {
        pair<Node*, int> p = Access(u, [&](Node* u)
        {
            if(u->l != nullptr)
            {
                if(u->l->siz > k)
                    return -1;
                k -= u->l->siz;
            }
            if(k == 0)
                return 0;
            -- k;
            return 1;
        });

        return (p.second == 0 ? p.first : nullptr); 
    }

    int GetPosition(Node *u)        // 0 -indexed
    {
        Splay(u);
        return (u->l == nullptr ? 0 : u->l->siz);
    }

    Node* GetRoot(Node* u)
    {
        Splay(u);
        return u;
    }

    pair<Node*, Node*> Split(Node* u, const function<bool(Node*)> &IsRight)
    {
        if(u == nullptr)
            return {nullptr, nullptr};
        
        pair<Node*, int> p = Access(u, [&](Node* u) {return IsRight(u) ? -1 : 1;});
        
        //if p.second == 1, we have found the greatest node which should be in the left subtree
        //if p.second == -1, we have found the smallest node which should be in the right subtree
        //then, we splayed this node and its now the root

        u = p.first;
        u->Push();

        if(p.second == -1)
        {
            Node* v = u->l;
            if(v == nullptr)
                return {nullptr, u};
            u->l = nullptr;
            v->p = u->p;
            v = GetRightmost(v);
            u->p = v;
            u->Pull();
            return {v, u};
        }
        else
        {
            Node* v = u->r;
            if(v == nullptr)
                return {u, nullptr};
            u->r = nullptr;
            u->Pull();
            return {u, v};
        }

        return {nullptr, nullptr};
    }

    Node* Merge(Node* u, Node* v)       //all in u <= all in v
    {
        if(u == nullptr)
            return u;
        if(v == nullptr)
            return v;
        
        u = GetRightmost(u);
        assert(v->r == nullptr);

        Splay(v);
        u->Push();
        u->r = v;
        u->Pull();
        return u;
    }
};
using namespace SplayChan;

int32_t main()
{
}
