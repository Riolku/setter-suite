class IdenticalCheckerReader : public ExactWhitespaceMixin<CheckerReader> {
protected:
  virtual FILE *getHelperStream() { return stdout; }
  void wrongWhitespaceError() override {
    preError();
    fprintf(getHelperStream(), "Check your Whitespace");
    exit(CheckerCodes::PE);
  }
  void invalidIntegerError() override {
    preError();
    fprintf(getHelperStream(), "Check your Integers");
    exit(CheckerCodes::PE);
  }

public:
  using ExactWhitespaceMixin<CheckerReader>::ExactWhitespaceMixin;
};
