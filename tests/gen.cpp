#include <iostream>
#include <stdlib.h>
using namespace std;

int main(int argc, char** argv) {
	int suite = atoi(argv[1]);
	int case_num = atoi(argv[2]);

	printf("%d %d\n", suite, case_num);
	fprintf(stderr, "%d\n", suite + case_num);
}
