class p_node_pointer_chan
{
public:
    int lc, rc;
    bool exp_create;

    p_node_pointer_chan() : lc(0), rc(0), exp_create(false) {};
    p_node_pointer_chan(int l, int r) : lc(l), rc(r) {};
};