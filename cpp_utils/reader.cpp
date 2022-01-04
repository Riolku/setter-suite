// modified from a template by wleung_bvg

namespace Reader {
const int ERROR_COUNT = 4;

enum error_type { INTERNAL_RANGE, EXTERNAL_RANGE, INVALID_ARGUMENT, WRONG_WHITESPACE };

const char *error_names[ERROR_COUNT] = {"INTERNAL_RANGE", "EXTERNAL_RANGE", "INVALID_ARGUMENT", "WRONG_WHITESPACE"};

class FileReader {

  private:
    FILE *stream;

    bool hasLast;
    char lastChar;

  public:
    FileReader(FILE *f) : stream(f), hasLast(false), lastChar(0) {}

    FileReader(char *path) : FileReader(fopen(path, "r")) {}

    virtual void __attribute__((noreturn)) errorHandler(error_type e) = 0;

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

    void trim() {
        while (isspace(peekChar()) && !eof()) {
            readChar();
        }
    }

    ll readInt(ll lo = numeric_limits<ll>::min(), ll hi = numeric_limits<ll>::max()) {
        string token = "";
        while (isdigit(peekChar()) || peekChar() == '-')
            token.push_back(readChar());
        try {
            ll ret = stoll(token);

            if (lo > ret || hi < ret)
                errorHandler(INTERNAL_RANGE);

            char c = token[0];
            if (c == '-')
                c = token[1];

            if ((token != "0") && ('1' > c || c > '9'))
                errorHandler(INVALID_ARGUMENT);

            return ret;
        } catch (const out_of_range &e) {
            errorHandler(EXTERNAL_RANGE);
        } catch (const invalid_argument &e) {
            errorHandler(INVALID_ARGUMENT);
        }
        throw runtime_error("We should never get here");
    }

    ld readFloat() {
        string token = "";
        while (isdigit(peekChar()) || peekChar() == '-' || peekChar() == '.')
            token.push_back(readChar());

        try {
            ld ret = stold(token);

            return ret;
        } catch (const out_of_range &e) {
            errorHandler(EXTERNAL_RANGE);
        } catch (const invalid_argument &e) {
            errorHandler(INVALID_ARGUMENT);
        }
        throw runtime_error("We should never get here");
    }

    string readFile() {
        string ret = "";
        while (!eof()) {
            ret.push_back(readChar());
        }

        readEOF();

        return ret;
    }

    string readString(int N) {
        string ret = "";
        for (int i = 0; i < N; i++) {
            ret.push_back(readChar());
        }

        return ret;
    }

    string readLine() {
        string ret = "";
        while (peekChar() != '\n' && !eof())
            ret.push_back(readChar());
        readNewLine();

        return ret;
    }

    void readSpace() {
        if (readChar() != ' ')
            errorHandler(WRONG_WHITESPACE);
    }

    void readNewLine() {
        if (readChar() != '\n')
            errorHandler(WRONG_WHITESPACE);
    }

    void readEOF() {
        if (!eof())
            errorHandler(WRONG_WHITESPACE);
    }

    string readToken() {
        string token = "";

        while (!isspace(peekChar()) && !eof())
            token.push_back(readChar());

        return token;
    }

  private:
    template <typename Arr> void _fill_arr(Arr &a, size_t N, ll lo, ll hi) {
        for (size_t i = 0; i < N; i++) {
            if (i)
                readSpace();
            a[i] = readInt(lo, hi);
        }
        readNewLine();
        return;
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
};

class ValidatingReader : public FileReader {
    using FileReader::FileReader;

    void __attribute__((noreturn)) errorHandler(error_type e) {
        if (e >= ERROR_COUNT)
            throw runtime_error("Unknown error in FileReader");

        throw runtime_error(error_names[e]);
    }
};
} // namespace Reader

using Reader::ValidatingReader;
