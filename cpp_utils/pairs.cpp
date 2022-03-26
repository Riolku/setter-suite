namespace UnorderedPair {
// starts counting from zero
// unordered pair x < y

inline ll first_n_sum(ll y) { return (y * (y + 1)) >> 1; }

// Given a pair, get the integer
ll to_num(ll x, ll y) { return first_n_sum(y) - x - 1; }

// Given an integer, get a pair
pair<ll, ll> to_pair(ll v) {
  ll y = (1 + sqrt(1 + (v << 3))) / 2;

  return {first_n_sum(y) - v - 1, y};
}
}; // namespace UnorderedPair

struct OrderedPairBijection {
  // starts counting from zero
  // ordered pair x != y
  int N;
  explicit OrderedPairBijection(int N) : N(N) {}

  pair<int, int> to_pair(ll v) {
    int x = v % N; // [0, N)
    int y = v / N; // [0, N - 1)
    if (y >= x)
      ++y;

    return {x, y};
  }

  ll to_num(int x, int y) {
    if (y > x)
      --y;
    return y * N + x;
  }
};
