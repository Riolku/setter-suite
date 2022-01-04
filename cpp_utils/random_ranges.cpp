struct vshuffle_t {};
template <typename T> vector<T> operator|(vector<T> v, vshuffle_t _) {
    shuffle(v.begin(), v.end(), get_engine());
    return v;
}
vshuffle_t vshuffle;

vector<int> permutation(int x) { return viota(1, x + 1) | to_vector | vshuffle; }

auto rand_array(size_t N, ll x, ll y) {
    return repeatMap(N, [x, y](size_t i) { return randint(x, y); });
}

auto distinct_array(size_t N, ll x, ll y) {
    return rand_array(N, x, y - N + 1) | to_vector | vsort | to_view | enumerate |
           vmap([](pair<ll, int> e) { return e.first + e.second; }) | to_vector | vshuffle;
}

struct ArrayBuilder : public vector<tuple<size_t, ll, ll>> {
    using vector<tuple<size_t, ll, ll>>::vector;

    auto build_normal() {
        return (*this) | to_view |
               vmap([](const tuple<size_t, ll, ll> &tp) { return rand_array(get<0>(tp), get<1>(tp), get<2>(tp)); }) |
               to_vector | to_view | vjoin;
    }
};
