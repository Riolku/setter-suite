template<typename T>
bool is_distinct(const vector<T> arr) {
  vector<T> cpy(arr.size(), T());

  copy(arr.begin(), arr.end(), cpy.begin());

  sort(cpy.begin(), cpy.end());

  for(int i = 1; i < cpy.size(); i++) if(cpy[i] == cpy[i - 1]) return false;

  return true;
}

template<typename T>
vector<pair<T, int>> enumerate(const vector<T> arr) {
  vector<pair<T, int>> ret;

  for(int i = 0; i < arr.size(); i++) {
    ret.emplace_back(arr[i], i);
  }

  return ret;
}

template<typename R, typename T>
vector<R> map(function<R(T)> func, const vector<T> arr) {
  vector<R> ret;

  for(int i = 0; i < arr.size(); i++) {
    ret.push_back(func(arr[i]));
  }

  return ret;
}

template<typename T>
vector<T> distinct(vector<T> arr) {
  vector<T> sarr = arr;

  sort(sarr.begin(), sarr.end());

  vector<T> ret;

  for(int i = 0; i < sarr.size(); i++) {
    if(i == 0 || sarr[i] != sarr[i - 1]) {
      ret.push_back(sarr[i]);
    }
  }

  return ret;
}
