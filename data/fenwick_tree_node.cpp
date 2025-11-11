class node_chan
{
public:
    int v = 0;

    inline void operator+=(node_chan &other)
    {
        v += other.v;
    }
    inline bool operator<(node_chan &other)
    {
        return v < other.v;
    }
};