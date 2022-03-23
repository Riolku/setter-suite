// clang-format off
List<List<shared_ptr<Test>, 1>, 1> cases = {
  {

  }
};
// clang-format on

int main(int argc, char **argv) {
  if (argv[1] == string("case-counts")) {
    print(cases.map_new(
        [](const List<shared_ptr<Test>, 1> &suite) { return suite.size(); }));
    return 0;
  }

  assert(argc >= 3);

  int suite = atoi(argv[1]);
  int case_num = atoi(argv[2]);

  init_engine(suite * 1e4 + case_num);

  cases[suite][case_num]->generate();
}
