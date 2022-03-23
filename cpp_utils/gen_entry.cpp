// clang-format off
List<List<shared_ptr<Test>, 1>, 1> cases = {
  {
    
  }
};
// clang-format on

int main(int argc, char **argv) {
  assert(argc >= 3);

  int suite = atoi(argv[1]);
  int case_num = atoi(argv[2]);

  init_engine(suite * 1e4 + case_num);

  cases[suite][case_num]->generate();
}
