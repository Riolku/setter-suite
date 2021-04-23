#pragma once

#include "types.hpp"

#include "random.cpp"

vector<ll> rand_array(int N, ll a, ll b) {
  vector<ll> ret(0, N);

  for(int i = 0; i < N; i++) {
    ret[i] = randint(a, b);
  }

  return ret;
}

vector<ld> rand_array(int N, ld a, ld b) {
  vector<ld> ret(0, N);

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

vector<ll> distinct_array(int N, ll l, ll r) {
  vector<ll> ret;

  ll K = r - l + 1;

  if(2 * N > K) {
    vector<ll> perm = shuffled_range(0, K - 1);

    for(int i = 0; i < N; i++) {
      ret.push_back(l + perm[i]);
    }

    return ret;
  }
  else {
    unordered_set<ll> seen;

    for(int i = 0; i < N; i++) {
      int x;
      do {
        x = randint(l, r);
      } while(seen.count(x));

      ret.push_back(x);
      seen.insert(x);
    }
  }

  return ret;
}

vector<ll> array_with_sum(int N, ll K) {
  vector<ll> splits = distinct_array(N - 1, 1, K - 1);
  splits.push_back(K);

  sort(splits.begin(), splits.end());

  vector<ll> ret = {splits[0]};

  for(int i = 1; i < N; i++) {
    ret.push_back(splits[i] - splits[i - 1]);
  }

  return ret;
}
