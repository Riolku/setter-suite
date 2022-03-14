template <typename Parent> class ExactWhitespaceMixin : public Parent {
  protected:
    void readNewLine() override {
        if (Parent::readChar() != '\n')
            Parent::wrongWhitespaceError();
    }

    void readEOF() override {
        if (Parent::eof())
            Parent::wrongWhitespaceError();
    }

    void readSpace() override {
        if (!Parent::eof())
            Parent::wrongWhitespaceError();
    }

  public:
    using Parent::Parent;
};
