#include "types.hpp"

// modified from a template by wleung_bvg

namespace Reader {
  enum error_type {INTERNAL_RANGE, EXTERNAL_RANGE, INVALID_ARGUMENT, WRONG_WHITESPACE};

  typedef void __attribute__((noreturn)) (*error_handler)(enum error_type e);

  void __attribute__((noreturn)) default_handler(error_type e) {
    throw runtime_error(to_string(e));
  }

  static error_handler handler = default_handler;

  void set_error_handler(error_handler f) {
    handler = f;
  }

  class FileReader {

  private:
    bool _hasLast = false;
    char _last;

    FILE* stream;

    char _peekChar() {
      _last = _hasLast ? _last : getc(stream);
      _hasLast = true;
      return _last;
    }

    char _getChar() {
      char ret = _peekChar();
      _hasLast = false;
      return ret;
    }

  public:
    FileReader(FILE* f) : stream(f) {}

    char readChar() {
      return _getChar();
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
      if(_getChar() != ' ') handler(WRONG_WHITESPACE);
    }

    void readNewLine() {
      if(_getChar() != '\n') handler(WRONG_WHITESPACE);
    }

    void readEOF() {
      if(_getChar() != char_traits<char>::eof()) handler(WRONG_WHITESPACE);
    }

  private:
    template<typename T>
    T& _fill_arr(T& res, int N, ll lo, ll hi) {
      for(int i = 0; i < N; i++) {
        res[i] = readInt(lo, hi);

        if(i == N - 1) readNewLine();
        else readSpace();
      }

      return res;
    }

  public:
    template<int length>
    array<ll, length> readIntTuple(ll lo, ll hi) {
      array<ll, length> res;

      return _fill_arr(res, length, lo, hi);
    }

    vector<ll> readIntArray(int N, ll lo, ll hi) {
      vector<ll> res(N, 0);

      return _fill_arr(res, N, lo, hi);
    }
  };
}

using Reader::FileReader;
