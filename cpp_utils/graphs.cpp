using Edge = pair<int, int>;

struct Graph {
  int N, M;
  List<Edge> edges;
  explicit Graph(int N) : N(N), M(0), edges() {}
  Graph(int N, List<Edge> edges) : N(N), edges(move(edges)) {}

  void add_edge(int u, int v) {
    edges.emplace_back(u, v);
    ++M;
  }

  void add_graph(const Graph &other, const List<Edge> &connectors) {
    edges.reserve(M + other.M + connectors.size());
    other.edges.for_each_pair([this](int u, int v) -> void { add_edge(u, v); });
    connectors.for_each_pair(
        [this](int u, int v) -> void { add_edge(u, v + N); });
    N += other.N;
  }

  bool is_connected() const {
    DSUWithRank dsu(N);
    edges.for_each_pair(
        [&dsu](int u, int v) -> void { dsu.merge_together(u, v); });
    return dsu.components() == 1;
  }
  bool is_tree() const { return M == N - 1 && is_connected(); }
};

namespace Printer {

void print(const Graph &g) {
  print(g.N, g.M);
  print_items(g.edges);
}

}; // namespace Printer
