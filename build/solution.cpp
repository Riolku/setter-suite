// Built files `sol_entry` on 2022-03-15
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

namespace Printer {
FILE *stream = stdout;

void set_stream(FILE *st) { stream = st; }

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

template <typename T, size_t... I> void print_tuple(const T &t, index_sequence<I...>) {
    (..., (print_impl(I == 0 ? "" : " "), print_impl(get<I>(t))));
}

template <typename... Ts> void print_impl(const tuple<Ts...> &a) {
    print_tuple(a, std::make_index_sequence<sizeof...(Ts)>());
}

template <typename R> void print_impl(const R &arr) {
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

template <typename T, typename... Ts> void print_many(const T &arg, Ts &&... args) {
    print_impl(arg);
    print_impl(' ');
    print_many(forward<Ts>(args)...);
}

void print() { fprintf(stream, "\n"); }

template <typename... Ts> void print(Ts &&... args) {
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

template <typename T> class List : public vector<T> {
    size_t offset = 0;

  public:
    using vector<T>::vector;
    List(vector<T> v) : vector<T>::vector(move(v)) {}

    List<T> &one_indexed() {
        offset = 1;
        return *this;
    }

    T &operator[](size_t x) { return this->at(x - offset); }
    const T &operator[](size_t x) const { return this->at(x - offset); }

    template <typename F> void for_each(F f) const { ::for_each(all(*this), f); }
    template <typename F> void for_each_pair(F f) const {
        ::for_each(all(*this), [&f](const T &p) {
            auto [a, b] = p;
            f(a, b);
        });
    }
    template <typename F> void for_each_enumerate(F f) const {
        size_t i = offset;
        for (auto it = this->begin(); it != this->end(); ++it) {
            f(*it, i);
            ++i;
        }
    }

    template <typename F> bool any_of(F f) const { return ::any_of(all(*this), f); }
    template <typename F> bool all_of(F f) const { return ::any_of(all(*this), f); }

    List<T> &sort() {
        ::sort(all(*this));
        return *this;
    }

    List<T> &reverse() {
        ::reverse(all(*this));
        return *this;
    }

    template <typename F> List<T> &map(F f) {
        ::transform(all(*this), this->begin(), f);
        return *this;
    }

    template <typename F> auto map_new(F f) const {
        List<decltype(declval<F>()(declval<T>()))> ret;
        ret.reserve(this->size());
        ::transform(all(*this), back_inserter(ret), f);
        return ret;
    }
};

template <typename F> auto generate(int N, F f) {
    List<decltype(declval<F>()())> ret;
    ret.reserve(N);
    ::generate_n(back_inserter(ret), N, move(f));
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

      public:
        using difference_type = void;
        using value_type = T;
        using pointer = void;
        using reference = void;
        using iterator_category = bidirectional_iterator_tag;

        iterator(T cur) : cur(move(cur)) {}

        iterator &operator++() {
            ++cur;
            return *this;
        }
        iterator &operator--() {
            --cur;
            return *this;
        }
        T operator*() { return cur; }

        bool operator<(const iterator &other) const { return cur < other.cur; }
        bool operator!=(const iterator &other) const { return cur != other.cur; }
    };

  public:
    Range(T l, T r) : l(move(l)), r(move(r)) {}
    Range(T r) : l(T()), r(move(r)) {}

    template <typename F> bool any_of(F f) const { return ::any_of(all(*this), f); }
    template <typename F> bool all_of(F f) const { return ::any_of(all(*this), f); }

    template <typename F> void for_each(F f) const { ::for_each(all(*this), f); }

    template <typename F> auto map_new(F f) const {
        List<decltype(declval<F>()(declval<T>()))> ret;
        ret.reserve(this->size());
        ::transform(all(*this), back_inserter(ret), f);
        return ret;
    }

    auto size() const { return r - l; }
    T begin() const { return iterator(l); }
    T end() const { return iterator(r); }
};

ValidatingReader in_r(stdin);
