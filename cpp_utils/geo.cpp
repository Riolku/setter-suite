#include "arrays.cpp"

vector<pair<ll, ll>> convex_polygon(int N, ll xl, ll xh, ll yl, ll yh) {
  int pxc = max(min(xh - xl, randint(1, N - 1)), N - xh + xl);

  vector<ll> xs = array_with_sum(pxc, xh - xl);

  for(ll x : array_with_sum(N - pxc, xh - xl)) {
    xs.push_back(-x);
  }

  int pyc = max(min(yh - yl, randint(1, N - 1)), N - yh + yl);

  vector<ll> ys = array_with_sum(pyc, yh - yl);

  for(ll y : array_with_sum(N - pyc, yh - yl)) {
    ys.push_back(-y);
  }

  shuffle(xs);
  shuffle(ys);

  vector<pair<ll, ll>> vectors;

  for(int i = 0; i < N; i++) vectors.emplace_back(xs[i], ys[i]);

  sort(vectors.begin(), vectors.end(), [](pair<ll,ll> x, pair<ll,ll> y) {
    return atan2(x.first, x.second) < atan2(y.first, y.second);
  });

  ll mx_x = 0, mx_y = 0;

  ll cur_x = 0, cur_y = 0;

  vector<pair<ll, ll>> points;

  for(int i = 0; i < N; i++) {
    cur_x += vectors[i].first;
    cur_y += vectors[i].second;

    mx_x = max(mx_x, cur_x);
    mx_y = max(mx_y, cur_y);

    points.emplace_back(cur_x, cur_y);
  }

  for(auto& p : points) {
    p.first += xh - mx_x;
    p.second += yh - mx_y;
  }

  return points;
}
