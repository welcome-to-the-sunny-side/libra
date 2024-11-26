/*
general ideas:
- Can do a dfs to an arbitrary node in O(log(n)) time, if you always splay it afterwards
- For single node modifications, we generally splay the node first and then do what we want
- BST property is maintained throughout (key can be redundant a lot of times)
*/

namespace splay_chan
{
    //common: push, splay, pull
    bool is_root(node* u)
    {
        if(u == nullptr)
            return false;
        return (u->p == nullptr);
    }

    void rotate(node* u)
    {
        auto p = u->p;
        assert(p != nullptr);

        p->push();
        u->push();

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

        p->pull();
        u->pull();
    }

    void splay(node *u)
    {
        if(u == nullptr)
            return;
        while(!is_root(u))
        {
            auto p = u->p;
            if(!is_root(p))
            {
                if((p->l == u) ^ (p->p->l == p))
                    rotate(u);
                else
                    rotate(p);
            }
            rotate(u);
        }
    }

    pair<node*, int> access(node* u, const function<int(node*)> &go_to)
    {
        if(u == nullptr)
            return {u, 0};
        splay(u);
        int d = 0;

        while(1)
        {
            u->push();
            d = go_to(u);
            if(d == 0)
                break;
            node *v = (d == -1 ? u->l : u->r);
            if(v == nullptr)
                break;
            u = v;      
        }

        splay(u);
        return {u, d};
    }

    node* get_leftmost(node* u)
    {
        return access(u, [&](node*) {return -1;}).first;
    }

    node* get_rightmost(node* u)
    {
        return access(u, [&](node*) {return 1;}).first;
    }

    node* get_kth(node* u, int k)        // 0-indexed
    {
        pair<node*, int> p = access(u, [&](node* u)
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
        v->push();
        splay(v);

        return (p.second == 0 ? v : nullptr); 
    }

    int get_position(node *u)        // 0 -indexed
    {
        splay(u);
        return (u->l == nullptr ? 0 : u->l->siz);
    }

    node* get_root(node* u)
    {
        splay(u);
        return u;
    }

    pair<node*, node*> split(node* u, const function<bool(node*)> &IsRight)
    {
        if(u == nullptr)
            return {nullptr, nullptr};
        
        pair<node*, int> p = access(u, [&](node* u) {return IsRight(u) ? -1 : 1;});
        
        //if p.second == 1, we have found the greatest node which should be in the left subtree
        //if p.second == -1, we have found the smallest node which should be in the right subtree
        //then in both cases, we splayed this node and its now the root

        u = p.first;
        u->push();
        splay(u);

        if(p.second == -1)
        {
            node* v = u->l;
            if(v == nullptr)
                return {nullptr, u};
            u->l = nullptr;
            v->p = nullptr;
            u->pull();
            return {v, u};
        }
        else
        {
            node* v = u->r;
            if(v == nullptr)
                return {u, nullptr};
            u->r = nullptr;
            v->p = nullptr;
            u->pull();
            return {u, v};
        }

        return {nullptr, nullptr};
    }

    node* merge(node* u, node* v)       //all in u <= all in v
    {
        if(u == nullptr)
            return v;
        if(v == nullptr)
            return u;
        
        u = get_rightmost(u);
        u->push();
        
        splay(u);

        assert(u->r == nullptr);
        u->r = v;
        v->p = u;
        u->pull();

        return u;
    }

    node *insert(node *r, node *v, const function<bool(node *)> &go_left)
    {
        //returns new root
        pair<node *, node *> p = split(r, go_left);
        return merge(p.first, merge(v, p.second));
    }

    node *remove(node *u)
    { 
        // returns the new root
        splay(u);
        u->push();
        node *x = u->l;
        node *y = u->r;
        u->l = u->r = nullptr;
        if(x != nullptr)
            x->p = nullptr;
        if(y != nullptr) 
            y->p = nullptr;
        node *z = merge(x, y);
        
        u->p = nullptr;
        u->push();
        u->pull(); // now u might be reusable...
        
        return z;
    }

    //untested
    node *next(node *v)
    {
        splay(v);
        v->push();
        //find leftmost node in subtree of right child of node v
        if (v->r == nullptr)
            return nullptr;
        v = v->r;
        while (v->l != nullptr)
        {
            v->push();
            v = v->l;
        }
        splay(v);
        return v;
    }

    //untested
    node *prev(node *v)
    {
        splay(v);
        v->push();
        //find rightmost node in subtree of left child of v
        if (v->l == nullptr)
            return nullptr;
        v = v->l;
        while (v->r != nullptr)
        {
            v->push();
            v = v->r;
        }
        splay(v);
        return v;
    }

    //0-indexed
    //`operate(0, node*)` is to operate with single node
    //`operate(1, node*)` is to operate on subtree of node
    template<typename O>
    node* query(node* u, int l, int r, O operate)
    {
        u = get_kth(u, l);
        assert(u != nullptr);
        assert(l <= r and r < u->siz);

        int remaining = (r - l + 1);
        
        -- remaining;
        operate(0, u);
        u->push();

        if(u->r != nullptr)
            u = u->r;

        while(remaining > 0)
        {
            u->push();

            if(u->l != nullptr)
            {
                if(u->l->siz >= remaining)
                {
                    u = u->l;
                    continue;
                }
                remaining -= u->l->siz;
                operate(1, u->l);
            }

            if(remaining > 0)
            {
                -- remaining;
                operate(0, u);
            }

            if(u->r == nullptr)
                break;
            u = u->r;
        }

        u->push();
        splay(u);
        u->pull();

        return u;
    }

    //0-indexed
    //`operate(0, node*)` is to operate with single node
    //`operate(1, node*)` is to operate on subtree of node
    template<typename O>
    node* modify(node* u, int l, int r, O operate)
    {
        u = get_kth(u, l);
        assert(u != nullptr);
        assert(l <= r and r < u->siz);

        int remaining = (r - l + 1);
        
        -- remaining;
        operate(0, u);
        u->push();

        auto dfs = [&](auto &&dfs) -> void
        {
            if(remaining == 0)
                return;
            
            node* cur = u;
            u->push();

            if(u->l != nullptr)
            {
                if(u->l->siz >= remaining)
                {
                    u = u->l;
                    dfs(dfs);

                    cur->pull();
                    return;
                }
                
                remaining -= u->l->siz;
                operate(1, u->l);
                u->pull();          //left child might have been updated
            }

            if(remaining > 0)
            {
                -- remaining;
                operate(0, u);
            }

            if(u->r == nullptr)
            {
                cur->pull();
                return;
            }

            u = u->r;
            dfs(dfs);

            cur->pull();
            return;
        };

        if(u->r != nullptr)
            u = u->r;
        
        node* cur = u;
        dfs(dfs);
        cur->pull();

        u->push();
        splay(u);
        u->pull();

        return u;
    }
};
using namespace splay_chan;