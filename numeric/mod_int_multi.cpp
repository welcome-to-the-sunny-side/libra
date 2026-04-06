template <typename... Mints>
struct multi_mint
{
    static constexpr size_t D = sizeof...(Mints);
    std::tuple<Mints...> v;

    multi_mint() = default;

    multi_mint(int64_t x) : v(Mints(x)...) {}

    template <typename... Ts, typename = std::enable_if_t<sizeof...(Ts) == D>>
    multi_mint(Ts... xs) : v(Mints(xs)...) {}

private:
    template <size_t... I>
    void add_impl(const multi_mint &other, std::index_sequence<I...>)
    {
        ((std::get<I>(v) += std::get<I>(other.v)), ...);
    }

    template <size_t... I>
    void sub_impl(const multi_mint &other, std::index_sequence<I...>)
    {
        ((std::get<I>(v) -= std::get<I>(other.v)), ...);
    }

    template <size_t... I>
    void mul_impl(const multi_mint &other, std::index_sequence<I...>)
    {
        ((std::get<I>(v) *= std::get<I>(other.v)), ...);
    }

    template <size_t... I>
    void div_impl(const multi_mint &other, std::index_sequence<I...>)
    {
        ((std::get<I>(v) /= std::get<I>(other.v)), ...);
    }

    template <size_t... I>
    bool eq_impl(const multi_mint &other, std::index_sequence<I...>) const
    {
        return ((std::get<I>(v) == std::get<I>(other.v)) && ...);
    }

    template <size_t... I>
    bool lt_impl(const multi_mint &other, std::index_sequence<I...>) const
    {
        return v < other.v; // tuple lexicographic comparison
    }

    template <size_t... I>
    multi_mint neg_impl(std::index_sequence<I...>) const
    {
        multi_mint res;
        res.v = std::make_tuple((-std::get<I>(v))...);
        return res;
    }

    template <size_t... I>
    multi_mint inv_impl(std::index_sequence<I...>) const
    {
        multi_mint res;
        res.v = std::make_tuple(std::get<I>(v).inv()...);
        return res;
    }

    template <size_t... I>
    multi_mint pow_impl(int64_t p, std::index_sequence<I...>) const
    {
        multi_mint res;
        res.v = std::make_tuple(std::get<I>(v).pow(p)...);
        return res;
    }

    template <size_t... I>
    void print_impl(std::ostream &os, std::index_sequence<I...>) const
    {
        os << '{';
        bool first = true;
        ((os << (std::exchange(first, false) ? "" : ", ") << std::get<I>(v)), ...);
        os << '}';
    }

public:
    multi_mint &operator+=(const multi_mint &other)
    {
        add_impl(other, std::make_index_sequence<D>{});
        return *this;
    }

    multi_mint &operator-=(const multi_mint &other)
    {
        sub_impl(other, std::make_index_sequence<D>{});
        return *this;
    }

    multi_mint &operator*=(const multi_mint &other)
    {
        mul_impl(other, std::make_index_sequence<D>{});
        return *this;
    }

    multi_mint &operator/=(const multi_mint &other)
    {
        div_impl(other, std::make_index_sequence<D>{});
        return *this;
    }

    multi_mint operator-() const
    {
        return neg_impl(std::make_index_sequence<D>{});
    }

    multi_mint inv() const
    {
        return inv_impl(std::make_index_sequence<D>{});
    }

    multi_mint pow(int64_t p) const
    {
        return pow_impl(p, std::make_index_sequence<D>{});
    }

    friend multi_mint operator+(multi_mint a, const multi_mint &b) { return a += b; }
    friend multi_mint operator-(multi_mint a, const multi_mint &b) { return a -= b; }
    friend multi_mint operator*(multi_mint a, const multi_mint &b) { return a *= b; }
    friend multi_mint operator/(multi_mint a, const multi_mint &b) { return a /= b; }

    friend bool operator==(const multi_mint &a, const multi_mint &b)
    {
        return a.eq_impl(b, std::make_index_sequence<D>{});
    }

    friend bool operator!=(const multi_mint &a, const multi_mint &b)
    {
        return !(a == b);
    }

    friend bool operator<(const multi_mint &a, const multi_mint &b)
    {
        return a.v < b.v;
    }

    friend bool operator>(const multi_mint &a, const multi_mint &b)
    {
        return b < a;
    }

    friend bool operator<=(const multi_mint &a, const multi_mint &b)
    {
        return !(b < a);
    }

    friend bool operator>=(const multi_mint &a, const multi_mint &b)
    {
        return !(a < b);
    }

    template <size_t I>
    auto &get() { return std::get<I>(v); }

    template <size_t I>
    const auto &get() const { return std::get<I>(v); }

    friend std::ostream &operator<<(std::ostream &os, const multi_mint &m)
    {
        m.print_impl(os, std::make_index_sequence<D>{});
        return os;
    }
};
template <const int &...MODS>
using multi_mint_mods = multi_mint<modular_int<MODS>...>;

const int MOD1 = 998244353;
const int MOD2 = 1000000007;
const int MOD3 = 1000000009;

using mint1 = modular_int<MOD1>;
using mint2 = modular_int<MOD2>;
using mint3 = modular_int<MOD3>;

using mmint = multi_mint<mint1, mint2, mint3>;

// using mmint = multi_mint_mods<MOD1, MOD2, MOD3>;