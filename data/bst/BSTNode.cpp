class Node
{
public:
    Node *l, *r, *p;

    //self
    int key;

    //subtree aggregate
    int siz;

    Node() : p(nullptr), l(nullptr), r(nullptr), key(0), siz(0) {};

    virtual void Push() = 0;
    virtual void Pull() = 0;
};

class SumNode : public Node
{
public:
    //self
    int64_t val;

    //subtree Aggregates
    int64_t sum;

    //lazy propagation
    int64_t add;

    SumNode() : val(0), sum(0), add(0) {};

    void Push()
    {
        val += add;
        if(l != nullptr)
            ((SumNode*)l)->sum += add * l->siz, ((SumNode*)l)->add += add;
        if(r != nullptr)
            ((SumNode*)r)->sum += add * r->siz, ((SumNode*)r)->add += add;
        add = 0;
    }
    void Pull()
    {
        siz = sum = 0;
        if(l != nullptr)
            siz += l->siz, sum += ((SumNode*)l)->sum;
        siz += 1, sum += val;
        if(r != nullptr)
            siz += r->siz, sum += ((SumNode*)r)->sum;
    }
};
