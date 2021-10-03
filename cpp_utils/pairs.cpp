
namespace UnorderedPair {
// starts counting from zero
// unordered pair x < y

// Given a pair, get the integer
ll to_num(ll x, ll y) { return y * (y - 1) / 2 + y - x - 1; }

// Given an integer, get a pair
pair<ll, ll> to_pair(ll v) {
    ll y = ll((1 + sqrt(1 + 8 * v)) / 2);

    return {y * (y - 1) / 2 + y - v - 1, y};
}
}; // namespace UnorderedPair

namespace OrderedPair {
// x != y, zero-indexed
ll to_num(ll x, ll y) {
    if (x > y) {
        return x * (x - 1) + y;
    } else {
        return y * y + x;
    }
}

pair<ll, ll> to_pair(ll v) {
    ll x = ll(1 + sqrt(1 + 4 * v) / 2);
    if (x * x <= v) {
        return {v - x * x, x};
    } else {
        return {x, v - x * (x - 1)};
    }
}
}; // namespace OrderedPair
