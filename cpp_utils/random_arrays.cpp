template <typename T> vector<T> random_array(int N, T lo, T hi) {
  vector<T> ret;
  ret.reserve(N);
  generate_n(back_inserter(ret), N, [lo, hi]() { return randint(lo, hi); });
  return ret;
}

template <typename T> void shuffle(vector<T> &arr) {
  shuffle(all(arr), get_engine());
}

template <typename T> vector<T> with_gaps(int N, T lo, T hi, T gap) {
  vector<T> ret = random_array(N, lo, hi - gap * (N - 1));
  sort(all(ret));
  int i = 0;
  transform(ret.begin() + 1, ret.end(), ret.begin(), [&i, gap](ll x) {
    i += gap;
    return x + i;
  });
  shuffle(ret);
  return ret;
}
