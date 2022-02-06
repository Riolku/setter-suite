struct IntegerValidation {
    ll lo, hi;

    bool check(ll x) { return lo <= x && x <= hi; }
    template <typename R> bool check(const R &r) {
        for (auto x : r)
            if (!check(x))
                return false;
        return true;
    }

    template <typename T, typename... Ts> bool check(const T &arg, Ts &&... args) {
        return check(arg) && check(args...);
    }
};

class ValidatingReader : public BaseReader {
    using BaseReader::BaseReader;

    void externalRangeError() { throw runtime_error("EXTERNAL_RANGE"); }
    void internalRangeError() { throw runtime_error("INTERNAL_RANGE"); }
    void wrongWhitespaceError() { throw runtime_error("WRONG_WHITESPACE"); }
    void invalidIntegerError() { throw runtime_error("INVALID_INTEGER"); }
};
