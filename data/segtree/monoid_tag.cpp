class tag_chan
{
public:
    int add = 0;

    tag_chan() : add(0) {};
    tag_chan(int x) : add(x) {};

    bool apply_to(monoid_chan &a, [[maybe_unused]] int l, [[maybe_unused]] int r) const
    {
        a.sum += add * (r - l + 1);
        return true;
    }
    void apply_to(tag_chan &t) const
    {
        t.add += add;
    }
    bool empty() const
    {
        return add == 0;
    }
};