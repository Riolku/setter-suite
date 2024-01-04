// Built with `init-template validator_entry` on 2024-01-04
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

void assert_permutation(const vector<int> &arr) {
  vector<int> check(arr.size(), 0);
  assert(std::all_of(arr.begin(), arr.end(), [&check](int x) { return ++check[x - 1] == 1; }));
}

class validator_out_of_range {};
class wrong_whitespace {};
class invalid_integer {};

class ValidatingReaderBase : public BaseReader {
protected:
  void externalRangeError() override { throw validator_out_of_range(); }
  void internalRangeError() override { throw validator_out_of_range(); }
  void wrongWhitespaceError() override { throw wrong_whitespace(); }
  void invalidIntegerError() override { throw invalid_integer(); }

public:
  using BaseReader::BaseReader;
};

using ValidatingReader = ExactWhitespaceMixin<ValidatingReaderBase>;

ValidatingReader in_r(stdin);

