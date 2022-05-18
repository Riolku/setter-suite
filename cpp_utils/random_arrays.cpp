template <typename T = ll> List<T> random_array(int N, T lo, T hi) {
  List<T> ret;
  ret.reserve(N);
  generate_n(back_inserter(ret), N, [lo, hi]() { return randint(lo, hi); });
  return ret;
}

template <typename T> void shuffle(vector<T> &arr) {
  shuffle(all(arr), get_engine());
}

List<int, 1> permutation(int N) {
  Range<int> r(1, N + 1);
  List<int, 1> ret(r.begin(), r.end());
  shuffle(ret);
  return ret;
}

template <typename T>
List<T> random_sorted_array_with_gaps(int N, T lo, T hi, T gap) {
  if (N == 0)
    return List<T>();

  List<T> ret = random_array(N, lo, hi - gap * (N - 1));
  sort(all(ret));
  if (gap != 0) {
    T i = 0;
    transform(ret.begin() + 1, ret.end(), ret.begin() + 1, [&i, gap](T x) {
      i += gap;
      return x + i;
    });
  }
  return ret;
}

template <typename T> List<T> with_gaps(int N, T lo, T hi, T gap) {
  List<T> ret = random_sorted_array_with_gaps(N, lo, hi, gap);
  shuffle(ret);
  return ret;
}

template <typename T> List<T> distinct_array(int N, T lo, T hi) {
  return with_gaps<T>(N, lo, hi, 1);
}

template <typename T> List<T> array_with_sum(int N, T sum, T lo) {
  List<T> ret = random_sorted_array_with_gaps(N - 1, 0, sum, lo);
  ret.reserve(N);
  ret.push_back(sum);
  adjacent_difference(all(ret), ret.begin());
  return ret;
}

template <typename T> class ArrayBuilder {
public:
  using BuilderInstruction = tuple<int, T, T>;
  using BuilderInstructionList = List<BuilderInstruction>;

protected:
  BuilderInstructionList instructions;

public:
  ArrayBuilder(List<BuilderInstruction> instructions)
      : instructions(move(instructions)) {}

  List<T> build() {
    List<T> ret;
    int list_size = 0;
    for (auto [c, l, r] : instructions) {
      list_size += c;
    }
    ret.reserve(list_size);
    for (auto [c, l, r] : instructions) {
      for (int i = 0; i < c; ++i) {
        ret.push_back(randint(l, r));
      }
    }
    shuffle(ret);
    return ret;
  }
};

using IntArrayBuilder = ArrayBuilder<int>;
using LongArrayBuilder = ArrayBuilder<ll>;
