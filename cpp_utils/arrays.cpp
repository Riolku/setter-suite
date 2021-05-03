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

// get an array such that each element is at least `d` apart
vector<ll> array_with_min_dist(int N, ll l, ll r, ll d) {
  vector<ll> ret = rand_array(N, l, r - (N - 1) * d);

  for(int i = 0; i < N; i++) {
    ret[i] += d * i;
  }

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
