namespace CheckerCodes {
  int AC = 0;
  int WA = 1;
  int PE = 2;
  int IE = 3;
  int PARTIAL = 7;
}

void __attribute__((noreturn)) checker_handler(enum Reader::error_type e) {
  switch(e) {
    case Reader::error_type::INTERNAL_RANGE:
    case Reader::error_type::EXTERNAL_RANGE:
      exit(CheckerCodes::WA);

    case Reader::error_type::INVALID_ARGUMENT:
    case Reader::error_type::WRONG_WHITESPACE:
      exit(CheckerCodes::PE);

    default:
      exit(CheckerCodes::IE);
  }
}

void assertWA(bool cond) {
  if(!cond) {
    exit(CheckerCodes::WA);
  }
}
