void print(vector<ll> arr, FILE* stream = stdout) {
  for(int i = 0; i < arr.size(); i++) {
    fprintf(stream, "%lld", arr[i]);

    if(i + 1 == arr.size()) {
      fprintf(stream, "\n");
    }
    else {
      fprintf(stream, " ");
    }
  }
}

void print(ll a, ll b, FILE* stream = stdout) {
  print(vector<ll>{a, b}, stream);
}

void print(pair<ll, ll> p, FILE* stream = stdout) {
  print(p.first, p.second, stream);
}
