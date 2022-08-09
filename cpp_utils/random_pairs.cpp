// Return a random pair (x, y) such that x != y and 0 + offset <= x < M + offset
// and 0 + offset <= y < M + offset
template <typename IntType, int offset>
pair<IntType, IntType> random_ordered_pair(int M) {
  IntType x = randint(0, M - 1);
  IntType y = randint(0, M - 2);
  if (y >= x)
    ++y;
  return {x + offset, y + offset};
}

// Generate a list of random ordered pairs
// These pairs are NOT guaranteed to be distinct!
template <typename IntType, int offset>
List<pair<IntType, IntType>> random_ordered_pairs(int N, int M) {
  return generate(N, [M]() -> pair<IntType, IntType> {
    return random_ordered_pair<IntType, offset>(M);
  });
}

// Return a random pair (x, y) such that 0 + offset <= x < y < M + offset
// and 0 + offset <= y < M + offset
template <typename IntType, int offset>
pair<IntType, IntType> random_unordered_pair(int M) {
  ll v = randint(0, UnorderedPair::max_pair(M));
  IntType a, b;
  tie(a, b) = UnorderedPair::to_pair(v);
  return {a + offset, b + offset};
}

// Generate a list of random unordered pairs
// These pairs are NOT guaranteed to be distinct!
template <typename IntType, int offset>
List<pair<IntType, IntType>> random_unordered_pairs(int N, int M) {
  return generate(N, [M]() -> pair<IntType, IntType> {
    return random_unordered_pair<IntType, offset>(M);
  });
}
