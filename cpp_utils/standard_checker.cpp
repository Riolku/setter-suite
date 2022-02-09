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

    void readSpaces() {
        while (isNonLineWhitespace(peekChar()) && !eof())
            readChar();
    }
    void readLines() {
        if (!skipAllWhitespace())
            wrongWhitespaceError(); // wanted a line but got none
    }

    void readSpace() { throw runtime_error("do not call readSpace() under standard checker, use readSpaces()"); }
    void readNewLine() { throw runtime_error("do not call readLine() under standard checker, use readLines()"); }
    void readEOF() { throw runtime_error("do not call readEOF() under standard checker"); }

    string readToken() {
        readSpaces();
        return CheckerReader::readToken();
    }

  protected:
    template <typename Arr> void _fill_arr(Arr &a, size_t N, ll lo, ll hi) {
        for (size_t i = 0; i < N; i++)
            a[i] = readInt(lo, hi);
    }

  public:
    template <size_t length, typename T = ll>
    array<T, length> readIntTuple(ll lo = numeric_limits<ll>::min(), ll hi = numeric_limits<ll>::max()) {
        array<T, length> res;
        _fill_arr(res, length, lo, hi);
        return res;
    }

    template <typename T = ll>
    vector<T> readIntArray(size_t N, ll lo = numeric_limits<ll>::min(), ll hi = numeric_limits<ll>::max()) {
        vector<T> v;
        v.resize(N);
        _fill_arr(v, N, lo, hi);
        return v;
    }

    template <typename T, typename... Ts> void readInts(T &arg, Ts &&... args) {
        arg = readInt();
        readInts(args...);
    }

    template <typename T> void readInts(T &arg) { arg = readInt(); }

    // PE could be confusing under standard checker
    void wrongWhitespaceError() override {
        preError();
        exit(CheckerCodes::WA);
    }
};
