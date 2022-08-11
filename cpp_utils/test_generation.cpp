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
public:
  virtual void get_input() = 0;
  virtual void print_input() = 0;
  virtual void solve_input() = 0;

  void generate() override {
    get_input();
    print_input();
    solve_input();
  }
};

class SingleTest

    // For the classic "T test cases" problems
    class MultipleTests : public Test {};
