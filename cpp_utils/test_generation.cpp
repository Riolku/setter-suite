struct Test {
  virtual void generate() = 0;

  virtual ~Test() {}
};

class LiteralTest : public Test {
  string input, output;

public:
  LiteralTest(string input, string output)
      : input(move(input)), output(move(output)) {}

  void generate() override {
    printf("%s", input.c_str());
    fprintf(stderr, "%s", output.c_str());
  }
};

class SolutionTestBase : public Test {
  virtual void get_input() = 0;
  virtual void print_input() = 0;
  virtual void solve_input() = 0;

public:
  void generate() override {
    get_input();
    set_stream(stdout);
    print_input();
    set_stream(stderr);
    solve_input();
  }
};

// For the classic "T tests" problems
class MultipleTests : public Test {
  unique_ptr<ListBuilder<unique_ptr<Test>>> builder;

public:
  MultipleTests(unique_ptr<ListBuilder<unique_ptr<Test>>> builder)
      : builder(move(builder)) {}

  void generate() override {
    auto tests = builder->build();
    print(tests.size());
    for (const auto &test : tests) {
      test->generate();
    }
  }
};

class MultipleTestsWithSumUnit : public SolutionTestBase {
public:
  virtual int get_unit_value() = 0;
};

class MultipleTestsWithSum : public Test {
  int total;
  unique_ptr<ListBuilder<unique_ptr<MultipleTestsWithSumUnit>>> builder;

public:
  MultipleTestsWithSum(
      int total,
      unique_ptr<ListBuilder<unique_ptr<MultipleTestsWithSumUnit>>> builder)
      : total(total), builder(move(builder)) {}

  void generate() override {
    auto tests = builder->build();
    int cur = 0;
    for (const auto &test : tests) {
      cur += test->get_unit_value();
    }
    while (cur > total) {
      cur -= tests.back()->get_unit_value();
      tests.pop_back();
    }
    assert(tests.size() > 0);

    print(tests.size());
    for (const auto &test : tests) {
      test->generate();
    }
  }
};
