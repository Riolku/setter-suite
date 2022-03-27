namespace DirectedGraph {
List<Edge> random_edges(int N, int M) {
  OrderedPairBijection mapper(N);
  return distinct_array<ll>(M, 0, 1LL * N * (N - 1) - 1)
      .map_new([&mapper](ll v) -> Edge {
        auto e = mapper.to_pair(v);
        return {e.first + 1, e.second + 1};
      });
}

Graph random_graph(int N, int M) { return Graph(N, random_edges(N, M)); }

List<Edge> random_dag_edges(int N, int M) {
  return distinct_array<ll>(M, 0, 1LL * N * (N - 1) / 2 - 1)
      .map_new([](ll v) -> Edge {
        auto e = UnorderedPair::to_pair(v);
        return {e.first + 1, e.second + 1};
      });
}

Graph random_maybe_disconnected_dag(int N, int M) {
  return Graph(N, random_dag_edges(N, M));
}

Graph random_connected_dag(int N, int M) {
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
  return Graph(N, move(edges));
}

Graph random_scc(int N, int M) {
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

  return Graph(N, edges);
}

}; // namespace DirectedGraph
