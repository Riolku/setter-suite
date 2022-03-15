template <typename T> class List : public vector<T> {
    size_t offset = 0;

  public:
    using vector<T>::vector;
    List(vector<T> v) : vector<T>::vector(move(v)) {}

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

    template <typename F> void for_each(F f) const { ::for_each(all(*this), f); }
    template <typename F> void for_each_pair(F f) const {
        ::for_each(all(*this), [&f](const T &p) {
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

    void sort() { ::sort(all(*this)); }

    template <typename F> void transform(F f) { ::transform(all(*this), this->begin(), f); }

    template <typename F> auto transform_new(F f) const {
        List<decltype(declval<F>()(declval<T>()))> ret;
        ret.reserve(this->size());
        ::transform(all(*this), back_inserter(ret), f);
        return ret;
    }
};

template <typename F> auto mapRange(int l, int r, F f) {
    List<decltype(declval<F>()(0))> ret;
    ret.reserve(r - l + 1);
    for (; l < r; ++l) {
        ret.push_back(f(l));
    }

    return ret;
}
template <typename F> auto mapRange(int r, F f) { return mapRange(0, r, f); }

template <typename F> auto generateList(int N, F f) {
    List<decltype(declval<F>()())> ret;
    ret.reserve(N);
    generate_n(back_inserter(ret), N, f);
    return ret;
}

template <typename T, typename F> void exhaust_queue(queue<T> &q, F f) {
    while (!q.empty()) {
        int x = q.front();
        q.pop();
        f(x);
    }
}
