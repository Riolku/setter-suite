struct Graph {
  int N, M;
  vector<vector<int>> adj;
  Graph(int N) : N(N), M(0), adj(N + 1, vector<int>()) {}

  void add_edge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
    ++M;
  }

  vector<bool> bfs(int s) const {
    queue<int> q;
    q.push(s);

    vector<bool> vis(N + 1, false);
    vis[s] = true;

    while (!q.empty()) {
      int v = q.front();
      q.pop();
      for (auto u : adj[v])
        if (!vis[u]) {
          vis[u] = true;
          q.push(u);
        }
    }
    return vis;
  }

  bool is_tree() const {
    if (M != N - 1)
      return false;
    auto vis = bfs(1);
    return all_of(vis.begin() + 1, vis.end(), [](bool x) { return x; });
  }
};
