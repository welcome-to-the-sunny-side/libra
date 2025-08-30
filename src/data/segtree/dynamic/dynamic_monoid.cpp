class dynamic_monoid_chan
{
public:
    dynamic_monoid_chan *left, *right;
    int sum = 0;

    dynamic_monoid_chan() : sum(0) {};
    dynamic_monoid_chan(int x) : sum(x) {};

    dynamic_monoid_chan unite(dynamic_monoid_chan b) const 
    {
        dynamic_monoid_chan res(sum + b.sum);
        return res;
    }
    static dynamic_monoid_chan get_default([[maybe_unused]] int l, [[maybe_unused]] int r)
    {
        return dynamic_monoid_chan();
    }

    dynamic_monoid_chan grow()
    {
        if(left == nullptr)
        {
            left = new dynamic_monoid_chan();
            right = new dynamic_monoid_chan();
        }
    }
};