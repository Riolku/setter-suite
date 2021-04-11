#include "types.hpp"

void init_engine(ull seed);

default_rng& get_engine();

ll randint(ll a, ll b);

ld randreal(ld a, ld b);

template<typename T>
int weighted_integer(vector<T> weights);

template<typename T1, typename T2>
T1 weighted_choice(vector<pair<T1, T2>> weighted_probabilities);

template<typename T>
T choice(vector<T> elements);

template<typename IT>
void shuffle(IT start, IT end)
