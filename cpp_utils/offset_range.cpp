template <range R, typename offset_t> struct offset_range {
    R r;
    offset_t offset;
    auto operator[](offset_t index) { return r[index - offset]; }
};

template <typename offset_t> struct make_offset_t { offset_t offset; };
template <range R, typename offset_t> offset_range<R, offset_t> operator|(R r, make_offset_t<offset_t> mot) {
    return offset_range{std::move(r), std::move(mot)};
}
make_offset_t one_indexed{1};
