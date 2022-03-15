template <typename T = ll> class RangeBase {
    T l, r;

  public:
    using value_type = T;
    RangeBase(T l, T r) : l(l), r(r) {}

    T operator[](T offset) const {
        assert(l + offset < r);
        return l + offset;
    }

    T begin() const { return l; }
    T end() const { return r; }
};
