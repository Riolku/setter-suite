class StandardCheckerReader : public CheckerReader {

    bool isLine(char c) { return c == '\n' || c == '\r'; }
    bool isNonLineWhitespace(char c) { return c == '\v' || c == '\f' || c == '\t' || c == ' '; }
    bool isWhite(char c) { return isLine(c) || isNonLineWhitespace(c); }

    bool skipAllWhitespace() {
        bool any_line = false;
        while (isWhite(peekChar()) && !eof())
            any_line |= isLine(readChar());
        return any_line;
    }

  public:
    StandardCheckerReader(FILE *f) : CheckerReader(f) { skipAllWhitespace(); }
    StandardCheckerReader(char *path) : CheckerReader(path) { skipAllWhitespace(); }

    void readSpace() override {
        if (!isNonLineWhitespace(readChar()))
            wrongWhitespaceError();

        while (isNonLineWhitespace(peekChar()))
            readChar();
    }
    void readNewLine() override {
        // If we read all the whitespace and don't find a line, we should fail.
        // However, if we are at EOF, then we shouldn't.
        // If we are indeed at EOF, but the checker needs to read more things, those methods will fail.
        if (!skipAllWhitespace() && !eof())
            wrongWhitespaceError();
    }

    void readEOF() override {
        skipAllWhitespace();
        if (!eof())
            wrongWhitespaceError(); // wanted EOF but found more token
    }

    // PE could be confusing under standard checker
    void wrongWhitespaceError() override {
        preError();
        exit(CheckerCodes::WA);
    }

    void invalidIntegerError() override {
        preError();
        exit(CheckerCodes::WA);
    }
};
