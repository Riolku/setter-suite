class SetWithBIT {
  int N, high_pow;
  size_t sz;
  List<int> bit;

  inline int LSB(int x) const { return x & -x; }

public:
  SetWithBIT(int N) : N(N), high_pow(1), sz(0), bit(N + 1, 0) {
    while (high_pow <= N)
      high_pow <<= 1;
    high_pow >>= 1;
  }

  int indexof(int x) const {
    int tot = 0;
    for (; x > 0; x -= LSB(x))
      tot += bit[x];
    return tot;
  }

  bool contains(int x) const { return indexof(x - 1) + 1 == indexof(x); }

  void add(int x) {
    assert(1 <= x && x <= N);

    for (; x <= N; x += LSB(x)) {
      ++bit[x];
    }

    ++sz;
  }

  void remove(int x) {
    assert(1 <= x && x <= N);

    for (; x <= N; x += LSB(x)) {
      --bit[x];
    }

    --sz;
  }

  void clear() {
    for (int i = 0; i <= N; ++i)
      bit[i] = 0;
    sz = 0;
  }

  size_t size() const { return sz; }

  int operator[](int i) const {
    int cur = 0;
    int p = high_pow;

    while (p) {
      if (cur + p <= N && bit[cur + p] < i) {
        cur += p;
        i -= bit[cur];
      }

      p >>= 1;
    }

    return cur + 1;
  }

  ~SetWithBIT() {}
};
