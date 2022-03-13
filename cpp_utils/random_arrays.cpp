vector<ll> random_array(int N, ll lo, ll hi) {
    vector<ll> ret;
    ret.reserve(N);
    generate_n(back_inserter(ret), N, [=]() { return randint(lo, hi); });
    return ret;
}

void shuffle(vector<ll> &arr) { shuffle(arr.begin(), arr.end(), get_engine()); }

vector<ll> with_gaps(int N, ll lo, ll hi, ll gap) {
    vector<ll> ret = random_array(N, lo, hi - gap * N);
    sort(ret.begin(), ret.end());
    for (int i = 0; i < N; ++i)
        ret[i] += i;
    shuffle(ret);
    return ret;
}
