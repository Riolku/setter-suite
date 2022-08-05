template <typename Parent> struct RandomGraph : public Parent {
  using Parent::Parent;

  void permute_edges() {
    auto perm = permutation(this->N);
    this->edges.map([&perm](pii e) -> pii {
      return {perm[e.first], perm[e.second]};
    });
  }
};

struct RandomDirectedGraph : public RandomGraph<DirectedGraph> {
  using RandomGraph<DirectedGraph>::RandomGraph;

  void shuffle() {
    permute_edges();
    ::shuffle(edges);
  }

  static List<Edge> random_edges(int N, int M) {
    return distinct_array<ll>(M, 0, OrderedPair::max_pair(N))
        .map_new([](ll v) -> Edge {
          auto e = OrderedPair::to_pair(v);
          return {e.first + 1, e.second + 1};
        });
  }

  static RandomDirectedGraph random_graph(int N, int M) {
    return RandomDirectedGraph(N, random_edges(N, M));
  }

  static List<Edge> random_dag_edges(int N, int M) {
    return distinct_array<ll>(M, 0, UnorderedPair::max_pair(N))
        .map_new([](ll v) -> Edge {
          auto e = UnorderedPair::to_pair(v);
          return {e.first + 1, e.second + 1};
        });
  }

  static RandomDirectedGraph random_maybe_disconnected_dag(int N, int M) {
    return RandomDirectedGraph(N, random_dag_edges(N, M));
  }

  static RandomDirectedGraph random_connected_dag(int N, int M) {
    List<Edge> edges;
    edges.reserve(M);
    for (int i = 1; i < N; ++i) {
      edges.emplace_back(i, i + 1);
    }
    random_dag_edges(N - 1, M - (N - 1)).for_each_pair([&edges](int u, int v) {
      // u, v are in [1, N - 1], u < v.
      // make sure we don't add [u, u + 1].
      edges.emplace_back(u, v + 1);
    });
    return RandomDirectedGraph(N, move(edges));
  }

  static RandomDirectedGraph random_scc(int N, int M) {
    assert(N >= 2);
    assert(M >= N);
    List<Edge> edges;
    edges.reserve(M);
    for (int i = 1; i < N; ++i) {
      edges.emplace_back(i, i + 1);
    }
    edges.emplace_back(N, 1);

    distinct_array<ll>(M - N, 0, 1LL * N * (N - 2) - 1)
        .for_each([&edges, N](ll x) {
          int u = x % N + 1; // [1, N]
          int v = x / N + 1; // [1, N - 2]
          if (u == N)
            ++v; // can't have v = 1 if u = N
          else if (v >= u)
            v += 2; // otherwise, can't let v = u or v = u + 1

          edges.emplace_back(u, v);
        });

    return RandomDirectedGraph(N, move(edges));
  }
};

struct RandomUndirectedGraph : public RandomGraph<UndirectedGraph> {
  using RandomGraph<UndirectedGraph>::RandomGraph;

  void shuffle() {
    permute_edges();
    edges.map([](pii e) -> pii {
      if (randint(0, 1))
        return e;
      return {e.second, e.first};
    });
    ::shuffle(edges);
  }

  static List<Edge> random_edges(int N, int M) {
    return RandomDirectedGraph::random_dag_edges(N, M);
  }

  static RandomUndirectedGraph star(int N) {
    List<Edge> edges;
    edges.reserve(N - 1);
    for (int i = 2; i <= N; ++i) {
      edges.emplace_back(1, i);
    }
    return RandomUndirectedGraph(N, move(edges));
  }

  static List<Edge> random_tree_edges(int N) {
    List<Edge> edges;
    edges.reserve(N - 1);
    for (int i = 2; i <= N; ++i) {
      edges.emplace_back(randint(1, i - 1), i);
    }
    return edges;
  }

  static RandomUndirectedGraph random_tree(int N) {
    return RandomUndirectedGraph(N, random_tree_edges(N));
  }
};
