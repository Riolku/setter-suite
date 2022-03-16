// Built files `identical_checker_entry` on 2022-03-15
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
#include <memory>
#include <queue>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;

#define all(x) (x).begin(), (x).end()

template <typename P> auto to_pair(P p) {
    auto [a, b] = p;
    return make_pair(move(a), move(b));
}

// modified from a template by wleung_bvg

class BaseReader {
    const static size_t MAX_TOKEN_SIZE = 1e6;

  private:
    FILE *stream;
    bool streamOpen;

    bool hasLast;
    char lastChar;

  public:
    BaseReader(FILE *f) : stream(f), streamOpen(true), hasLast(false), lastChar(0) {}

    BaseReader(char *path) : BaseReader(fopen(path, "r")) {}

    char peekChar() {
        if (!hasLast) {
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

    bool eof() { return peekChar() == char_traits<char>::eof(); }

    ll readInt(ll lo = numeric_limits<ll>::min(), ll hi = numeric_limits<ll>::max()) {
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
        string token;
        while (!isspace(peekChar()) && !eof() && token.size() <= MAX_TOKEN_SIZE)
            token.push_back(readChar());
        return token;
    }

    string readString(size_t N) {
        string token;
        while (!eof() && token.size() < N) {
            token.push_back(readChar());
        }
        return token;
    }

  private:
    template <typename Arr> void _fill_arr(Arr &a, size_t N, ll lo, ll hi) {
        for (size_t i = 0; i < N; i++) {
            if (i)
                readSpace();
            a[i] = readInt(lo, hi);
        }
        readNewLine();
    }

  public:
    template <size_t length, typename T = ll>
    array<T, length> readIntTuple(ll lo = numeric_limits<ll>::min(), ll hi = numeric_limits<ll>::max()) {
        array<T, length> res;
        _fill_arr(res, length, lo, hi);
        return res;
    }

    template <typename T = ll>
    pair<T, T> readIntPair(ll lo = numeric_limits<ll>::min(), ll hi = numeric_limits<ll>::max()) {
        T a = readInt(lo, hi);
        readSpace();
        T b = readInt(lo, hi);
        readNewLine();
        return {a, b};
    }

    template <typename T = ll>
    vector<T> readIntArray(size_t N, ll lo = numeric_limits<ll>::min(), ll hi = numeric_limits<ll>::max()) {
        vector<T> v;
        v.resize(N);
        _fill_arr(v, N, lo, hi);
        return v;
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

    void closeStream() {
        if (streamOpen) {
            fclose(stream);
            streamOpen = false;
        }
    }

    virtual ~BaseReader() { closeStream(); }

  protected:
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
        if (Parent::readChar() != '\n')
            Parent::wrongWhitespaceError();
    }

    void readEOF() override {
        if (!Parent::eof())
            Parent::wrongWhitespaceError();
    }

    void readSpace() override {
        if (Parent::readChar() != ' ')
            Parent::wrongWhitespaceError();
    }

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
    void wrongWhitespaceError() override { throw runtime_error("WRONG_WHITESPACE"); }
    void invalidIntegerError() override { throw runtime_error("INVALID_INTEGER"); }

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

void assertOrCode(bool cond, int code) {
    if (!cond)
        exit(code);
}
void assertWA(bool cond) { assertOrCode(cond, CheckerCodes::WA); }
void assertPE(bool cond) { assertOrCode(cond, CheckerCodes::WA); }

class CheckerReader : public BaseReader {
  protected:
    virtual void preError() {}
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
        printf("Check your Whitespace");
        exit(CheckerCodes::PE);
    }
    void invalidIntegerError() override {
        preError();
        printf("Check your Integers");
        exit(CheckerCodes::PE);
    }

  public:
    using BaseReader::BaseReader;
};

using IdenticalCheckerReader = ExactWhitespaceMixin<CheckerReader>;

int main(int argc, char **argv) {
    IdenticalCheckerReader prog_r(argv[2]);
    ValidatingReader in_r(argv[1]), sol_r(argv[3]);
}
