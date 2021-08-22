template<typename T>
struct List;

template<typename T>
List<T> range(T x);

template<typename T>
List<T> range(T l, T r);

template<typename T1, typename T2>
List<pair<T1, T2>> zip(const List<T1>& a, const List<T2>& b);

template<typename T>
struct List : public vector<T> {
  int index_start = 0;

  using vector<T>::vector;

  template<typename O>
  List(O other) requires Iterable<O> : List(other.begin(), other.end()) {}

  int get_start() {
    return index_start;
  }

  int get_end() {
    return index_start + this->size();
  }

  List<T>& set_index(int x) {
    index_start = x;

    return *this;
  }

  List<T>& zero_indexed() {
    return set_index(0);
  }

  List<T>& one_indexed() {
    return set_index(1);
  }

  T& operator[](int x) {
    return this->at(x - index_start);
  }

  const T& operator[](int x) const {
    return this->at(x - index_start);
  }

  List<T>& append(T other) {
    this->push_back(other);

    return *this;
  }

  template<typename OT>
  List<T>& extend(OT other) requires Iterable<OT> {
    this->insert(this->end(), other.begin(), other.end());

    return *this;
  }

  template<typename OT>
  List<T>& operator+=(OT other) requires Iterable<OT> {
    return extend(other);
  }

  template<typename OT>
  List<T> operator+(OT other) requires Iterable<OT> {
    List<T> ret(this->begin(), this->end());

    return ret.extend(other).set_index(index_start);
  }

  template<typename NT>
  List<T> operator*(NT times) requires is_integral<NT>::value {
    List<T> ret;

    auto n = this->size();

    ret.resize(times * n);

    for(int i = 0; i < ret.size(); i++) {
      ret.at(i) = this->at(i % n + index_start);
    }

    return ret.set_index(index_start);
  }

  template<typename R, typename F>
  List<R> map(F f) const requires is_function<F>::value {
    List<R> ret;

    ret.resize(this->size());

    transform(this->begin(), this->end(), ret.begin(), f);

    return ret.set_index(index_start);
  }

  template<typename F>
  List<T>& forEach(F f) requires is_function<F>::value {
    for_each(this->begin(), this->end(), f);

    return *this;
  }

  size_t count(T v) const {
    return ::count(this->begin(), this->end(), v);
  }

  List<T>& reverse() {
    ::reverse(this->begin(), this->end());

    return *this;
  }

  template<typename F>
  List<T> filter(F f) requires Predicate<F, T> {
    List<T> cpy(this->begin(), this->end());

    cpy.erase(remove_if(cpy.begin(), cpy.end(), [&](const T& v) {
      return !f(v);
    }), cpy.end());

    return cpy.set_index(index_start);
  }

  List<T>& removeAll(T v) {
    this->erase(remove(this->begin(), this->end(), v), this->end());

    return *this;
  }

  List<T> slice(int from) const {
    return List<T>(this->begin() + from - index_start, this->end()).set_index(index_start);
  }

  List<T> slice(int start, int end) const {
    return List<T>(
      this->begin() + start - index_start,
      this->begin() + end - index_start
    ).set_index(index_start);
  }

  List<pair<T, int>> enumerate() const {
    return zip(*this, range(index_start, index_start + this->size())).set_index(index_start);
  }

  List<T>& sort() {
    ::sort(this->begin(), this->end());

    return *this;
  }

  template<typename F>
  List<T>& sortBy(F f) requires Predicate<F, T> {
    ::sort(this->begin(), this->end(), [&](const T& a, const T& b) {
      return f(a) < f(b);
    });

    return *this;
  }

  bool isSorted() const {
    return is_sorted(this->begin(), this->end());
  }

  List<T> getDistinct() const {
    List<T> cpy(this->begin(), this->end());

    cpy.sort();

    cpy.erase(unique(cpy.begin(), cpy.end()), cpy.end());

    return cpy.set_index(index_start);
  }

  bool isDistinct() const {
    List<T> cpy = *this;

    cpy.sort();

    return adjacent_find(cpy.begin(), cpy.end()) == cpy.end();
  }

  T sum() const {
    T ans = T();

    for(const T& x : *this) {
      ans += x;
    }

    return ans;
  }

  T max() const {
    assert(this->size() > 0);

    return *max_element(this->begin(), this->end());
  }

  T min() const {
    assert(this->size() > 0);

    return *min_element(this->begin(), this->end());
  }
};

template<typename T>
List<T> range(T x) {
  return range(T(), x);
}

template<typename T>
List<T> range(T l, T r) {
  return range(l, r, 1);
}

template<typename T>
List<T> range(T l, T r, T i) {
  List<T> ret;

  for(; l < r; l += i) {
    ret.append(l);
  }

  return ret;
}

template<typename T1, typename T2>
List<pair<T1, T2>> zip(const List<T1>& a, const List<T2>& b) {
  auto x = range(min(a.size(), b.size())).template map<pair<T1, T2>>([&](int x) {
    return make_pair(a.at(x), b.at(x));
  });

  if(a.index_start == b.index_start) {
    x.set_index(a.index_start);
  }

  return x;
}
