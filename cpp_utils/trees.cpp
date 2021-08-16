struct tree {
  int N;
  List<List<int>> edges;

  tree(int N) : N(N), edges(N - 1, {0, 0}) {}

  void permute() {
    vector<int> perm = permutation(N - 1);

    edges = edges.map<List<int>>([&](List<int>& edge) {
      return edge.map<int>([&](int x) {
        return perm[x - 1] + 1;
      });
    });
  }

  void print_edges() {
    edges.forEach([&](pair<int, int>& p) {
      print(p);
    });
  }

  void with_depth(int d) {
    List<int> depths(N, 0);

    RandomList<int> rl = { 1 };

    for(int i = 1; i <= d; i++) {
      edges[i - 1] = { i, i + 1 };

      depths[i] = i;

      if(i != d) rl.append(i);
    }

    for(int i = d + 1; i <= N; i++) {
      int x = rl.choice();

      depths[i] = depths[x] + 1;

      if(depths[i] != d) rl.append(i);

      edges[i - 1] = { x, i };
    }
  }
};
