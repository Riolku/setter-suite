#include "types.hpp"

// modified from a template by wleung_bvg

namespace Reader {
  const int ERROR_COUNT = 4;

  enum error_type {INTERNAL_RANGE = 0, EXTERNAL_RANGE, INVALID_ARGUMENT, WRONG_WHITESPACE};

  const char* error_names[ERROR_COUNT] = {"INTERNAL_RANGE", "EXTERNAL_RANGE", "INVALID_ARGUMENT", "WRONG_WHITESPACE"};

  typedef void __attribute__((noreturn)) (*error_handler)(enum error_type e);

  void __attribute__((noreturn)) default_handler(error_type e) {
    if(e >= ERROR_COUNT) throw runtime_error("Unknown error in FileReader");

    throw runtime_error(error_names[e]);
  }

  static error_handler handler = default_handler;

  void set_error_handler(error_handler f) {
    handler = f;
  }

  class FileReader {

  private:
    FILE* stream;

    bool hasLast;
    char lastChar;

  public:
    FileReader(FILE* f) : stream(f), hasLast(false), lastChar(0) {}

    char peekChar() {
      if(!hasLast) {
        lastChar = getc(stream);
        hasLast = true;
      }
      return lastChar;
    }

    char readChar() {
      char ret = peekChar();
      hasLast = false;
      return ret;
    }


    bool eof() {
      return peekChar() == char_traits<char>::eof();
    }

    void trim() {
      while(isspace(peekChar()) && !eof()) {
        readChar();
      }
    }

    ll readInt(ll minValid, ll maxValid) {
      string token = "";
      while (isdigit(peekChar()) || peekChar() == '-') token.push_back(readChar());
      try {
        ll ret = stoll(token);

        if(minValid > ret || maxValid < ret) handler(INTERNAL_RANGE);

        return ret;
      }
      catch(const out_of_range& e) {
        handler(EXTERNAL_RANGE);
      }
      catch(const invalid_argument& e) {
        handler(INVALID_ARGUMENT);
      }
    }

    ld readFloat(ld minValid, ld maxValid) {
      string token = "";
      while (isdigit(peekChar()) || peekChar() == '-' || peekChar() == '.') token.push_back(readChar());

      try {
        ld ret = stold(token);

        if(minValid > ret || maxValid < ret) handler(INTERNAL_RANGE);

        return ret;
      }
      catch(const out_of_range& e) {
        handler(EXTERNAL_RANGE);
      }
      catch(const invalid_argument& e) {
        handler(INVALID_ARGUMENT);
      }
    }

    string readFile() {
      string ret = "";
      while(!eof()) {
        ret.push_back(readChar());
      }

      readEOF();

      return ret;
    }

    string readString(int N) {
      string ret = "";
      for(int i = 0; i < N; i++) {
        ret.push_back(readChar());
      }

      return ret;
    }

    string readLine() {
      string ret = "";
      while (peekChar() != '\n' && !eof()) ret.push_back(readChar());
      readNewLine();

      return ret;
    }

    void readSpace() {
      if(readChar() != ' ') handler(WRONG_WHITESPACE);
    }

    void readNewLine() {
      if(readChar() != '\n') handler(WRONG_WHITESPACE);
    }

    void readEOF() {
      if(readChar() != char_traits<char>::eof()) handler(WRONG_WHITESPACE);
    }

    string readToken() {
      string token = "";

      while(!isspace(peekChar()) && !eof()) token.push_back(readChar());

      return token;
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
