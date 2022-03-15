template <typename T> class List : public vector<T> {
    size_t offset = 0;

  public:
    using vector<T>::vector;

    List<T> &one_indexed() {
        offset = 1;
        return *this;
    }
    const List<T> &one_indexed() const {
        offset = 1;
        return *this;
    }

    T &operator[](size_t x) { return this->at(x - offset); }
    const T &operator[](size_t x) const { return this->at(x - offset); }

    template <typename F> List<T> &for_each(const F &f) {
        ::for_each(all(*this), f);
        return *this;
    }
    template <typename F> const List<T> &for_each(const F &f) const {
        ::for_each(all(*this), f);
        return *this;
    }

    List<T> &sort() {
        ::sort(all(*this));
        return *this;
    }

    template <typename F> List<T> &transform(const F &f) {
        ::transform(all(*this), this->begin(), f);
        return *this;
    }

    template <typename F> auto transform_new(const F &f) const {
        List<decltype(declval<F>()(declval<T>()))> ret;
        ret.reserve(this->size());
        ::transform(all(*this), back_inserter(ret), f);
        return ret;
    }
};

template <typename F> auto mapRange(int l, int r, F &f) {
    List<decltype(declval<F>()(0))> ret;
    ret.reserve(r - l + 1);
    for (; l < r; ++l) {
        ret.push_back(f(l));
    }

    return ret;
}
template <typename F> auto mapRange(int r, F &f) { return mapRange(0, r, f); }

template <typename F> auto generateList(int N, F &f) {
    List<decltype(declval<F>()())> ret;
    ret.reserve(N);
    generate_n(back_inserter(ret), N, f);
    return ret;
}
