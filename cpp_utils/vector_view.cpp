template <typename T> struct owns_vector_view : public ranges::view_interface<owns_vector_view<T>> {
    shared_ptr<const vector<T>> p;
    owns_vector_view() = default;
    owns_vector_view(vector<T> v) : p(make_shared<vector<T>>(std::move(v))) {}
    auto begin() const { return p->begin(); }
    auto end() const { return p->end(); }
};

struct vector_view_tag {};
template <typename T> owns_vector_view<T> operator|(vector<T> &&v, vector_view_tag _) {
    return owns_vector_view(std::move(v));
}
template <typename T> auto operator|(const vector<T> &v, vector_view_tag _) { return ref_view(v); }
vector_view_tag to_view;

struct to_vector_t {};
template <range R> auto operator|(R r, to_vector_t _) requires ranges::sized_range<R> {
    vector<range_value_t<R>> v;
    v.reserve(rsize(r));
    for (auto x : r)
        v.push_back(x);
    return v;
}
template <range R> auto operator|(R r, to_vector_t _) {
    vector<range_value_t<R>> v;
    for (auto x : r)
        v.push_back(x);
    return v;
}
to_vector_t to_vector;
