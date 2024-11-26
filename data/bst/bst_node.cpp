class node
{
public:
//tree info
    node *l, *r, *p;
    //self
    int key;
    //subtree aggregate
    int siz;

//monoid info
    //self
    int64_t val;

    //subtree Aggregates
    int64_t sum;

    //lazy propagation
    int64_t add;

    node()
    {
        p = l = r = nullptr;
        key = siz = val = sum = add = 0;
    }

    //current attributes are already updated, update child attributes so that a pull would result in the correct aggregate
    void push()
    {
        if(l != nullptr)
            l->sum += add * l->siz, l->val += add, l->add += add;
        if(r != nullptr)
            r->sum += add * r->siz, r->val += add, r->add += add;
        add = 0;
    }
    void pull()
    {
        siz = sum = 0;
        if(l != nullptr)
            siz += l->siz, sum += l->sum;
        siz += 1, sum += val;
        if(r != nullptr)
            siz += r->siz, sum += r->sum;
    }
};
