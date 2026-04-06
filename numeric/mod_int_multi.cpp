template <typename... Mints>
struct multi_mint
{
    static constexpr int D = sizeof...(Mints);
    tuple<Mints...> v;

    multi_mint() = default;

    // same integer copied into all coordinates
    multi_mint(int64_t x) : v(Mints(x)...) {}

    // component-wise initialization
    template <typename... Ts, typename = enable_if_t<sizeof...(Ts) == D>>
    multi_mint(Ts... xs) : v(Mints(xs)...) {}

private:
    template <size_t... I>
    void add_impl(const multi_mint &other, index_sequence<I...>)
    {
        ((std::get<I>(v) += std::get<I>(other.v)), ...);
    }

    template <size_t... I>
    void sub_impl(const multi_mint &other, index_sequence<I...>)
    {
        ((std::get<I>(v) -= std::get<I>(other.v)), ...);
    }

    template <size_t... I>
    void mul_impl(const multi_mint &other, index_sequence<I...>)
    {
        ((std::get<I>(v) *= std::get<I>(other.v)), ...);
    }

    template <size_t... I>
    void div_impl(const multi_mint &other, index_sequence<I...>)
    {
        ((std::get<I>(v) /= std::get<I>(other.v)), ...);
    }

    template <size_t... I>
    bool eq_impl(const multi_mint &other, index_sequence<I...>) const
    {
        return ((std::get<I>(v) == std::get<I>(other.v)) && ...);
    }

    template <size_t... I>
    multi_mint pow_impl(int64_t p, index_sequence<I...>) const
    {
        multi_mint res;
        res.v = make_tuple(std::get<I>(v).pow(p)...);
        return res;
    }

    template <size_t... I>
    void print_impl(ostream &os, index_sequence<I...>) const
    {
        os << '{';
        bool first = true;
        ((os << (std::exchange(first, false) ? "" : ", ") << std::get<I>(v)), ...);
        os << '}';
    }

public:
    multi_mint &operator+=(const multi_mint &other)
    {
        add_impl(other, make_index_sequence<D>{});
        return *this;
    }

    multi_mint &operator-=(const multi_mint &other)
    {
        sub_impl(other, make_index_sequence<D>{});
        return *this;
    }

    multi_mint &operator*=(const multi_mint &other)
    {
        mul_impl(other, make_index_sequence<D>{});
        return *this;
    }

    multi_mint &operator/=(const multi_mint &other)
    {
        div_impl(other, make_index_sequence<D>{});
        return *this;
    }

    friend multi_mint operator+(multi_mint a, const multi_mint &b) { return a += b; }
    friend multi_mint operator-(multi_mint a, const multi_mint &b) { return a -= b; }
    friend multi_mint operator*(multi_mint a, const multi_mint &b) { return a *= b; }
    friend multi_mint operator/(multi_mint a, const multi_mint &b) { return a /= b; }

    friend bool operator==(const multi_mint &a, const multi_mint &b)
    {
        return a.eq_impl(b, make_index_sequence<D>{});
    }

    friend bool operator!=(const multi_mint &a, const multi_mint &b)
    {
        return !(a == b);
    }

    multi_mint pow(int64_t p) const
    {
        return pow_impl(p, make_index_sequence<D>{});
    }

    template <size_t I>
    auto &get() { return std::get<I>(v); }

    template <size_t I>
    const auto &get() const { return std::get<I>(v); }

    friend ostream &operator<<(ostream &os, const multi_mint &m)
    {
        m.print_impl(os, make_index_sequence<D>{});
        return os;
    }
};

const int MOD1 = 998244353;
const int MOD2 = 1000000007;
const int MOD3 = 1000000009;

using mint1 = modular_int<MOD1>;
using mint2 = modular_int<MOD2>;
using mint3 = modular_int<MOD3>;

using mmint = multi_mint<mint1, mint2, mint3>;