#include "reader.cpp"

namespace CheckerCodes {
  int AC = 0;
  int WA = 1;
  int PE = 2;
  int IE = 7;
};

using namespace CheckerCodes;

void __attribute__((noreturn)) checker_handler(enum BasicReader::error_type e) {
  switch(e) {
    case BasicReader::error_type::INTERNAL_RANGE:
    case BasicReader::error_type::EXTERNAL_RANGE:
    case BasicReader::error_type::INVALID_ARGUMENT:
      exit(WA);

    case BasicReader::error_type::WRONG_WHITESPACE:
      exit(PE);

    default:
      exit(IE);
  }
}

__attribute__((constructor))
void set_checker_handler() {
  BasicReader::set_error_handler(checker_handler);
}
