// Built with `init-template gen_entry` on 2023-06-13
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

// Simple Types
void print_impl(bool x) { fprintf(stream, "%d", x); }
void print_impl(int x) { fprintf(stream, "%d", x); }
void print_impl(ll x) { fprintf(stream, "%lld", x); }
void print_impl(size_t x) { fprintf(stream, "%lu", x); }
void print_impl(const char *x) { fprintf(stream, "%s", x); }
void print_impl(const string &x) { print_impl(x.c_str()); }
void print_impl(char x) { fprintf(stream, "%c", x); }

// Recursive types. First, declare all overloads
template <typename T> void print_impl(const T &);
template <typename A, typename B> void print_impl(const pair<A, B> &);
template <typename... Ts> void print_impl(const tuple<Ts...> &t);
template <typename T, typename Container> void print_impl(queue<T, Container>);
template <typename T, typename Container, typename Compare>
void print_impl(priority_queue<T, Container, Compare>);
template <typename T> void print_impl(const set<T> &);
template <typename T> void print_impl(const vector<T> &);
template <typename T, size_t size> void print_impl(const array<T, size> &);
template <typename T> void print_impl(const unordered_set<T> &);

// Recursive type definitions
template <typename T> void print_impl(const T &x) {
  print_impl(x.get_printable());
}

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

template <typename T> void print_iterable(const T &arr) {
  bool first = true;

  for (auto x : arr) {
    if (!first) {
      print_impl(" ");
    }
    first = false;

    print_impl(x);
  }
}

template <typename T> void print_impl(const set<T> &arr) {
  print_iterable(arr);
}
template <typename T> void print_impl(const vector<T> &arr) {
  print_iterable(arr);
}
template <typename T> void print_impl(const unordered_set<T> &arr) {
  print_iterable(arr);
}
template <typename T, size_t size> void print_impl(const array<T, size> &arr) {
  print_iterable(arr);
}
template <typename T, typename Container>
void print_impl(queue<T, Container> q) {
  vector<T> dummy;
  dummy.reserve(q.size());
  while (!q.empty()) {
    dummy.push_back(move(q.front()));
    q.pop_front();
  }
  print_impl(dummy);
}

template <typename T, typename Container, typename Compare>
void print_impl(priority_queue<T, Container, Compare> pq) {
  vector<T> dummy;
  dummy.reserve(pq.size());
  while (!pq.empty()) {
    dummy.push_back(move(pq.top()));
    pq.pop();
  }
  print_impl(dummy);
}
// print_many helper
template <typename T> void print_many(const T &arg) { print_impl(arg); }
template <typename T, typename... Ts>
void print_many(const T &arg, Ts &&...args) {
  print_impl(arg);
  print_impl(' ');
  print_many(forward<Ts>(args)...);
}

template <typename... Ts> void print(Ts &&...args) {
  print_many(forward<Ts>(args)...);

  print();
}
template <> void print() { fprintf(stream, "\n"); }
template <typename T> void print_map(const T &mp) {
  for (auto p : mp) {
    print(p);
  }
}

template <typename R> void print_items(const R &r) {
  for (auto x : r) {
    print(x);
  }
}

template <typename K, typename V>
void print_items(const unordered_map<K, V> &mp) {
  print_map(mp);
}
template <typename K, typename V> void print_items(const map<K, V> &mp) {
  print_map(mp);
}

template <typename T> void print_yes_no(const T &xs) {
  for (const auto &x : xs)
    print_yes_no(x);
}
template <> void print_yes_no(const bool &x) { print(x ? "YES" : "NO"); }
}; // namespace Printer

using Printer::print;
using Printer::print_items;
using Printer::print_yes_no;
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
    for (const T &elem : other)
      this->push_back(elem);
    return *this;
  }
  List<T, offset> &operator+=(List<T, offset> &&other) {
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
  List<T> ret = random_sorted_array_with_gaps(N - 1, lo, sum - lo, lo);
  ret.reserve(N);
  ret.push_back(sum);
  adjacent_difference(all(ret), ret.begin());
  return ret;
}

List<int> binary_array_with_sum(int N, int one_count) {
  assert(one_count <= N);
  auto indices = distinct_array<int>(one_count, 1, N);
  List<int, 1> ret(N, 0);
  for (int x : indices)
    ret[x] = 1;

  return ret;
}

template <typename T> class RandomArrayBuilder {
public:
  using BuilderInstruction = tuple<int, T, T>;
  using BuilderInstructionList = List<BuilderInstruction>;

protected:
  BuilderInstructionList instructions;

public:
  RandomArrayBuilder(BuilderInstructionList instructions)
      : instructions(move(instructions)) {}

  List<T> build() {
    List<T> ret;
    int list_size = 0;
    for (auto tp : instructions) {
      list_size += get<0>(tp);
    }
    ret.reserve(list_size);
    for (auto tp : instructions) {
      int c;
      T l, r;
      tie(c, l, r) = tp;
      for (int i = 0; i < c; ++i) {
        ret.push_back(randint(l, r));
      }
    }
    shuffle(ret);
    return ret;
  }
};

using IntArrayBuilder = RandomArrayBuilder<int>;
using LongArrayBuilder = RandomArrayBuilder<ll>;

template <typename T> struct ListBuilder {
  virtual List<T> build() = 0;

  virtual ~ListBuilder() {}
};

template <typename T> class SingleElement : public ListBuilder<T> {
  T x;

public:
  List<T> build() override { return {x}; }

  SingleElement(T x) : x(move(x)) {}
};

template <typename T> class MultipleElements : public ListBuilder<T> {
  List<T> elems;

public:
  List<T> build() override { return elems; }

  MultipleElements(List<T> elems) : elems(move(elems)) {}
};

template <typename T> class SerialElements : public ListBuilder<T> {
  using PartType = ListBuilder<T>;
  using PartPtr = unique_ptr<PartType>;
  using PartList = List<PartPtr>;

  PartList to_parts(PartList cur) { return cur; }
  template <typename... Ts>
  PartList to_parts(PartList cur, PartPtr ptr, Ts &&...rest) {
    cur.push_back(move(ptr));
    return to_parts(move(cur), forward<Ts>(rest)...);
  }
  template <typename... Ts>
  PartList to_parts(PartList cur, T elem, Ts &&...rest) {
    return to_parts(move(cur), make_unique<SingleElement<T>>(move(elem)),
                    forward<Ts>(rest)...);
  }

  PartList parts;

public:
  template <typename... Ts>
  SerialElements(Ts &&...rest)
      : parts(to_parts(PartList{}, forward<Ts>(rest)...)) {}

  List<T> build() override {
    List<T> ret;
    for (const auto &part : parts) {
      ret += part->build();
    }
    return ret;
  }
};

template <typename T> class RepeatElement : public ListBuilder<T> {
  int N;
  unique_ptr<ListBuilder<T>> part;

public:
  RepeatElement(int N, unique_ptr<ListBuilder<T>> part)
      : N(N), part(move(part)) {}

  RepeatElement(int N, T elem)
      : RepeatElement(N, make_unique<SingleElement<T>>(move(elem))) {}

  List<T> build() {
    List<T> ret;
    for (int i = 0; i < N; ++i) {
      ret += part->build();
    }
    return ret;
  }
};

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

class SolutionTestBase : public Test {
  virtual void get_input() = 0;
  virtual void print_input() = 0;
  virtual void solve_input() = 0;

public:
  void generate() override {
    get_input();
    set_stream(stdout);
    print_input();
    set_stream(stderr);
    solve_input();
  }
};

// For the classic "T tests" problems
class MultipleTests : public Test {
  unique_ptr<ListBuilder<shared_ptr<Test>>> builder;

public:
  MultipleTests(unique_ptr<ListBuilder<shared_ptr<Test>>> builder)
      : builder(move(builder)) {}

  void generate() override {
    auto tests = builder->build();
    shuffle(tests);
    print(tests.size());
    for (const auto &test : tests) {
      test->generate();
    }
  }
};

template <typename T> class RandomTestUnion : public ListBuilder<T> {
  T first, second;
  int num, denom;

public:
  RandomTestUnion(T first, T second, int num = 1, int denom = 2)
      : first(move(first)), second(move(second)), num(num), denom(denom) {}

  List<T> build() { return {randint(1, denom) <= num ? first : second}; }
};

class MultipleTestsWithSumUnit : public SolutionTestBase {
public:
  // When tests are duplicated, they share the same memory, and as such doing
  // initialization will overwrite The idea is to get the unit value which may
  // be constant, but is likely random, (but still does not modify the object)
  // And then store it before calling generate()
  virtual int get_unit_value() const = 0;
  virtual void store_unit_value(int) = 0;
};

class MultipleTestsWithSum : public Test {
  int max_total;
  unique_ptr<ListBuilder<shared_ptr<MultipleTestsWithSumUnit>>> builder;

public:
  MultipleTestsWithSum(
      int max_total,
      unique_ptr<ListBuilder<shared_ptr<MultipleTestsWithSumUnit>>> builder)
      : max_total(max_total), builder(move(builder)) {}

  void generate() override {
    List<shared_ptr<MultipleTestsWithSumUnit>, 1> tests = builder->build();
    int cur_total = 0;
    int test_count = 0;
    List<int, 1> test_sizes(tests.size(), -1);

    // Basically do greedy knapsack
    for (size_t ti = 1; ti <= tests.size(); ++ti) {
      int v = tests[ti]->get_unit_value();
      if (cur_total + v <= max_total) {
        ++test_count;
        cur_total += v;
        test_sizes[ti] = v;
      }
    }
    print(test_count);
    List<pair<shared_ptr<MultipleTestsWithSumUnit>, int>, 1> used_tests;
    used_tests.reserve(test_count);
    for (size_t ti = 1; ti <= tests.size(); ++ti) {
      int sz = test_sizes[ti];
      if (sz != -1) {
        used_tests.emplace_back(tests[ti], sz);
      }
    }
    shuffle(used_tests);

    for (auto [test, sz] : used_tests) {
      test->store_unit_value(sz);
      test->generate();
    }
  }
};

// clang-format off
List<List<shared_ptr<Test>, 1>, 1> cases = {
  {

  }
};
// clang-format on

const int _PAGE_SZ = 4096;
const int _BUF_SZ = _PAGE_SZ * 16;
char _out_buf[_BUF_SZ], _err_buf[_BUF_SZ];

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
  assert(setvbuf(stdout, _out_buf, _IOFBF, _BUF_SZ) == 0);
  assert(setvbuf(stderr, _err_buf, _IOFBF, _BUF_SZ) == 0);

  int suite = atoi(argv[1]);
  int case_num = atoi(argv[2]);

  init_engine(suite * 1e4 + case_num);

  cases[suite][case_num]->generate();
}

