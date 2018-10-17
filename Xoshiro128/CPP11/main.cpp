#include "Xoshiro128.hpp"

#include <time.h>
#include <stdio.h>

int main() {
	unsigned long s[5] = { 1,2,3,4,5 };
	Xoshiro<> rnd(s, 5);

	for (int i = 0; i < 100; ++i) printf("%lu\n", rnd());
	
	XOSHIRO128 seed = rnd.SaveData();
	printf("seed %lu\n", seed.s[0]);
	printf("seed %lu\n", seed.s[1]);
	printf("seed %lu\n", seed.s[2]);
	printf("seed %lu\n", seed.s[3]);
	return 0;
}
