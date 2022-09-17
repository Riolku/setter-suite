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
  unique_ptr<ListBuilder<shared_ptr<Test>>> builder;

public:
  MultipleTests(unique_ptr<ListBuilder<shared_ptr<Test>>> builder)
      : builder(move(builder)) {}

  void generate() override {
    auto tests = builder->build();
    print(tests.size());
    for (const auto &test : tests) {
      test->generate();
    }
  }
};

template <typename T> class RandomTestUnion : public ListBuilder<T> {
  T first, second;
  int num, denom;

public:
  RandomTestUnion(T first, T second, int num = 1, int denom = 2)
      : first(move(first)), second(move(second)), num(num), denom(denom) {}

  List<T> build() { return {randint(1, denom) <= num ? first : second}; }
};

class MultipleTestsWithSumUnit : public SolutionTestBase {
public:
  // When tests are duplicated, they share the same memory, and as such doing
  // initialization will overwrite The idea is to get the unit value which may
  // be constant, but is likely random, (but still does not modify the object)
  // And then store it before calling generate()
  virtual int get_unit_value() const = 0;
  virtual void store_unit_value(int) = 0;
};

class MultipleTestsWithSum : public Test {
  int max_total;
  unique_ptr<ListBuilder<shared_ptr<MultipleTestsWithSumUnit>>> builder;

public:
  MultipleTestsWithSum(
      int max_total,
      unique_ptr<ListBuilder<shared_ptr<MultipleTestsWithSumUnit>>> builder)
      : total(total), builder(move(builder)) {}

  void generate() override {
    List<shared_ptr<MultipleTestsWithSumUnit>, 1> tests = builder->build();
    int cur_total = 0;
    int test_count = 0;
    List<int, 1> test_sizes(tests.size(), -1);

    // Basically do greedy knapsack
    for (size_t ti = 1; ti <= tests.size(); ++ti) {
      int v = tests[ti]->get_unit_value();
      if (cur_total + v <= max_total) {
        ++test_count;
        cur_total += v;
        test_sizes[ti] = v;
      }
    }
    print(test_count);
    List<pair<shared_ptr<MultipleTestsWithSumUnit>, int>, 1> used_tests;
    used_tests.reserve(test_count);
    for (size_t ti = 1; ti <= tests.size(); ++ti) {
      int sz = test_sizes[ti];
      if (sz != -1) {
        used_tests.emplace_back(tests[ti], sz);
      }
    }
    shuffle(used_tests);

    for (auto [test, sz] : used_tests) {
      test->store_unit_value(sz);
      test->generate();
    }
  }
};
