template <typename T> struct RandomList : public List<T> {
    using List<T>::List;

    template <typename OT> RandomList(List<OT> other) : List<T>(other) {}

    List<T> &shuffle() {
        ::shuffle(this->begin(), this->end(), get_engine());

        return *this;
    }

    T choice() { return (*this)[::randint(this->get_start(), this->get_end() - 1)]; }
};

template <typename T> List<T> permutation(T x) { return RandomList<int>(range(T(1), x + 1)).shuffle(); }

template <typename T> List<T> rand_array(int N, T x, T y) {
    return range(N).map<T>([&](int _) { return randint(x, y); });
}

template <typename T> List<T> distinct_array(int N, T x, T y) {
    RandomList<int> arr = rand_array(N, x, y - N + 1).sort().enumerate().template map<T>([&](pair<T, int> e) {
        return e.first + e.second;
    });

    return arr.shuffle();
}

template <typename T> class ArrayBuilder {
  public:
    List<tuple<int, T, T>> ab;

    ArrayBuilder(List<tuple<int, T, T>> ab) { this->ab = ab; }

    List<T> build_normal() {
        return ab
            .template map<List<T>>([](tuple<int, T, T> x) {
                auto [len, lo, hi] = x;

                return rand_array(len, lo, hi);
            })
            .sum();
    }
};
