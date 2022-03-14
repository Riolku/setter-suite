vector<ll> random_array(int N, ll lo, ll hi) {
    vector<ll> ret;
    ret.reserve(N);
    generate_n(back_inserter(ret), N, [=]() { return randint(lo, hi); });
    return ret;
}

void shuffle(vector<ll> &arr) { shuffle(all(arr), get_engine()); }

vector<ll> with_gaps(int N, ll lo, ll hi, ll gap) {
    vector<ll> ret = random_array(N, lo, hi - gap * N);
    sort(all(ret));
    int i = -1;
    transform(all(ret), ret.begin(), [&i](int x) {
        ++i;
        return x + i;
    });
    return ret;
}
