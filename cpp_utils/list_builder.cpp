template <typename T> struct ListPart {
  virtual List<T> build() = 0;

  virtual ~ListPart() {}
};

template <typename T> class SingleElement : public ListPart<T> {
  virtual T get_element() = 0;

public:
  List<T> build() override {
    return { get_element(); };
  }
};

template <typename T>
