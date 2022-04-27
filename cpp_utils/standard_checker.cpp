class StandardCheckerReader : public CheckerReader {
  // 0 - don't skip space
  // 1 - skip non-lines
  // 2 - skip all space
  // 3 - same as 2, but don't error if there is no space
  int whitespace_flag;

  bool isLine(char c) { return c == '\n' || c == '\r'; }
  bool isNonLineWhitespace(char c) {
    return c == '\v' || c == '\f' || c == '\t' || c == ' ';
  }
  bool isWhite(char c) { return isLine(c) || isNonLineWhitespace(c); }

  bool skipAllWhitespace() {
    bool any_line = false;
    while (isWhite(peekChar()) && !eof())
      any_line |= isLine(readChar());
    return any_line;
  }

public:
  StandardCheckerReader(FILE *f) : CheckerReader(f), whitespace_flag(3) {}
  StandardCheckerReader(const char *path)
      : CheckerReader(path), whitespace_flag(3) {}

  void readSpace() override { whitespace_flag = 1; }
  void readNewLine() override { whitespace_flag = 2; }

  void preReadToken() override {
    if (whitespace_flag == 1) {
      if (!isNonLineWhitespace(readChar())) {
        wrongWhitespaceError();
      }

      while (isNonLineWhitespace(peekChar())) {
        readChar();
      }
    } else if (whitespace_flag == 2 || whitespace_flag == 3) {
      if (!skipAllWhitespace() && whitespace_flag != 3) {
        wrongWhitespaceError();
      }
    } else {
      assert(whitespace_flag == 0);
    }
    whitespace_flag = 0;
  }

  void readEOFImpl() {
    skipAllWhitespace();
    if (!eof())
      wrongWhitespaceError(); // wanted EOF but found more token
  }
  void readEOF() override { readEOFImpl(); }

  // PE could be confusing under standard checker
  void wrongWhitespaceError() override {
    preError();
    exit(CheckerCodes::WA);
  }

  void invalidIntegerError() override {
    preError();
    exit(CheckerCodes::WA);
  }

  ~StandardCheckerReader() { readEOFImpl(); }
};
