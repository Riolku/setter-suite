int main(int argc, char** argv) {
  FileReader in_r(fopen(argv[1], "r"), Reader::runtime_error_handler), prog_r(fopen(argv[2], "r"), checker_handler);


}
