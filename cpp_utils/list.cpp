template <typename T, int offset = 0> class List : public vector<T> {
public:
  using vector<T>::vector;
  List(vector<T> v) : vector<T>::vector(move(v)) {}
  template <int other_offset>
  List(List<T, other_offset> other) : vector<T>::vector(move(other)) {}
  T &operator[](size_t x) { return this->at(x - offset); }
  const T &operator[](size_t x) const { return this->at(x - offset); }

  template <typename F> void for_each(F f) { ::for_each(all(*this), f); }
  template <typename F> void for_each(F f) const { ::for_each(all(*this), f); }
  template <typename F> void for_each_pair(F f) {
    ::for_each(all(*this), [&f](T &p) -> void {
      decltype(p.first) &a = get<0>(p);
      decltype(p.second) &b = get<1>(p);
      f(a, b);
    });
  }
  template <typename F> void for_each_pair(F f) const {
    ::for_each(all(*this), [&f](const T &p) -> void {
      const decltype(p.first) &a = get<0>(p);
      const decltype(p.second) &b = get<1>(p);
      f(a, b);
    });
  }
  template <typename F> void for_each_enumerate(F f) {
    size_t i = offset;
    for (auto it = this->begin(); it != this->end(); ++it) {
      f(*it, i);
      ++i;
    }
  }
  template <typename F> void for_each_enumerate(F f) const {
    size_t i = offset;
    for (auto it = this->begin(); it != this->end(); ++it) {
      f(*it, i);
      ++i;
    }
  }
  template <typename F> void for_each_adjacent(F f) const {
    if (this->size() == 0)
      return;
    auto it1 = this->begin();
    auto it2 = next(it1);
    while (it2 != this->end()) {
      f(*it1, *it2);
      ++it1;
      ++it2;
    }
  }
  template <typename F> void for_each_adjacent(F f) {
    if (this->size() == 0)
      return;
    auto it1 = this->begin();
    auto it2 = next(it1);
    while (it2 != this->end()) {
      F(*it1, *it2);
      ++it1;
      ++it2;
    }
  }

  List<T, offset> &operator+=(const List<T, offset> &other) {
    this->reserve(this->size() + other.size());
    for (const T &elem : other)
      this->push_back(elem);
    return *this;
  }
  List<T, offset> &operator+=(List<T, offset> &&other) {
    this->reserve(this->size() + other.size());
    for (auto &&elem : other)
      this->push_back(move(elem));
    return *this;
  }
  List<T, offset> operator+(const List<T, offset> &other) const & {
    List<T, offset> ret(this->begin(), this->end());
    return ret += other;
  }
  List<T, offset> operator+(List<T, offset> &&other) const & {
    List<T, offset> ret(this->begin(), this->end());
    return ret += move(other);
  }
  List<T, offset> operator+(const List<T, offset> &other) && {
    return *this += other;
  }
  List<T, offset> operator+(List<T, offset> &&other) && {
    return *this += move(other);
  }

  template <typename F> bool any_of(F f) const {
    return ::any_of(all(*this), f);
  }
  template <typename F> bool all_of(F f) const {
    return ::any_of(all(*this), f);
  }

  T max() const {
    assert(this->size() > 0);
    return *::max_element(this->begin(), this->end());
  }
  T max(const T &init) const {
    if (this->size() == 0)
      return init;
    return max();
  }
  T max(T &&init) const {
    if (this->size() == 0)
      return move(init);
    return max();
  }

  T min() const {
    assert(this->size() > 0);
    return *::min_element(this->begin(), this->end());
  }
  T min(const T &init) const {
    if (this->size() == 0)
      return init;
    return min();
  }
  T min(T &&init) const {
    if (this->size() == 0)
      return move(init);
    return min();
  }

  T sum(T start = T()) const {
    return ::accumulate(this->begin(), this->end(), move(start));
  }

  List<T, offset> &sort() {
    ::sort(all(*this));
    return *this;
  }

  List<T, offset> &reverse() {
    ::reverse(all(*this));
    return *this;
  }

  template <typename F> List<T, offset> &map(F f) {
    ::transform(all(*this), this->begin(), f);
    return *this;
  }

  template <typename F>
  auto map_new(F f) const -> List<decltype(f(declval<T>())), offset> {
    List<decltype(f(declval<T>())), offset> ret;
    ret.reserve(this->size());
    ::transform(all(*this), back_inserter(ret), f);
    return ret;
  }

  template <typename F>
  auto map_adjacent_new(F f) const
      -> List<decltype(f(declval<T>(), declval<T>())), offset> {
    List<decltype(f(declval<T>(), declval<T>())), offset> ret;
    if (this->size() == 0)
      return ret;
    ret.reserve(this->size() - 1);
    auto it1 = this->begin();
    auto it2 = next(it1);
    while (it2 != this->end()) {
      ret.push_back(f(*it1, *it2));
      ++it1;
      ++it2;
    }
    return ret;
  }

  const vector<T> &get_printable() const { return *this; }
};

template <int offset = 0, typename F>
auto generate(int N, F f) -> List<decltype(f()), offset> {
  List<decltype(f()), offset> ret;
  ret.reserve(N);
  ::generate_n(back_inserter(ret), N, move(f));
  return ret;
}

template <typename T, int offset>
List<T, offset> interleave(List<T, offset> A, List<T, offset> B) {
  assert(A.size() == B.size() || A.size() == B.size() + 1);
  List<T, offset> ret;
  ret.reserve(A.size() + B.size());

  auto ait = A.begin();
  auto bit = B.begin();

  while (ait != A.end() && bit != B.end()) {
    ret.push_back(*ait);
    ret.push_back(*bit);
    ++ait;
    ++bit;
  }

  if (ait != A.end()) {
    ret.push_back(*ait);
    ++ait;
  }
  assert(ait == A.end());
  assert(bit == B.end());
  return ret;
}

template <typename T, typename F> void exhaust_queue(queue<T> &q, F f) {
  while (!q.empty()) {
    T x(move(q.front()));
    q.pop();
    f(move(x));
  }
}
