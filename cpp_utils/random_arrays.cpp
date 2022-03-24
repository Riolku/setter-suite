template <typename T> vector<T> random_array(int N, T lo, T hi) {
  vector<T> ret;
  ret.reserve(N);
  generate_n(back_inserter(ret), N, [lo, hi]() { return randint(lo, hi); });
  return ret;
}

template <typename T> void shuffle(vector<T> &arr) {
  shuffle(all(arr), get_engine());
}

template <typename T>
vector<T> random_sorted_array_with_gaps(int N, T lo, T hi, T gap) {
  if (N == 0)
    return vector<T>();

  vector<T> ret = random_array(N, lo, hi - gap * (N - 1));
  sort(all(ret));
  if (gap != 0) {
    T i = 0;
    transform(ret.begin() + 1, ret.end(), ret.begin() + 1, [&i, gap](T x) {
      i += gap;
      return x + i;
    });
  }
  return ret;
}

template <typename T> vector<T> with_gaps(int N, T lo, T hi, T gap) {
  vector<T> ret = random_sorted_array_with_gaps(N, lo, hi, gap);
  shuffle(ret);
  return ret;
}

template <typename T> vector<T> distinct_array(int N, T lo, T hi) {
  return with_gaps<T>(N, lo, hi, 1);
}

template <typename T> vector<T> array_with_sum(int N, T sum, T lo) {
  vector<T> ret = random_sorted_array_with_gaps(N - 1, 0, sum, lo);
  ret.insert(ret.begin(), 0);
  ret.push_back(sum);
  adjacent_difference(all(ret), ret.begin());
  ret.erase(ret.begin());
  return ret;
}
