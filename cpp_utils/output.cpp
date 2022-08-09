namespace Printer {
FILE *stream = stdout;

void set_stream(FILE *st) { stream = st; }

// Simple Types
void print_impl(bool x) { fprintf(stream, "%d", x); }
void print_impl(int x) { fprintf(stream, "%d", x); }
void print_impl(ll x) { fprintf(stream, "%lld", x); }
void print_impl(size_t x) { fprintf(stream, "%lu", x); }
void print_impl(const char *x) { fprintf(stream, "%s", x); }
void print_impl(const string &x) { print_impl(x.c_str()); }
void print_impl(char x) { fprintf(stream, "%c", x); }

// Recursive types. First, declare all overloads
template <typename T> void print_impl(const T &);
template <typename A, typename B> void print_impl(const pair<A, B> &);
template <typename... Ts> void print_impl(const tuple<Ts...> &t);
template <typename T> void print_impl(queue<T> arr);
template <typename T> void print_impl(const set<T> &arr);
template <typename T> void print_impl(const vector<T> &arr);
template <typename T> void print_impl(const unordered_set<T> &arr);

// Recursive type definitions
template <typename T> void print_impl(const T &x) {
  print_impl(x.get_printable());
}

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

template <typename T> void print_iterable(const T &arr) {
  bool first = true;

  for (auto x : arr) {
    if (!first) {
      print_impl(" ");
    }
    first = false;

    print_impl(x);
  }
}

template <typename T> void print_impl(queue<T> q) {
  vector<T> dummy;
  dummy.reserve(q.size());
  while (!q.empty()) {
    dummy.push_back(move(q.front()));
    q.pop_front();
  }
  print_impl(dummy);
}

template <typename T> void print_impl(priority_queue<T> pq) {
  vector<T> dummy;
  dummy.reserve(pq.size());
  while (!pq.empty()) {
    dummy.push_back(move(pq.top()));
    pq.pop();
  }
  print_impl(dummy);
}

template <typename T> void print_impl(const set<T> &arr) {
  print_iterable(arr);
}
template <typename T> void print_impl(const vector<T> &arr) {
  print_iterable(arr);
}
template <typename T> void print_impl(const unordered_set<T> &arr) {
  print_iterable(arr);
}

// print_many helper
template <typename T> void print_many(const T &arg) { print_impl(arg); }
template <typename T, typename... Ts>
void print_many(const T &arg, Ts &&...args) {
  print_impl(arg);
  print_impl(' ');
  print_many(forward<Ts>(args)...);
}

template <typename... Ts> void print(Ts &&...args) {
  print_many(forward<Ts>(args)...);

  print();
}
template <> void print() { fprintf(stream, "\n"); }
template <typename T> void print_map(const T &mp) {
  for (auto p : mp) {
    print(p);
  }
}

template <typename R> void print_items(const R &r) {
  for (auto x : r) {
    print(x);
  }
}

template <typename K, typename V>
void print_items(const unordered_map<K, V> &mp) {
  print_map(mp);
}
template <typename K, typename V> void print_items(const map<K, V> &mp) {
  print_map(mp);
}

template <typename T> void print_yes_no(const T &xs) {
  for (const auto &x : xs)
    print_yes_no(x);
}
template <> void print_yes_no(const bool &x) { print(x ? "YES" : "NO"); }
}; // namespace Printer

using Printer::print;
using Printer::print_items;
using Printer::print_yes_no;
using Printer::set_stream;
