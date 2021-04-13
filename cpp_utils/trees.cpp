#include <cmath>

#include "types.hpp"
#include "random.cpp"
#include "arrays.cpp"

struct tree {
  int N;
  vector<pair<int, int>> edges;

  tree(int N) : N(N), edges(N - 1, {0, 0}) {}

  void permute() {
    vector<int> perm = permutation(N);

    for(int i = 0; i < N - 1; i++) {
      edges[i] = {perm[edges[i].first - 1], perm[edges[i].second - 1]};
    }
  }
};

tree true_random_tree(int N) {
  tree ret(N);

  for(int i = 1; i < N; i++) {
    ret.edges[i - 1] = {randint(1, i), i + 1};
  }

  ret.permute();

  return ret;
}

tree perfect_star_graph(int N) {
  tree ret(N);

  for(int i = 1; i < N; i++) {
    ret.edges[i - 1] = {1, i + 1};
  }

  ret.permute();

  return ret;
}

tree perfect_line_graph(int N) {
  tree ret(N);

  for(int i = 1; i < N; i++) {
    ret.edges[i - 1] = {i, i + 1};
  }

  ret.permute();

  return ret;
}

tree rough_line_graph(int N) {
  tree ret(N);

  for(int i = 1; i < N; i++) {
    double cbrt = pow(N, 1.0 / 3);

    if(weighted_integer({1, 2 * cbrt - 1}) == 1) {
      ret.edges[i - 1] = {i, i + 1};
    }
    else {
      ret.edges[i - 1] = {randint(1, i), i + 1};
    }
  }

  ret.permute();

  return ret;
}

tree rough_star_graph(int N) {
  tree ret(N);

  for(int i = 1; i < N; i++) {
    double cbrt = pow(N, 1.0 / 3);

    if(weighted_integer({1, 2 * cbrt - 1}) == 1) {
      ret.edges[i - 1] = {1, i + 1};
    }
    else {
      ret.edges[i - 1] = {randint(1, i), i + 1};
    }
  }

  ret.permute();

  return ret;
}

tree random_tree(int N) {
  switch(weighted_integer({1, 1, 1, 1, 1})) {
    case 0:
      return perfect_line_graph(N);

    case 1:
      return perfect_star_graph(N);

    case 2:
      return rough_line_graph(N);

    case 3:
      return rough_star_graph(N);

    case 4:
      return true_random_tree(N);

    default:
      throw runtime_error("weighted_integer returned an invalid value.");
  }
}

int main() {
  init_engine(0);
}
