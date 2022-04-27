int main(int argc, char **argv) {
  assert(argc >= 4);
  StandardCheckerReader user_r(argv[2]);
  ValidatingReader in_r(argv[1]), ref_r(argv[3]);
}
