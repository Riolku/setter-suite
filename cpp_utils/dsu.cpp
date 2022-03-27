class DSU {
  int N;
  List<int, 1> parent;
  List<int, 1> rank;
  int component_count;

public:
  explicit DSU(int N)
      : N(N), parent(Range<int>(1, N + 1)), rank(N, 0), component_count(N) {}

  int getparent(int u) {
    if (parent[u] != u)
      parent[u] = getparent(parent[u]);
    return parent[u];
  }

  void merge(int u, int v) {
    int uroot = getparent(u);
    int vroot = getparent(v);

    if (uroot != vroot) {
      if (rank[uroot] < rank[vroot]) {
        parent[uroot] = vroot;
      } else if (rank[vroot] < rank[uroot]) {
        parent[vroot] = uroot;
      } else {
        parent[uroot] = vroot;
        ++rank[vroot];
      }
      --component_count;
    }
  }

  bool same_component(int u, int v) { return getparent(u) == getparent(v); }
  int components() const { return component_count; }
};
