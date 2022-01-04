namespace CheckerCodes {
int AC = 0;
int WA = 1;
int PE = 2;
int IE = 3;
int PARTIAL = 7;
} // namespace CheckerCodes

class CheckerReader : public Reader::FileReader {
    using FileReader::FileReader;

    void __attribute__((noreturn)) errorHandler(enum Reader::error_type e) {
        switch (e) {
        case Reader::error_type::OUT_OF_RANGE:
            exit(CheckerCodes::WA);

        case Reader::error_type::INVALID_ARGUMENT:
        case Reader::error_type::WRONG_WHITESPACE:
            exit(CheckerCodes::PE);

        default:
            exit(CheckerCodes::IE);
        }
    }
};

void assertWA(bool cond) {
    if (!cond) {
        exit(CheckerCodes::WA);
    }
}
