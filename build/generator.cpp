// Built with `init-template gen_entry` on 2022-03-15
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

template <size_t index, typename T> typename enable_if<index == 0, void>::type print_tuple(const T &t) {}

template <size_t index, typename T> typename enable_if<index == 1, void>::type print_tuple(const T &t) {
    print_impl(get<tuple_size<T>() - index>(t));
}

template <size_t index, typename T> typename enable_if<(index > 1), void>::type print_tuple(const T &t) {
    print_impl(get<tuple_size<T>() - index>(t));
    print_impl(' ');
    print_tuple<index - 1>(t);
}

template <typename... Ts> void print_impl(const tuple<Ts...> &t) { print_tuple<sizeof...(Ts)>(t); }

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

template <typename T, typename... Ts> void print_many(const T &arg, Ts &&...args) {
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
        ::for_each(all(*this), [&f](const T &p) -> void {
            auto a = get<0>(p), b = get<1>(p);
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
        List<result_of<F(T)>> ret;
        ret.reserve(this->size());
        ::transform(all(*this), back_inserter(ret), f);
        return ret;
    }
};

template <typename F> auto generate(int N, F f) {
    List<result_of<F()>> ret;
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

vector<ll> random_array(int N, ll lo, ll hi) {
    vector<ll> ret;
    ret.reserve(N);
    generate_n(back_inserter(ret), N, [=]() { return randint(lo, hi); });
    return ret;
}

void shuffle(vector<ll> &arr) { shuffle(all(arr), get_engine()); }

vector<ll> with_gaps(int N, ll lo, ll hi, ll gap) {
    vector<ll> ret = random_array(N, lo, hi - gap * N);
    sort(all(ret));
    int i = -1;
    transform(all(ret), ret.begin(), [&i](int x) {
        ++i;
        return x + i;
    });
    return ret;
}

int main(int argc, char **argv) {
    assert(argc >= 3);

    int suite = atoi(argv[1]);
    int case_num = atoi(argv[2]);

    init_engine(suite * 1e4 + case_num);
}

