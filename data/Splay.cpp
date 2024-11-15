class Node
{
public:
    Node *l, *r, *p;

    int key;
    int64_t val;

    //subtree aggregates
    int siz;
    int64_t sum;

    //lazy propagation
    int64_t add;

    Node() : p(nullptr), l(nullptr), r(nullptr), key(0), val(0), sum(0), add(0) {};

    void Push()
    {
        val += add;
        if(l != nullptr)
            l->sum += add * l->siz, l->add += add;
        if(r != nullptr)
            r->sum += add * r->siz, r->add += r->add;
        add = 0;
    }
    void Pull()
    {
        siz = sum = 0;
        if(l != nullptr)
            siz += l->siz, sum += l->sum;
        siz += 1, sum += val;
        if(r != nullptr)
            siz += r->siz, sum += r->sum;
    }
};

namespace SplayChan
{
    //common: push, splay, pull

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

        u->p = p->p;
        p->p = u;

        if(u->p != nullptr)
        {
            if(u->p->l == p)
                u->p->l = u;
            if(u->p->r == p)
                u->p->r = u;
        }

        if(u == p->l)
        {
            p->l = u->r;
            if(u->r != nullptr)
                u->r->p = p;
            u->r = p;
        }
        else
        {
            p->r = u->l;
            if(u->l != nullptr)
                u->l->p = p;
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
        int d = 0;

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

        auto v = p.first;
        v->Push();
        Splay(v);

        return (p.second == 0 ? v : nullptr); 
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
        //then in both cases, we splayed this node and its now the root

        u = p.first;
        u->Push();
        Splay(u);

        if(p.second == -1)
        {
            Node* v = u->l;
            if(v == nullptr)
                return {nullptr, u};
            u->l = nullptr;
            v->p = nullptr;
            u->Pull();
            return {v, u};
        }
        else
        {
            Node* v = u->r;
            if(v == nullptr)
                return {u, nullptr};
            u->r = nullptr;
            v->p = nullptr;
            u->Pull();
            return {u, v};
        }

        return {nullptr, nullptr};
    }

    Node* Merge(Node* u, Node* v)       //all in u <= all in v
    {
        if(u == nullptr)
            return v;
        if(v == nullptr)
            return u;
        
        u = GetRightmost(u);
        u->Push();
        
        Splay(u);

        assert(u->r == nullptr);
        u->r = v;
        v->p = u;
        u->Pull();

        return u;
    }

    Node *Insert(Node *r, Node *v, const function<bool(Node *)> &go_left)
    {
        //returns new root
        pair<Node *, Node *> p = Split(r, go_left);
        return Merge(p.first, Merge(v, p.second));
    }

    Node *Remove(Node *u)
    { 
        // returns the new root
        Splay(u);
        u->Push();
        Node *x = u->l;
        Node *y = u->r;
        u->l = u->r = nullptr;
        if(x != nullptr)
            x->p = nullptr;
        if(y != nullptr) 
            y->p = nullptr;
        Node *z = Merge(x, y);
        
        u->p = nullptr;
        u->Push();
        u->Pull(); // now u might be reusable...
        
        return z;
    }

    //untested
    Node *Next(Node *v)
    {
        Splay(v);
        v->Push();
        //find leftmost node in subtree of right child of node v
        if (v->r == nullptr)
            return nullptr;
        v = v->r;
        while (v->l != nullptr)
        {
            v->Push();
            v = v->l;
        }
        Splay(v);
        return v;
    }

    //untested
    Node *Prev(Node *v)
    {
        Splay(v);
        v->Push();
        //find rightmost node in subtree of left child of v
        if (v->l == nullptr)
            return nullptr;
        v = v->l;
        while (v->r != nullptr)
        {
            v->Push();
            v = v->r;
        }
        Splay(v);
        return v;
    }
};
using namespace SplayChan;