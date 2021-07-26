vector<ll> rand_array(int N, ll a, ll b) {
  vector<ll> ret(N, 0);

  for(int i = 0; i < N; i++) {
    ret[i] = randint(a, b);
  }

  return ret;
}

vector<ld> rand_double_array(int N, ld a, ld b) {
  vector<ld> ret(N, 0);

  for(int i = 0; i < N; i++) {
    ret[i] = randreal(a, b);
  }

  return ret;
}

vector<int> shuffled_range(int l, int r) {
  vector<int> ret;
  for(int i = l; i <= r; i++) ret.push_back(i);

  shuffle(ret);

  return ret;
}

vector<int> permutation(int N) {
  return shuffled_range(1, N);
}

// get an array such that each element is at least `d` apart
vector<ll> array_with_min_dist(int N, ll l, ll r, ll d) {
  vector<ll> ret = rand_array(N, l, r - (N - 1) * d);

  sort(ret.begin(), ret.end());

  for(int i = 0; i < N; i++) {
    ret[i] += d * i;
  }

  shuffle(ret);

  return ret;
}

vector<ll> distinct_array(int N, ll l, ll r) {
  return array_with_min_dist(N, l, r, 1);
}

vector<ll> array_with_sum(int N, ll K) {
  vector<ll> splits = rand_array(N - 1, 0, K);

  splits.push_back(K);

  sort(splits.begin(), splits.end());

  vector<ll> ret = {splits[0]};

  for(int i = 1; i < N; i++) {
    ret.push_back(splits[i] - splits[i - 1]);
  }

  return ret;
}

class ArrayBuilder {
public:
  vector<tuple<ll, ll, ll>> ab;

  ArrayBuilder(vector<tuple<ll, ll, ll>> ab) {
    this->ab = ab;
  }

  vector<ll> build_normal() {
    vector<ll> ans;

    for(auto [len, lo, hi] : ab) {
      for(int i = 0; i < len; i++) ans.push_back(randint(lo, hi));
    }

    return ans;
  }
};