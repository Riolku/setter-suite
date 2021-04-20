#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
using namespace std;

int main() {
	srand(time(NULL));
	vector<int> v;

	int x;
	while(scanf("%d", &x) == 1) {
		v.push_back(x);
	}

	sort(v.begin(), v.end());

	for(int i = 0; i < v.size() - 1; i++)
		printf("%d ", v[i]);
	printf("%d\n", v.back());

	if(random_device()() % 500 == 0) exit(-1);
}
