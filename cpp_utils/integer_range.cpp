template <typename T = ll> class Range {
  T l, r;

  class iterator {
    T cur;

    iterator(T cur) : cur(move(cur)) {}

  public:
    using difference_type = decltype(declval<T>() - declval<T>());
    using value_type = T;
    using pointer = void;
    using reference = void;
    using iterator_category = random_access_iterator_tag;

    iterator &operator++() {
      ++cur;
      return *this;
    }
    iterator &operator--() {
      --cur;
      return *this;
    }
    value_type operator*() { return cur; }
    value_type operator[](const T &offset) { return cur + offset; }

    iterator operator+(const T &offset) const { return iterator(cur + offset); }
    iterator operator-(const T &offset) const { return iterator(cur - offset); }
    difference_type operator-(const iterator &other) const {
      return cur - other.cur;
    }
    iterator &operator-=(const T &offset) {
      cur -= offset;
      return *this;
    }
    iterator &operator+=(const T &offset) {
      cur += offset;
      return *this;
    }

    bool operator>=(const iterator &other) const { return cur >= other.cur; }
    bool operator<=(const iterator &other) const { return cur <= other.cur; }
    bool operator>(const iterator &other) const { return cur > other.cur; }
    bool operator<(const iterator &other) const { return cur < other.cur; }
    bool operator!=(const iterator &other) const { return cur != other.cur; }
    bool operator==(const iterator &other) const { return cur == other.cur; }

    friend class Range;
  };

public:
  Range(T l, T r) : l(move(l)), r(move(r)) {}
  Range(T r) : l(T()), r(move(r)) {}

  template <typename F> bool any_of(F f) const {
    return ::any_of(all(*this), f);
  }
  template <typename F> bool all_of(F f) const {
    return ::any_of(all(*this), f);
  }

  template <typename F> void for_each(F f) const { ::for_each(all(*this), f); }

  template <typename F>
  auto map_new(F f) const -> List<decltype(f(declval<T>()))> {
    List<decltype(f(declval<T>()))> ret;
    ret.reserve(this->size());
    ::transform(all(*this), back_inserter(ret), f);
    return ret;
  }

  decltype(declval<T>() - declval<T>()) size() const { return r - l; }
  iterator begin() const { return iterator(l); }
  iterator end() const { return iterator(r); }
};
