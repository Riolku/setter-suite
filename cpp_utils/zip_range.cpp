template <range R> struct zip_range {
    R r;
    zip_range(R r) : r(std::move(r)) {}
};

template <range A, range B> struct full_zip : public ranges::view_interface<full_zip<A, B>> {
    A a;
    B b;
    full_zip() = default;
    full_zip(A a, B b) : a(std::move(a)), b(std::move(b)) {}

    struct iterator {
        using difference_type = range_difference_t<A>;
        using value_type = pair<range_reference_t<const A>, range_reference_t<const B>>;
        iterator_t<const A> a_it;
        iterator_t<const B> b_it;
        sentinel_t<const A> a_end;
        sentinel_t<const B> b_end;

        value_type operator*() const { return value_type(*a_it, *b_it); }
        value_type operator[](difference_type d) const { return *(iterator(*this) + d); }

        iterator &operator++() {
            ++a_it;
            ++b_it;
            return *this;
        }
        iterator operator++(int) {
            iterator tmp(*this);
            ++tmp;
            return tmp;
        }

        iterator &operator--() {
            --a_it;
            --b_it;
            return *this;
        }
        iterator operator--(int) {
            iterator tmp(*this);
            --tmp;
            return tmp;
        }
        difference_type operator-(const iterator &other) const { return a_it - other.a_it; }
        iterator &operator-=(difference_type v) {
            a_it -= v;
            b_it -= v;
            return *this;
        }
        iterator operator-(difference_type v) const { return iterator(*this) -= v; }
        iterator &operator+=(difference_type v) {
            a_it += v;
            b_it += v;
            return *this;
        }
        iterator operator+(difference_type v) const { return iterator(*this) += v; }

        friend constexpr iterator operator+(difference_type n, iterator i) { return i += n; }

        auto operator<=>(const iterator &other) const { return a_it <=> other.a_it; }
        bool operator==(const iterator &other) const { return a_it == other.a_it && b_it == other.b_it; }
        bool operator!=(const unreachable_sentinel_t &other) const { return a_it != a_end && b_it != b_end; }
    };

    iterator begin() const { return iterator{a.begin(), b.begin(), a.end(), b.end()}; }
    unreachable_sentinel_t end() const { return unreachable_sentinel_t{}; }
};
template <range R, range zR> auto operator|(R r, zip_range<zR> zr) { return full_zip(std::move(r), std::move(zr.r)); }
