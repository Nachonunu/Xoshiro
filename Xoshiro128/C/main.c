#define _XOSHIRO_IMPLEMENTATION
#include "Xoshiro128.h"

#include <stdio.h>

int main() {

	unsigned long s[5] = { 1,2,3,4,5 };
	XOSHIRO128 seed = Xoshiro128_Init_Array(s, 5);

	for (int i = 0; i < 100; ++i) printf("%lu\n", Xoshiro128ss_Gen_Uint32(&seed));
	printf("seed %lu\n", seed.s[0]);
	printf("seed %lu\n", seed.s[1]);
	printf("seed %lu\n", seed.s[2]);
	printf("seed %lu\n", seed.s[3]);
	return 0;
}
