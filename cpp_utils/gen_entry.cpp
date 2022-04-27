// clang-format off
List<List<shared_ptr<Test>, 1>, 1> cases = {
  {

  }
};
// clang-format on
vector<string> case_count_flags = {"case-counts", "cc", "case_counts"};

int main(int argc, char **argv) {
  assert(argc >= 2);
  if (find(all(case_count_flags), argv[1]) != case_count_flags.end()) {
    print(cases.map_new([](const List<shared_ptr<Test>, 1> &suite) -> size_t {
      return suite.size();
    }));
    return 0;
  }

  assert(argc >= 3);

  int suite = atoi(argv[1]);
  int case_num = atoi(argv[2]);

  init_engine(suite * 1e4 + case_num);

  cases[suite][case_num]->generate();
}
