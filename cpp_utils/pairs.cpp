namespace UnorderedPair {
// starts counting from zero
// unordered pair x < y

inline ll first_n_sum(ll y) { return (y * (y + 1)) >> 1; }

// Biggest number such that the largest number in pairs is N - 1
// That is, we have N numbers, [0, N - 1]
ll max_pair(ll N) { return first_n_sum(N - 1) - 1; }

// Given a pair, get the integer
ll to_num(ll x, ll y) { return first_n_sum(y) - x - 1; }

// Given an integer, get a pair
pair<ll, ll> to_pair(ll v) {
  ll y = ll(1 + sqrt(1 + (v << 3))) >> 1;

  return {first_n_sum(y) - v - 1, y};
}
}; // namespace UnorderedPair

namespace OrderedPair {
inline ll pick2(ll N) { return N * (N - 1); }

ll max_pair(ll N) { return (pick2(N) >> 1) - 1; }
// starts counting from zero
// ordered pair x != y
pair<ll, ll> to_pair(ll v) {
  ll group_num = ll(1 + sqrt(1 + (v << 2))) >> 1;
  ll group_start = pick2(group_num);

  if (group_num + group_start > v)
    return {v - group_start, group_num};
  return {group_num, v - (group_num + group_start)};
}

ll to_num(ll x, ll y) {
  ll group_num = max(x, y);
  ll group_start = pick2(group_num);

  if (y == group_num)
    return group_start + x;
  return group_start + group_num + y;
}
}; // namespace OrderedPair
