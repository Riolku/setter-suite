namespace CheckerCodes {
int AC = 0;
int WA = 1;
int PE = 2;
int IE = 3;
int PARTIAL = 7;
} // namespace CheckerCodes

void preErrorHook();

void assertOrCode(bool cond, int code) {
  if (!cond) {
    preErrorHook();
    exit(code);
  }
}
void assertWA(bool cond) { assertOrCode(cond, CheckerCodes::WA); }
void assertPE(bool cond) { assertOrCode(cond, CheckerCodes::PE); }

int partial(int points, int denom) {
  fprintf(stderr, "partial %d/%d\n", points, denom);
  printf("%d/%d points", points, denom);
  return CheckerCodes::PARTIAL;
}

class CheckerReader : public BaseReader {
protected:
  virtual void preError() { preErrorHook(); }
  void externalRangeError() override {
    preError();
    exit(CheckerCodes::WA);
  }
  void internalRangeError() override {
    preError();
    exit(CheckerCodes::WA);
  }
  void wrongWhitespaceError() override {
    preError();
    exit(CheckerCodes::PE);
  }
  void invalidIntegerError() override {
    preError();
    exit(CheckerCodes::PE);
  }

public:
  using BaseReader::BaseReader;
};
