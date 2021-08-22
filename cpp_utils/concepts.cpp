template<typename T>
concept Iterable = requires(T x) {
  x.begin();
  x.end();
};

template<typename F, typename T>
concept Predicate = requires(F f, T x) {
  { f(x) } -> convertible_to<bool>;
};
