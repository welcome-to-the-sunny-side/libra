struct random64 : std::mt19937_64
{
    using std::mt19937_64::mt19937_64;
    using std::mt19937_64::operator();

    static int64_t gen_seed()
    {
        return std::chrono::steady_clock::now().time_since_epoch().count();
    }

    random64() : std::mt19937_64(gen_seed()) {}

    template <class Int>
    auto operator()(Int a, Int b)
        -> std::enable_if_t<std::is_integral_v<Int>, Int>
    {
        return std::uniform_int_distribution<Int>(a, b)(*this);
    }

    template <class Int>
    auto operator()(Int a)
        -> std::enable_if_t<std::is_integral_v<Int>, Int>
    {
        return std::uniform_int_distribution<Int>(0, a - 1)(*this);
    }

    template <class Real>
    auto operator()(Real a, Real b)
        -> std::enable_if_t<std::is_floating_point_v<Real>, Real>
    {
        return std::uniform_real_distribution<Real>(a, b)(*this);
    }
};