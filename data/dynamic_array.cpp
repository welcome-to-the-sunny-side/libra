template <typename T>
class dynamic_array_chan
{
    struct Node
    {
        T v;
        uint32_t p;
        int sz;
        Node *l, *r;
        Node(const T &_v, uint32_t _p) : v(_v), p(_p), sz(1), l(nullptr), r(nullptr) {}
    };
    Node *root = nullptr;

    // tiny xorshift RNG for priorities (fast, header-only)
    static uint32_t rng()
    {
        static uint32_t x = (uint32_t)chrono::steady_clock::now().time_since_epoch().count();
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        return x ? x : 1u;
    }

    static int SZ(Node *t) { return t ? t->sz : 0; }
    static void pull(Node *t)
    {
        if (t)
            t->sz = 1 + SZ(t->l) + SZ(t->r);
    }

    static void split(Node *t, int k, Node *&a, Node *&b)
    {
        if (!t)
        {
            a = b = nullptr;
            return;
        }
        if (SZ(t->l) >= k)
        {
            split(t->l, k, a, t->l);
            b = t;
        }
        else
        {
            split(t->r, k - SZ(t->l) - 1, t->r, b);
            a = t;
        }
        pull(t);
    }
    static Node *merge(Node *a, Node *b)
    {
        if (!a || !b)
            return a ? a : b;
        if (a->p > b->p)
        {
            a->r = merge(a->r, b);
            pull(a);
            return a;
        }
        else
        {
            b->l = merge(a, b->l);
            pull(b);
            return b;
        }
    }

    Node *kth(Node *t, int i) const
    {
        while (t)
        {
            int L = SZ(t->l);
            if (i < L)
                t = t->l;
            else if (i == L)
                return t;
            else
            {
                i -= L + 1;
                t = t->r;
            }
        }
        return nullptr; // unreachable if guarded by asserts
    }

public:
    int size() const { return SZ(root); }

    T get(int i) const
    {
        assert(0 <= i && i < size());
        return kth(root, i)->v;
    }

    void set(int i, const T &x)
    {
        assert(0 <= i && i < size());
        kth(root, i)->v = x;
    }

    void insert(int i, const T &x)
    {
        assert(0 <= i && i <= size());
        Node *A, *B;
        split(root, i, A, B);
        root = merge(merge(A, new Node(x, rng())), B);
    }

    void erase(int i)
    {
        assert(0 <= i && i < size());
        Node *A, *B, *C;
        split(root, i, A, B);
        split(B, 1, B, C);
        // delete B; // (optional in contests)
        root = merge(A, C);
    }
};
