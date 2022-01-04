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

template <range R> void print_impl(const R &arr) {
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

template <range R> void print_items(const R &r) {
    for (auto x : r) {
        print(x);
    }
}
}; // namespace Printer

using Printer::print;
using Printer::print_items;
using Printer::set_stream;
