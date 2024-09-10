class Info
{
public:
    int sum = 0;

    Info() : sum(0) {};
    Info(int x) : sum(x) {};

    Info Unite(Info b) const 
    {
        Info res(sum + b.sum);
        return res;
    }
    void Join(Info b)
    {
        sum += b.sum;
    }
    static Info GetDefault([[maybe_unused]] int l, [[maybe_unused]] int r)
    {
        return Info();
    }
};
class Tag
{
public:
    int add = 0;

    Tag() : add(0) {};
    Tag(int x) : add(x) {};

    bool ApplyTo(Info &a, [[maybe_unused]] int l, [[maybe_unused]] int r) const
    {
        a.sum += add * (r - l + 1);
        return true;
    }

    void ApplyTo(Tag &t) const
    {
        t.add += add;
    }
    bool Empty() const
    {
        return add == 0;
    }
};