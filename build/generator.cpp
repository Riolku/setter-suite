// Built with `init-template gen_entry` on 2022-05-17
#include <algorithm>
#include <cmath>
#include <random>

#include <iostream>

#include <functional>

#include <cassert>
#include <exception>
#include <stdexcept>

#include <array>
#include <deque>
#include <initializer_list>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;

#define all(x) (x).begin(), (x).end()

namespace Printer {
FILE *stream = stdout;

void set_stream(FILE *st) { stream = st; }

void print_impl(bool x) { fprintf(stream, "%d", x); }
void print_impl(int x) { fprintf(stream, "%d", x); }
void print_impl(ll x) { fprintf(stream, "%lld", x); }
void print_impl(size_t x) { fprintf(stream, "%lu", x); }
void print_impl(const char *x) { fprintf(stream, "%s", x); }
void print_impl(const string &x) { print_impl(x.c_str()); }
void print_impl(char x) { fprintf(stream, "%c", x); }

template <typename A, typename B> void print_impl(const pair<A, B> &p) {
  print_impl(p.first);
  print_impl(' ');
  print_impl(p.second);
}

template <size_t index, typename T>
typename enable_if<index == 0, void>::type print_tuple(const T &t) {}

template <size_t index, typename T>
typename enable_if<index == 1, void>::type print_tuple(const T &t) {
  print_impl(get<tuple_size<T>() - index>(t));
}

template <size_t index, typename T>
typename enable_if<(index > 1), void>::type print_tuple(const T &t) {
  print_impl(get<tuple_size<T>() - index>(t));
  print_impl(' ');
  print_tuple<index - 1>(t);
}

template <typename... Ts> void print_impl(const tuple<Ts...> &t) {
  print_tuple<sizeof...(Ts)>(t);
}

template <typename T> void print_impl(const T &arr) {
  bool first = true;

  for (auto x : arr) {
    if (!first) {
      print_impl(" ");
    }
    first = false;

    print_impl(x);
  }
}

void print_many() {}

template <typename T> void print_many(const T &arg) { print_impl(arg); }

template <typename T, typename... Ts>
void print_many(const T &arg, Ts &&...args) {
  print_impl(arg);
  print_impl(' ');
  print_many(forward<Ts>(args)...);
}

void print() { fprintf(stream, "\n"); }

template <typename... Ts> void print(Ts &&...args) {
  print_many(forward<Ts>(args)...);

  print();
}

template <typename R> void print_items(const R &r) {
  for (auto x : r) {
    print(x);
  }
}
}; // namespace Printer

using Printer::print;
using Printer::print_items;
using Printer::set_stream;

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
    ::for_each(all(*this), [&f](const T &p) -> void {
      auto a = get<0>(p), b = get<1>(p);
      f(a, b);
    });
  }
  template <typename F> void for_each_pair(F f) const {
    ::for_each(all(*this), [&f](const T &p) -> void {
      auto a = get<0>(p), b = get<1>(p);
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
    int x = q.front();
    q.pop();
    f(x);
  }
}

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

using default_rng = mt19937_64;

static default_rng *_random_engine = 0;

void init_engine(ull seed) { _random_engine = new default_rng(seed); }

default_rng &get_engine() {
  if (!_random_engine) {
    throw runtime_error("No engine configured!");
  }

  return *_random_engine;
}

ll randint(ll a, ll b) {
  assert(a <= b);
  return uniform_int_distribution<ll>(a, b)(get_engine());
}

template <typename T = ll> List<T> random_array(int N, T lo, T hi) {
  List<T> ret;
  ret.reserve(N);
  generate_n(back_inserter(ret), N, [lo, hi]() { return randint(lo, hi); });
  return ret;
}

template <typename T> void shuffle(vector<T> &arr) {
  shuffle(all(arr), get_engine());
}

List<int, 1> permutation(int N) {
  Range<int> r(1, N + 1);
  List<int, 1> ret(r.begin(), r.end());
  shuffle(ret);
  return ret;
}

template <typename T>
List<T> random_sorted_array_with_gaps(int N, T lo, T hi, T gap) {
  if (N == 0)
    return List<T>();

  List<T> ret = random_array(N, lo, hi - gap * (N - 1));
  sort(all(ret));
  if (gap != 0) {
    T i = 0;
    transform(ret.begin() + 1, ret.end(), ret.begin() + 1, [&i, gap](T x) {
      i += gap;
      return x + i;
    });
  }
  return ret;
}

template <typename T> List<T> with_gaps(int N, T lo, T hi, T gap) {
  List<T> ret = random_sorted_array_with_gaps(N, lo, hi, gap);
  shuffle(ret);
  return ret;
}

template <typename T> List<T> distinct_array(int N, T lo, T hi) {
  return with_gaps<T>(N, lo, hi, 1);
}

template <typename T> List<T> array_with_sum(int N, T sum, T lo) {
  List<T> ret = random_sorted_array_with_gaps(N - 1, 0, sum, lo);
  ret.reserve(N);
  ret.push_back(sum);
  adjacent_difference(all(ret), ret.begin());
  return ret;
}

template <typename T> class ArrayBuilder {
public:
  using BuilderInstruction = tuple<int, T, T>;
  using BuilderInstructionList = List<BuilderInstruction>;

protected:
  BuilderInstructionList instructions;

public:
  ArrayBuilder(List<BuilderInstruction> instructions)
      : instructions(move(instructions)) {}

  List<T> build() {
    List<T> ret;
    int list_size = 0;
    for (auto tp : instructions) {
      int c, l, r;
      tie(c, l, r) = tp;
      list_size += c;
    }
    ret.reserve(list_size);
    for (auto [c, l, r] : instructions) {
      for (int i = 0; i < c; ++i) {
        ret.push_back(randint(l, r));
      }
    }
    shuffle(ret);
    return ret;
  }
};

using IntArrayBuilder = ArrayBuilder<int>;
using LongArrayBuilder = ArrayBuilder<ll>;

struct Test {
  virtual void generate() = 0;

  virtual ~Test() {}
};

class LiteralTest : public Test {
  string input, output;

public:
  LiteralTest(string input, string output)
      : input(move(input)), output(move(output)) {}

  void generate() override {
    printf("%s", input.c_str());
    fprintf(stderr, "%s", output.c_str());
  }
};

// clang-format off
List<List<shared_ptr<Test>, 1>, 1> cases = {
  {

  }
};
// clang-format on
vector<string> case_count_flags = {"case-counts", "cc", "case_counts"};

int main(int argc, char **argv) {
  assert(argc >= 2);
  if (find(all(case_count_flags), argv[1]) != case_count_flags.end()) {
    print(cases.map_new([](const List<shared_ptr<Test>, 1> &suite) -> size_t {
      return suite.size();
    }));
    return 0;
  }

  assert(argc >= 3);

  int suite = atoi(argv[1]);
  int case_num = atoi(argv[2]);

  init_engine(suite * 1e4 + case_num);

  cases[suite][case_num]->generate();
}

