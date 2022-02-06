// modified from a template by wleung_bvg

class BaseReader {
    const static size_t MAX_TOKEN_SIZE = 1e6;

  private:
    FILE *stream;
    bool streamOpen;

    bool hasLast;
    char lastChar;

  public:
    BaseReader(FILE *f) : stream(f), streamOpen(true), hasLast(false), lastChar(0) {}

    BaseReader(char *path) : FileReader(fopen(path, "r")) {}

    char peekChar() {
        if (!hasLast) {
            lastChar = getc(stream);
            hasLast = true;
        }
        return lastChar;
    }

    char readChar() {
        char ret = peekChar();
        hasLast = false;
        return ret;
    }

    bool eof() { return peekChar() == char_traits<char>::eof(); }

    ll readInt(ll lo = numeric_limits<ll>::min(), ll hi = numeric_limits<ll>::max()) {
        try {
            ll value = fetchIntegerToken(readToken());
            if (ret < lo || ret > hi)
                internalRangeError();
            return value;
        } catch (const out_of_range &e) {
            externalRangeError();
        }
    }

  private:
    ll convertIntegerToken(const string &token) {
        if (token.size() == 0)
            invalidIntegerError();
        if (token == "0")
            return token;

        size_t i = 0;
        if (token[0] == '-') {
            ++i;
            if (token.size() == 1)
                invalidIntegerError();
        }

        if (token[i] > '9' || token[i] < '1')
            invalidIntegerError();
        ++i;

        for (; i < token.size(); ++i) {
            if (token[i] > '9' || token[i] < '0')
                invalidIntegerError();
        }

        return stoll(token);
    }

  public:
    void readSpace() {
        if (readChar() != ' ')
            wrongWhitespaceError();
    }

    void readNewLine() {
        if (peekChar() == '\r') {
            readChar();
            if (peekChar() == '\n')
                readChar();
        } else if (peekChar() == '\n') {
            readChar();
        } else {
            wrongWhitespaceError();
        }
    }

    void readEOF() {
        if (!eof())
            wrongWhitespaceError();
    }

    string readToken() {
        string token;
        while (!isspace(peekChar()) && !eof() && token.size() <= MAX_TOKEN_SIZE)
            token.push_back(readChar());
        return token;
    }

    string readString(size_t N) {
        string token;
        while (!eof() && token.size() < N) {
            token.push_back(readChar());
        }
        return token;
    }

  protected:
    template <typename Arr> void _fill_arr(Arr &a, size_t N, ll lo, ll hi) {
        for (size_t i = 0; i < N; i++) {
            if (i)
                readSpace();
            a[i] = readInt(lo, hi);
        }
        readNewLine();
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
        readSpace();
        readInts(args...);
    }

    template <typename T> void readInts(T &arg) {
        arg = readInt();
        readNewLine();
    }

    void closeStream() {
        if (streamOpen) {
            fclose(stream);
            streamOpen = false;
        }
    }

  protected:
    virtual void externalRangeError() __attribute__((noreturn)) = 0;
    virtual void internalRangeError() __attribute__((noreturn)) = 0;
    virtual void wrongWhitespaceError() __attribute__((noreturn)) = 0;
    virtual void invalidIntegerError() __attribute__((noreturn)) = 0;

  public:
    virtual ~BaseReader() { closeStream(); }
};
