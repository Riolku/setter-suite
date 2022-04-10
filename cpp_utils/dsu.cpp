class DSU {
protected:
  int N;
  List<int, 1> parent;
  int component_count;

public:
  explicit DSU(int N)
      : N(N), parent(Range<int>(1, N + 1)), component_count(N) {}

  int getparent(int u) {
    if (parent[u] != u)
      parent[u] = getparent(parent[u]);
    return parent[u];
  }

  bool same_component(int u, int v) { return getparent(u) == getparent(v); }
  int components() const { return component_count; }

  virtual ~DSU() {}
};

class DSUWithoutRank : public DSU {
public:
  using DSU::DSU;

  // guarantee that u -> v
  void merge_into(int u, int v) {
    int uroot = getparent(u);
    int vroot = getparent(v);

    if (uroot != vroot) {
      parent[uroot] = vroot;
      --component_count;
    }
  }
};

class DSUWithRank : public DSU {
  List<int, 1> rank;

public:
  explicit DSUWithRank(int N) : DSU(N), rank(N, 0) {}

  void merge_together(int u, int v) {
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
};
