struct graph {
  int N, M;
  vector<pair<int, int>> edges;

  graph(int N, int M) : N(N), M(M), edges(M, {0, 0}) {}

  void permute() {
    vector<int> perm = permutation(N);

    for(int i = 0; i < M; i++) {
      edges[i] = {perm[edges[i].first - 1], perm[edges[i].second - 1]};
    }
  }

  void print_edges(bool shuffle_edges = true, FILE* stream = stdout) {
    if(shuffle_edges) permute();

    for(int i = 0; i < M; i++) {
      fprintf(stream, "%d %d\n", edges[i].first, edges[i].second);
    }
  }
};
