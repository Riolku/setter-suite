#include <iostream>
#include <limits>
#include <vector>
#include <array>
using namespace std;

// modified from a template by wleung_bvg

using ll=long long;using ld=long double;

namespace BasicReader {
  bool _hasLast = false;
  char _last;

  enum error_type {INTERNAL_RANGE, EXTERNAL_RANGE, INVALID_ARGUMENT, WRONG_CHAR};

  typedef void(*error_handler)(enum error_type e);

  void default_handler(error_type e) {
    throw runtime_error(to_string(e));
  }

  [[noreturn]] error_handler handler = default_handler;

  void set_error_handler(error_handler f) {
    handler = f;
  }

  char _peekChar() {
    _last = _hasLast ? _last : getchar();
    _hasLast = true;
    return _last;
  }

  char _getChar() {
    char ret = _peekChar();
    _hasLast = false;
    return ret;
  }

  ll readInt(ll minValid, ll maxValid) {
    string token = "";
    while (isdigit(_peekChar()) || _peekChar() == '-') token.push_back(_getChar());
    try {
      ll ret = stoll(token);

      if(minValid > ret || maxValid < ret) handler(INTERNAL_RANGE);

      return ret;
    }
    catch(out_of_range e) {
      handler(EXTERNAL_RANGE);
    }
    catch(invalid_argument e) {
      handler(INVALID_ARGUMENT);
    }
  }

  ld readFloat(ld minValid, ld maxValid) {
    string token = "";
    while (isdigit(_peekChar()) || _peekChar() == '-' || _peekChar() == '.') token.push_back(_getChar());

    try {
      ld ret = stold(token);

      if(minValid > ret || maxValid < ret) handler(INTERNAL_RANGE);

      return ret;
    }
    catch(out_of_range e) {
      handler(EXTERNAL_RANGE);
    }
    catch(invalid_argument e) {
      handler(INVALID_ARGUMENT);
    }
  }

  string readLine() {
    string token = "";
    while (_peekChar() != '\n') token.push_back(_getChar());
    _getChar();

    return token;
  }

  void readSpace() {
    if(_getChar() != ' ') handler(WRONG_CHAR);
  }

  void readNewLine() {
    if(_getChar() != '\n') handler(WRONG_CHAR);
  }

  void readEOF() {
    if(_getChar() != char_traits<char>::eof()) handler(WRONG_CHAR);
  }
}

using BasicReader::readInt;
using BasicReader::readFloat;
using BasicReader::readLine;
using BasicReader::readSpace;
using BasicReader::readNewLine;
using BasicReader::readEOF;

namespace AdvancedReader {
  template<typename T>
  T& _fill_arr(T& res, int N, ll lo, ll hi) {
    for(int i = 0; i < N; i++) {
      res[i] = BasicReader::readInt(lo, hi);

      if(i == N - 1) BasicReader::readNewLine();
      else BasicReader::readSpace();
    }

    return res;
  }

  template<int length>
  array<ll, length> readLongTuple(ll lo, ll hi) {
    array<ll, length> res;

    return _fill_arr(res, length, lo, hi);
  }

  vector<ll> readLongArray(int N, ll lo, ll hi) {
    vector<ll> res(N, 0);

    return _fill_arr(res, N, lo, hi);
  }
}

using AdvancedReader::readLongArray;
using AdvancedReader::readLongTuple;
