template <typename T> class List : public vector<T> {
    size_t offset = 0;

  public:
    using vector<T>::vector;
    List(vector<T> v) : vector<T>::vector(move(v)) {}

    List<T> &one_indexed() {
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

    List<T> &sort() {
        ::sort(all(*this));
        return *this;
    }

    List<T> &reverse() {
        ::reverse(all(*this));
        return *this;
    }

    template <typename F> List<T> &map(F f) {
        ::transform(all(*this), this->begin(), f);
        return *this;
    }

    template <typename F> auto map_new(F f) const {
        List<decltype(declval<F>()(declval<T>()))> ret;
        ret.reserve(this->size());
        ::transform(all(*this), back_inserter(ret), f);
        return ret;
    }
};

template <typename F> auto generate(int N, F f) {
    List<decltype(declval<F>()())> ret;
    ret.reserve(N);
    ::generate_n(back_inserter(ret), N, move(f));
    return ret;
}

template <typename T, typename F> void exhaust_queue(queue<T> &q, F f) {
    while (!q.empty()) {
        int x = q.front();
        q.pop();
        f(x);
    }
}
