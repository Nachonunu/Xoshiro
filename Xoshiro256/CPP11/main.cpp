#include "Xoshiro256.hpp"

#include <stdio.h>

int main() {
	unsigned long long a[5] = {1,2,3,4,5};
	Xoshiro256pp<> rnd(a, 5);
	for (int i = 0; i < 100; ++i) printf("%llu\n", rnd());
	XOSHIRO256 seed = rnd.SaveData();
	printf("seed %llu\n", seed.s[0]);
	printf("seed %llu\n", seed.s[1]);
	printf("seed %llu\n", seed.s[2]);
	printf("seed %llu\n", seed.s[3]);
	return 0;
}
