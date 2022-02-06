namespace CheckerCodes {
int AC = 0;
int WA = 1;
int PE = 2;
int IE = 3;
int PARTIAL = 7;
} // namespace CheckerCodes

void assertOrCode(bool cond, int code) {
    if (!cond)
        exit(code);
}
void assertWA(bool cond) { assertOrCode(cond, CheckerCodes::WA); }
void assertPE(bool cond) { assertOrCode(cond, CheckerCodes::WA); }

class CheckerReader : public BaseReader {
    void externalRangeError() override { exit(CheckerCodes::WA); }
    void internalRangeError() override { exit(CheckerCodes::WA); }
    void wrongWhitespaceError() override {
        puts("Check your Whitespace");
        exit(CheckerCodes::PE);
    }
    void invalidIntegerError() override {
        puts("Check your Integers");
        exit(CheckerCodes::PE);
    }

  public:
    using BaseReader::BaseReader;
};
