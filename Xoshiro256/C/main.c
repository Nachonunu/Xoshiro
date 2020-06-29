#define _XOSHIRO256_IMPLEMENTATION
#include "Xoshiro256.h"

#include <stdio.h>

int main() {
	unsigned long long a[5] = {1,2,3,4,5};
	XOSHIRO256 seed = Xoshiro256_Init_Array(a, 5);
	for (int i = 0; i < 100; ++i) printf("%I64u\n", Xoshiro256pp_Gen_Uint64(&seed));
	printf("seed %I64u\n", seed.s[0]);
	printf("seed %I64u\n", seed.s[1]);
	printf("seed %I64u\n", seed.s[2]);
	printf("seed %I64u\n", seed.s[3]);
	return 0;
}
