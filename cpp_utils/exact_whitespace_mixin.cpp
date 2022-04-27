template <typename Parent> class ExactWhitespaceMixin : public Parent {
public:
  void readNewLine() override {
    if (Parent::rawReadChar() != '\n')
      Parent::wrongWhitespaceError();
  }

  void readEOFImpl() {
    if (!Parent::eof())
      Parent::wrongWhitespaceError();
  }
  void readEOF() override { readEOFImpl(); }

  void readSpace() override {
    if (Parent::rawReadChar() != ' ')
      Parent::wrongWhitespaceError();
  }

  ~ExactWhitespaceMixin() { readEOFImpl(); }

  using Parent::Parent;
};
