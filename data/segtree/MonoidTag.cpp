class TagChan
{
public:
    int add = 0;

    TagChan() : add(0) {};
    TagChan(int x) : add(x) {};

    bool ApplyTo(MonoidChan &a, [[maybe_unused]] int l, [[maybe_unused]] int r) const
    {
        a.sum += add * (r - l + 1);
        return true;
    }
    void ApplyTo(TagChan &t) const
    {
        t.add += add;
    }
    bool Empty() const
    {
        return add == 0;
    }
};