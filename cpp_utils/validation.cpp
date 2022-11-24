void assert_permutation(const List<int, 1> &arr) {
  List<int, 1> check(arr.size(), 0);
  assert(arr.all_of([&check](int x) { return ++check[x] == 1; }));
}

class validator_out_of_range {};
class wrong_whitespace {};
class invalid_integer {};

class ValidatingReaderBase : public BaseReader {
protected:
  void externalRangeError() override { throw validator_out_of_range(); }
  void internalRangeError() override { throw validator_out_of_range(); }
  void wrongWhitespaceError() override { throw wrong_whitespace(); }
  void invalidIntegerError() override { throw invalid_integer(); }

public:
  using BaseReader::BaseReader;
};

using ValidatingReader = ExactWhitespaceMixin<ValidatingReaderBase>;
