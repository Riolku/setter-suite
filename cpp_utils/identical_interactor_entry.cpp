// Hook for all library functions to call before exiting on error.
void preErrorHook() {}

int main(int argc, char **argv) {
  assert(argc >= 3);
  IdenticalInteractorReader user_r(stdin);
  ValidatingReader in_r(argv[1]), ref_r(argv[2]);
}
