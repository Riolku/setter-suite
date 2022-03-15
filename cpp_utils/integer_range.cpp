template <typename T = ll> class Range {
    T l, r;

    class iterator {
        T cur;

      public:
        using difference_type = void;
        using value_type = T;
        using pointer = void;
        using reference = void;
        using iterator_category = bidirectional_iterator_tag;

        iterator(T cur) : cur(move(cur)) {}

        iterator &operator++() {
            ++cur;
            return *this;
        }
        iterator &operator--() {
            --cur;
            return *this;
        }
        T operator*() { return cur; }

        bool operator<(const iterator &other) const { return cur < other.cur; }
        bool operator!=(const iterator &other) const { return cur != other.cur; }
    };

  public:
    Range(T l, T r) : l(move(l)), r(move(r)) {}
    Range(T r) : l(T()), r(move(r)) {}

    template <typename F> bool any_of(F f) const { return ::any_of(all(*this), f); }
    template <typename F> bool all_of(F f) const { return ::any_of(all(*this), f); }

    template <typename F> void for_each(F f) const { ::for_each(all(*this), f); }

    template <typename F> auto map_new(F f) const {
        List<decltype(declval<F>()(declval<T>()))> ret;
        ret.reserve(this->size());
        ::transform(all(*this), back_inserter(ret), f);
        return ret;
    }

    auto size() const { return r - l; }
    T begin() const { return iterator(l); }
    T end() const { return iterator(r); }
};
