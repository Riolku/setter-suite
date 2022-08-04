class RandomSetBit : public SetWithBIT {
public:
  using SetWithBIT::SetWithBIT;

  int random() const { return (*this)[randint(1, size())]; }
  int pop_random() {
    int x = random();
    remove(x);
    return x;
  }
};

// A container class for random data.
// Push adds, and Pop removes and returns a random element.
template <typename T> class RandomStack {
  List<T, 1> data;

public:
  RandomStack() {}

  size_t size() { return data.size(); }

  void push(T x) { data.push_back(move(x)); }

  T pop() {
    int sz = size();
    swap(data[randint(1, sz)], data[sz]);
    T ret(move(data.back()));
    data.pop_back();
    return ret;
  }
};
