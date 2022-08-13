template <typename T> struct ListBuilder {
  virtual List<T> build() = 0;

  virtual ~ListBuilder() {}
};

template <typename T> class SingleElement : public ListBuilder<T> {
  T x;

public:
  List<T> build() override { return {x}; }

  SingleElement(T x) : x(move(x)) {}
};

template <typename T> class MultipleElements : public ListBuilder<T> {
  List<T> elems;

public:
  List<T> build() override { return elems; }

  MultipleElements(List<T> elems) : elems(move(elems)) {}
};

template <typename T> class SerialElements : public ListBuilder<T> {
  using PartType = ListBuilder<T>;
  using PartPtr = unique_ptr<PartType>;
  using PartList = List<PartPtr>;

  PartList to_parts(PartList cur) { return cur; }
  template <typename... Ts>
  PartList to_parts(PartList cur, PartPtr ptr, Ts &&...rest) {
    cur.push_back(move(ptr));
    return to_parts(move(cur), forward<Ts>(rest)...);
  }
  template <typename... Ts>
  PartList to_parts(PartList cur, T elem, Ts &&...rest) {
    return to_parts(move(cur), make_unique<SingleElement<T>>(move(elem)),
                    forward<Ts>(rest)...);
  }

  PartList parts;

public:
  template <typename... Ts>
  SerialElements(Ts &&...rest)
      : parts(to_parts(PartList{}, forward<Ts>(rest)...)) {}

  List<T> build() override {
    List<T> ret;
    for (const auto &part : parts) {
      ret += part->build();
    }
    return ret;
  }
};

template <typename T> class RepeatElement : public ListBuilder<T> {
  int N;
  unique_ptr<ListBuilder<T>> part;

public:
  RepeatElement(int N, unique_ptr<ListBuilder<T>> part)
      : N(N), part(move(part)) {}

  RepeatElement(int N, T elem)
      : RepeatElement(N, make_unique<SingleElement<T>>(move(elem))) {}

  List<T> build() {
    List<T> ret;
    for (int i = 0; i < N; ++i) {
      ret += part->build();
    }
    return ret;
  }
};
