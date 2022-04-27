// Built with `init-template validator_entry` on 2022-04-27
#include <algorithm>
#include <cmath>
#include <random>

#include <iostream>

#include <functional>

#include <cassert>
#include <exception>
#include <stdexcept>

#include <array>
#include <initializer_list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;

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
  bool streamOpen;

  bool hasLast;
  char lastChar;

public:
  BaseReader(FILE *f)
      : stream(f), streamOpen(true), hasLast(false), lastChar(0) {}

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
  string readToken() {
    preReadToken();
    string token;
    while (!isspace(rawPeekChar()) && !eof() && token.size() <= MAX_TOKEN_SIZE)
      token.push_back(rawReadChar());
    return token;
  }

  char peekChar() {
    preReadChar();
    return rawPeekChar();
  }

  char readChar(char min_char = 0, char max_char = 127) {
    preReadChar();
    char c = rawReadChar();
    if (c > max_char || c < min_char) {
      internalRangeError();
    }
    return c;
  }

  string readString(int N, char min_char = 0, char max_char = 127) {
    preReadString();
    string ret;
    ret.reserve(N);
    while (ret.size() < N)
      ret.push_back(readChar(min_char, max_char));
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
  virtual void readEOF() = 0;
};

template <typename Parent> class ExactWhitespaceMixin : public Parent {
public:
  void readNewLine() override {
    if (Parent::rawReadChar() != '\n')
      Parent::wrongWhitespaceError();
  }

  void readEOFImpl() {
    if (!Parent::eof())
      Parent::wrongWhitespaceError();
  }
  void readEOF() override { readEOFImpl(); }

  void readSpace() override {
    if (Parent::rawReadChar() != ' ')
      Parent::wrongWhitespaceError();
  }

  ~ExactWhitespaceMixin() { readEOFImpl(); }

  using Parent::Parent;
};

struct IntegerValidation {
  ll lo, hi;

  bool check(ll x) { return lo <= x && x <= hi; }
  template <typename R> bool check(const R &r) {
    for (auto x : r)
      if (!check(x))
        return false;
    return true;
  }

  template <typename T, typename... Ts> bool check(const T &arg, Ts &&...args) {
    return check(arg) && check(args...);
  }
};

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

ValidatingReader in_r(stdin);

