#include <iostream>
#include <functional>
#include <limits>
using namespace std;

// modified from a template by wleung_bvg

using ll=long long;using ld=long double;

namespace Validator {
  bool _hasLast = false;
  char _last;

  void default_handler() {
    throw runtime_error("Invalid input!");
  }

  [[noreturn]] void (*func)() = default_handler;

  void set_validator_handler(void (*f)()) {
    func = f;
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

      if(minValid > ret || maxValid < ret) func();

      return ret;
    }
    catch(out_of_range e) {
      func();
    }
    catch(invalid_argument e) {
      func();
    }
  }

  ld readFloat(ld minValid, ld maxValid) {
    string token = "";
    while (isdigit(_peekChar()) || _peekChar() == '-' || _peekChar() == '.') token.push_back(_getChar());

    try {
      ld ret = stold(token);

      if(minValid > ret || maxValid < ret) func();

      return ret;
    }
    catch(out_of_range e) {
      func();
    }
    catch(invalid_argument e) {
      func();
    }
  }

  void readSpace() {
    if(_getChar() != ' ') func();
  }

  void readNewLine() {
    if(_getChar() != '\n') func();
  }

  void readEOF() {
    if(_getChar() != char_traits<char>::eof()) func();
  }
}

using namespace Validator;
