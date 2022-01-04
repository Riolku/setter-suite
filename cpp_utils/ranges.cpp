template <typename Num, typename F> auto repeatMap(Num n, F f) { return viota(Num(), n) | vmap(f); }

template <typename Num, range R> auto multiply(Num n, R r) {
    return repeatMap(n, [r](int _) { return r; }) | to_vector | to_view | vjoin;
}

template <typename F> struct rfor_each { F f; };
template <range R, typename F> void operator|(R r, rfor_each<F> f) { for_each(r.begin(), r.end(), f.f); }

struct pairs_t {};
template <range R> auto operator|(R r, pairs_t _) { return full_zip(r, r | vdrop(1)); }
pairs_t get_pairs;

const zip_range enumerate(viota(0));

struct vsort_t {};
template <typename T> vector<T> operator|(vector<T> v, vsort_t _) {
    sort(v.begin(), v.end());
    return v;
}
vsort_t vsort;

struct rsum_t {};
template <range R> auto operator|(R r, rsum_t _) { return accumulate(r.begin(), r.end(), range_value_t<R>()); }
rsum_t rsum;

struct rmax_t {};
template <range R> auto operator|(R r, rmax_t _) {
    auto it = max_element(r.begin(), r.end());
    assert(it != r.end());
    return *it;
}
rmax_t rmax;

struct rmin_t {};
template <range R> auto operator|(R r, rmin_t _) {
    auto it = min_element(r.begin(), r.end());
    assert(it != r.end());
    return *it;
}
rmin_t rmin;

struct ris_sorted_t {};
template <range R> bool operator|(R r, ris_sorted_t _) { return is_sorted(r.begin(), r.end()); }
ris_sorted_t ris_sorted;

struct ris_distinct_t {};
template <range R> bool operator|(R r, ris_distinct_t _) { return adjacent_find(r.begin(), r.end()) == r.end(); }
ris_distinct_t ris_distinct;
