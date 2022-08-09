using Edge = pair<int, int>;

struct Graph {
  int N, M;
  List<Edge> edges;
  explicit Graph(int N) : N(N), M(0), edges() {}
  Graph(int N, List<Edge> edges) : N(N), M(edges.size()), edges(move(edges)) {}

  template <typename GraphType>
  static GraphType read_graph(int N, int M, BaseReader &reader) {
    List<Edge> edges = generate(
        M, [N, &reader]() -> Edge { return reader.readIntPair<int>(1, N); });

    return GraphType(N, move(edges));
  }

  virtual void normalize_edges() = 0;
  bool has_duplicate_edges() {
    normalize_edges();
    edges.sort();
    return adjacent_find(all(edges)) != edges.end();
  }

  bool has_self_loops() const {
    return edges.any_of([](const Edge &e) { return e.first == e.second; });
  }

  void add_edge(int u, int v) {
    edges.emplace_back(u, v);
    ++M;
  }

  virtual List<List<int>, 1> make_adj() const = 0;

  List<int, 1> bfs(int src) const {
    List<int, 1> ret(N, -1);
    List<List<int>, 1> adj = make_adj();
    ret[src] = 0;
    queue<int> q;
    q.push(src);
    exhaust_queue(q, [&adj, &q, &ret](int v) {
      for (int u : adj[v]) {
        if (ret[u] == -1) {
          ret[u] = ret[v] + 1;
          q.push(u);
        }
      }
    });
    return ret;
  }

  void add_graph(const Graph &other, const List<Edge> &connectors) {
    other.edges.for_each_pair(
        [this](int u, int v) -> void { add_edge(u + N, v + N); });
    connectors.for_each_pair(
        [this](int u, int v) -> void { add_edge(u, v + N); });
    N += other.N;
  }

  bool is_connected_from_dists(const List<int, 1> &dists) const {
    return dists.all_of([](int x) -> bool { return x != -1; });
  }

  bool is_connected() const {
    List<int, 1> dists = bfs(1);
    return is_connected_from_dists(dists);
  }
  bool is_tree() const { return M == N - 1 && is_connected(); }
};

struct DirectedGraph : public Graph {
  using Graph::Graph;

  void normalize_edges() override {}
  virtual List<List<int>, 1> make_adj() const {
    List<List<int>, 1> adj(N);
    edges.for_each_pair([&adj](int u, int v) { adj[u].push_back(v); });
    return adj;
  }
};

struct UndirectedGraph : public Graph {
  using Graph::Graph;

  void normalize_edges() override {
    edges.for_each_pair([](int &u, int &v) {
      if (u > v) {
        swap(u, v);
      }
    });
  }

  virtual List<List<int>, 1> make_adj() const {
    List<List<int>, 1> adj(N);
    edges.for_each_pair([&adj](int u, int v) {
      adj[u].push_back(v);
      adj[v].push_back(u);
    });
    return adj;
  }
};
