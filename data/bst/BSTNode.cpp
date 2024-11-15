class Node
{
public:
//tree info
    Node *l, *r, *p;
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

    Node() : p(nullptr), l(nullptr), r(nullptr), key(0), siz(0),
             val(0), sum(0), add(0) {};
    
    void Push()
    {
        val += add;
        if(l != nullptr)
            l->sum += add * l->siz, l->add += add;
        if(r != nullptr)
            r->sum += add * r->siz, r->add += add;
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