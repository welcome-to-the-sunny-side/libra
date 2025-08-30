class monoid_chan
{
public:
    int sum = 0;

    monoid_chan() : sum(0) {};
    monoid_chan(int x) : sum(x) {};

    monoid_chan unite(monoid_chan b) const 
    {
        monoid_chan res(sum + b.sum);
        return res;
    }
    static monoid_chan get_default([[maybe_unused]] int l, [[maybe_unused]] int r)
    {
        return monoid_chan();
    }
};
