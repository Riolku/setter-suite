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
