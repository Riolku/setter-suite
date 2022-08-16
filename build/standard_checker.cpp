// Built with `init-template standard_checker_entry` on 2022-08-16
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

// modified from a template by wleung_bvg

class BaseReader {
  const static size_t MAX_TOKEN_SIZE = 1e7;

private:
  FILE *stream;

  bool hasLast;
  char lastChar;

public:
  BaseReader(FILE *f) : stream(f), hasLast(false), lastChar(0) {}

  BaseReader(const char *path) : BaseReader(fopen(path, "r")) {}

protected:
  char rawPeekChar() {
    if (!hasLast) {
      lastChar = getc(stream);
      hasLast = true;
    }
    return lastChar;
  }

  char rawReadChar() {
    char ret = rawPeekChar();
    hasLast = false;
    return ret;
  }

  bool eof() { return rawPeekChar() == char_traits<char>::eof(); }

public:
  ll readInt(ll lo = numeric_limits<ll>::min(),
             ll hi = numeric_limits<ll>::max()) {
    try {
      ll ret = convertIntegerToken(readToken());
      if (ret < lo || ret > hi)
        internalRangeError();
      return ret;
    } catch (const out_of_range &e) {
      externalRangeError();
    }
    throw runtime_error("We should never get here");
  }

private:
  ll convertIntegerToken(const string &token) {
    if (token.size() == 0)
      invalidIntegerError();
    if (token == "0")
      return 0;

    size_t i = 0;
    if (token[0] == '-') {
      ++i;
      if (token.size() == 1)
        invalidIntegerError();
    }

    if (token[i] > '9' || token[i] < '1')
      invalidIntegerError();
    ++i;

    for (; i < token.size(); ++i) {
      if (token[i] > '9' || token[i] < '0')
        invalidIntegerError();
    }

    return stoll(token);
  }

public:
  char readChar(char min_char = 0, char max_char = 127) {
    preReadChar();
    char c = rawReadChar();
    if (c > max_char || c < min_char) {
      internalRangeError();
    }
    return c;
  }

  string readToken(char min_char = 0, char max_char = 127) {
    preReadToken();
    string token;
    while (!isspace(rawPeekChar()) && !eof() &&
           token.size() <= MAX_TOKEN_SIZE) {
      token.push_back(readChar(min_char, max_char));
    }
    return token;
  }

  char peekChar() {
    preReadChar();
    return rawPeekChar();
  }

  string readString(int N, char min_char = 0, char max_char = 127) {
    preReadString();
    string ret;
    ret.reserve(N);
    for (int i = 0; i < N; ++i) {
      ret.push_back(readChar(min_char, max_char));
    }
    return ret;
  }

private:
  template <typename Iter> void _fill_arr(Iter it, size_t N, ll lo, ll hi) {
    for (size_t i = 0; i < N; i++) {
      if (i)
        readSpace();
      *it = readInt(lo, hi);
      ++it;
    }
    readNewLine();
  }

public:
  template <size_t length, typename T = ll>
  array<T, length> readIntTuple(ll lo = numeric_limits<ll>::min(),
                                ll hi = numeric_limits<ll>::max()) {
    array<T, length> res;
    _fill_arr(res.begin(), length, lo, hi);
    return res;
  }

  template <typename T = ll>
  pair<T, T> readIntPair(ll lo = numeric_limits<ll>::min(),
                         ll hi = numeric_limits<ll>::max()) {
    T a = readInt(lo, hi);
    readSpace();
    T b = readInt(lo, hi);
    readNewLine();
    return {a, b};
  }

  template <typename T = ll>
  T readSingleInt(ll lo = numeric_limits<ll>::min(),
                  ll hi = numeric_limits<ll>::max()) {
    T x = readInt(lo, hi);
    readNewLine();
    return x;
  }

  template <typename T = ll>
  vector<T> readIntArray(size_t N, ll lo = numeric_limits<ll>::min(),
                         ll hi = numeric_limits<ll>::max()) {
    vector<T> v;
    v.reserve(N);
    _fill_arr(back_inserter(v), N, lo, hi);
    return v;
  }

  template <typename T = ll>
  pair<vector<T>, int>
  readIntArrayOrFlag(size_t N, ll lo = numeric_limits<ll>::min(),
                     ll hi = numeric_limits<ll>::max(), T flag = -1) {
    T first = readInt();
    if (first == flag) {
      readNewLine();
      return {vector<T>(), -1};
    }

    if (lo > first || first > hi) {
      internalRangeError();
      throw runtime_error("We should never get here");
    } else {
      vector<T> v;
      v.reserve(N);
      v.push_back(first);
      if (N > 1) {
        readSpace();
      }
      _fill_arr(back_inserter(v), N - 1, lo, hi);
      return {v, N};
    }
  }

  template <typename T, typename... Ts> void readInts(T &arg, Ts &&...args) {
    arg = readInt();
    readSpace();
    readInts(args...);
  }

  template <typename T> void readInts(T &arg) {
    arg = readInt();
    readNewLine();
  }

  virtual ~BaseReader() {}

protected:
  virtual void preReadToken() {}
  virtual void preReadString() { preReadToken(); }
  virtual void preReadChar() { preReadToken(); }
  virtual void internalRangeError() = 0;
  virtual void externalRangeError() = 0;
  virtual void invalidIntegerError() = 0;
  virtual void wrongWhitespaceError() = 0;

public:
  virtual void readNewLine() = 0;
  virtual void readSpace() = 0;
};

template <typename Parent> class ExactWhitespaceMixin : public Parent {
  void readEOF() {
    if (!Parent::eof())
      Parent::wrongWhitespaceError();
  }

public:
  void readNewLine() override {
    if (Parent::rawReadChar() != '\n')
      Parent::wrongWhitespaceError();
  }

  void readSpace() override {
    if (Parent::rawReadChar() != ' ')
      Parent::wrongWhitespaceError();
  }

  ~ExactWhitespaceMixin() { readEOF(); }

  using Parent::Parent;
};

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
    T x(move(q.front()));
    q.pop();
    f(x);
  }
}

void assert_permutation(const List<int, 1> &arr) {
  List<int, 1> check(arr.size(), 0);
  assert(arr.all_of([&check](int x) { return ++check[x] == 1; }));
}

class ValidatingReaderBase : public BaseReader {
protected:
  void externalRangeError() override { throw runtime_error("EXTERNAL_RANGE"); }
  void internalRangeError() override { throw runtime_error("INTERNAL_RANGE"); }
  void wrongWhitespaceError() override {
    throw runtime_error("WRONG_WHITESPACE");
  }
  void invalidIntegerError() override {
    throw runtime_error("INVALID_INTEGER");
  }

public:
  using BaseReader::BaseReader;
};

using ValidatingReader = ExactWhitespaceMixin<ValidatingReaderBase>;

namespace CheckerCodes {
int AC = 0;
int WA = 1;
int PE = 2;
int IE = 3;
int PARTIAL = 7;
} // namespace CheckerCodes

void preErrorHook();

void assertOrCode(bool cond, int code) {
  if (!cond) {
    preErrorHook();
    exit(code);
  }
}
void assertWA(bool cond) { assertOrCode(cond, CheckerCodes::WA); }
void assertPE(bool cond) { assertOrCode(cond, CheckerCodes::PE); }

int partial(int points, int denom) {
  fprintf(stderr, "partial %d/%d\n", points, denom);
  printf("%d/%d points", points, denom);
  return CheckerCodes::PARTIAL;
}

class CheckerReader : public BaseReader {
protected:
  virtual void preError() { preErrorHook(); }
  void externalRangeError() override {
    preError();
    exit(CheckerCodes::WA);
  }
  void internalRangeError() override {
    preError();
    exit(CheckerCodes::WA);
  }
  void wrongWhitespaceError() override {
    preError();
    exit(CheckerCodes::PE);
  }
  void invalidIntegerError() override {
    preError();
    exit(CheckerCodes::PE);
  }

public:
  using BaseReader::BaseReader;
};

class StandardCheckerReader : public CheckerReader {
  // 0 - don't skip space
  // 1 - skip non-lines
  // 2 - skip all space
  // 3 - same as 2, but don't error if there is no space
  int whitespace_flag;

  bool isLine(char c) { return c == '\n' || c == '\r'; }
  bool isNonLineWhitespace(char c) {
    return c == '\v' || c == '\f' || c == '\t' || c == ' ';
  }
  bool isWhite(char c) { return isLine(c) || isNonLineWhitespace(c); }

  bool skipAllWhitespace() {
    bool any_line = false;
    while (isWhite(rawPeekChar()) && !eof())
      any_line |= isLine(rawReadChar());
    return any_line;
  }

  void readEOF() {
    skipAllWhitespace();
    if (!eof())
      wrongWhitespaceError(); // wanted EOF but found more token
  }

public:
  StandardCheckerReader(FILE *f) : CheckerReader(f), whitespace_flag(3) {}
  StandardCheckerReader(const char *path)
      : CheckerReader(path), whitespace_flag(3) {}

  void readSpace() override { whitespace_flag = 1; }
  void readNewLine() override { whitespace_flag = 2; }

  void preReadToken() override {
    if (whitespace_flag == 1) {
      if (!isNonLineWhitespace(rawReadChar())) {
        wrongWhitespaceError();
      }

      while (isNonLineWhitespace(rawPeekChar())) {
        rawReadChar();
      }
    } else if (whitespace_flag == 2 || whitespace_flag == 3) {
      if (!skipAllWhitespace() && whitespace_flag != 3) {
        wrongWhitespaceError();
      }
    } else {
      assert(whitespace_flag == 0);
    }
    whitespace_flag = 0;
  }

  // PE could be confusing under standard checker
  void wrongWhitespaceError() override {
    preError();
    exit(CheckerCodes::WA);
  }

  void invalidIntegerError() override {
    preError();
    exit(CheckerCodes::WA);
  }

  ~StandardCheckerReader() { readEOF(); }
};

// Hook for all library functions to call before exiting on error.
void preErrorHook() {}

int main(int argc, char **argv) {
  assert(argc >= 4);
  StandardCheckerReader user_r(argv[2]);
  ValidatingReader in_r(argv[1]), ref_r(argv[3]);
}

