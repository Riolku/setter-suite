template <typename Parent> class ExactWhitespaceMixin : public Parent {
  void readEOF() {
    if (!Parent::eof())
      Parent::wrongWhitespaceError();
  }

public:
  void readNewLine() override {
    if (Parent::rawReadChar() != '\n')
      Parent::wrongWhitespaceError();
  }

  void readSpace() override {
    if (Parent::rawReadChar() != ' ')
      Parent::wrongWhitespaceError();
  }

  ~ExactWhitespaceMixin() { readEOF(); }

  using Parent::Parent;
};
