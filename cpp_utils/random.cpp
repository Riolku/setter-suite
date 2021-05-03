#pragma once

#include "types.hpp"

using default_rng = mt19937_64;

static default_rng * _random_engine = 0;

void init_engine(ull seed) {
  _random_engine = new default_rng(seed);
}

default_rng& get_engine() {
  if(!_random_engine) {
    throw runtime_error("No engine configured!");
  }

  return *_random_engine;
}

ll randint(ll a, ll b) {
  assert(a <= b);
  return uniform_int_distribution<ll>(a, b)(get_engine());
}

// [a, b)
ld randreal(ld a, ld b) {
  assert(a <= b);
  return uniform_real_distribution<ld>(a, b)(get_engine());
}

int weighted_integer(initializer_list<double> weights) {
  return discrete_distribution<int>(weights)(get_engine());
}

template<typename T1, typename T2>
T1 weighted_choice(vector<pair<T1, T2>> weighted_probabilities) {
  int N = weighted_probabilities.size();

  vector<T2> weights(N);

  for(int i = 0; i < N; i++) {
    weights[i] = weighted_probabilities[i].second;
  }

  return weighted_probabilities[weighted_integer(weights)].first;
}

template<typename T>
T choice(vector<T> elements) {
  return elements[randint(0, elements.size() - 1)];
}

template<typename IT>
void shuffle(IT start, IT end) {
  shuffle(start, end, get_engine());
}

template<typename T>
void shuffle(vector<T>& v) {
  shuffle(v.begin(), v.end());
}
