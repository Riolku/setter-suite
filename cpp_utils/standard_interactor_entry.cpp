void preInteractorError() {
  // Before exiting on errors, what does your interactor output?
}

int main(int argc, char **argv) {
  assert(argc >= 3);
  StandardInteractorReader user_r(stdin);
  ValidatingReader in_r(argv[1]), ref_r(argv[2]);
}
