void assert_permutation(const vector<int> &arr) {
  vector<int> check(arr.size(), 0);
  assert(std::all_of(arr.begin(), arr.end(), [&check](int x) { return ++check[x - 1] == 1; }));
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
