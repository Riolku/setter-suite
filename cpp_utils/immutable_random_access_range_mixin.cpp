template <typename Container> class ImmutableRandomAccessRangeMixin : public Container {
    size_t offset = 0;

  public:
    using value_type = Container::value_type;

    using Container::Container;
    ImmutableRandomAccessRangeMixin(Container c) : Container(move(c)) {}

    void one_indexed() { offset = 1; }

    Container::value_type &operator[](size_t x) { return this->at(x - offset); }
    const Container::value_type &operator[](size_t x) const { return this->at(x - offset); }

    template <typename F> void for_each(F f) const { ::for_each(all(*this), f); }
    template <typename F> void for_each_pair(F f) const {
        ::for_each(all(*this), [&f](const Container::value_type &p) {
            auto [a, b] = p;
            f(a, b);
        });
    }
    template <typename F> void for_each_enumerate(F f) const {
        size_t i = offset;
        for (auto it = this->begin(); it != this->end(); ++it) {
            f(*it, i);
            ++i;
        }
    }

    template <typename F> bool any_of(F f) const { return ::any_of(all(*this), f); }
    template <typename F> bool all_of(F f) const { return ::any_of(all(*this), f); }

    template <typename F> auto transform_new(F f) const {
        List<decltype(declval<F>()(declval<Container::value_type>()))> ret;
        ret.reserve(this->size());
        ::transform(all(*this), back_inserter(ret), f);
        return ret;
    }
};
