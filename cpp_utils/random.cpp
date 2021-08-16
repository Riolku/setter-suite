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
