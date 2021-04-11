#include "reader.cpp"

int main() {
  auto [a, b] = readLongTuple<2>(1, 100);

  readEOF();
}
