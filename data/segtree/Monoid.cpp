class MonoidChan
{
public:
    int sum = 0;

    MonoidChan() : sum(0) {};
    MonoidChan(int x) : sum(x) {};

    MonoidChan Unite(MonoidChan b) const 
    {
        MonoidChan res(sum + b.sum);
        return res;
    }
    static MonoidChan GetDefault([[maybe_unused]] int l, [[maybe_unused]] int r)
    {
        return MonoidChan();
    }
};
