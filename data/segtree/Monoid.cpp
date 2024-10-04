class InfoChan
{
public:
    int sum = 0;

    InfoChan() : sum(0) {};
    InfoChan(int x) : sum(x) {};

    InfoChan Unite(InfoChan b) const 
    {
        InfoChan res(sum + b.sum);
        return res;
    }
    static InfoChan GetDefault([[maybe_unused]] int l, [[maybe_unused]] int r)
    {
        return InfoChan();
    }
};
class TagChan
{
public:
    int add = 0;

    TagChan() : add(0) {};
    TagChan(int x) : add(x) {};

    bool ApplyTo(InfoChan &a, [[maybe_unused]] int l, [[maybe_unused]] int r) const
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