#pragma once

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

  void print_edges(FILE* stream = stdout) {
    for(int i = 0; i < N - 1; i++) {
      fprintf(stream, "%d %d\n", edges[i].first, edges[i].second);
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

tree perfect_star_tree(int N) {
  tree ret(N);

  for(int i = 1; i < N; i++) {
    ret.edges[i - 1] = {1, i + 1};
  }

  ret.permute();

  return ret;
}

tree perfect_line_tree(int N) {
  tree ret(N);

  for(int i = 1; i < N; i++) {
    ret.edges[i - 1] = {i, i + 1};
  }

  ret.permute();

  return ret;
}

tree rough_line_tree(int N) {
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

tree rough_star_tree(int N) {
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

enum { PERFECT_LINE_TREE, PERFECT_STAR_TREE, ROUGH_LINE_TREE, ROUGH_STAR_TREE, TRUE_RANDOM_TREE } tree_type;

tree get_tree(int N, int tp) {
  switch(tp) {
    case PERFECT_LINE_TREE: return perfect_line_tree(N);

    case PERFECT_STAR_TREE: return perfect_star_tree(N);

    case ROUGH_LINE_TREE: return rough_line_tree(N);

    case ROUGH_STAR_TREE: return rough_star_tree(N);

    case TRUE_RANDOM_TREE: return true_random_tree(N);

    default:
      throw runtime_error("Invalid tree type passed to get_tree");
  }
}

tree random_tree(int N) {
  return get_tree(N, choice(vector<int>({ PERFECT_LINE_TREE, PERFECT_STAR_TREE, ROUGH_LINE_TREE, ROUGH_STAR_TREE, TRUE_RANDOM_TREE })));
}
