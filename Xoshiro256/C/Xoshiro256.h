/*! Xoshiro256 C v0.2 | CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/deed) */
#ifndef _XOSHIRO256_H
#define _XOSHIRO256_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct { unsigned long long s[4]; } XOSHIRO256;

XOSHIRO256 Xoshiro256_Init_Int(unsigned long long seed);
XOSHIRO256 Xoshiro256_Init_Array(unsigned long long* seed, int seed_length);

/* Xoshiro256pp */
unsigned long long Xoshiro256pp_Gen_Uint64(XOSHIRO256* data);
long double Xoshiro256pp_Gen_Real(XOSHIRO256* data);
unsigned long long Xoshiro256pp_Gen_Free(XOSHIRO256* data, unsigned long long num);

/* Xoshiro256ss */
unsigned long long Xoshiro256ss_Gen_Uint64(XOSHIRO256* data);
long double Xoshiro256ss_Gen_Real(XOSHIRO256* data);
unsigned long long Xoshiro256ss_Gen_Free(XOSHIRO256* data, unsigned long long num);

/* Xoshiro256p */
unsigned long long Xoshiro256p_Gen_Uint64(XOSHIRO256* data);
long double Xoshiro256p_Gen_Real(XOSHIRO256* data);
unsigned long long Xoshiro256p_Gen_Free(XOSHIRO256* data, unsigned long long num);

#ifdef __cplusplus
}
#endif
#endif /* _XOSHIRO256_H */

#ifdef _XOSHIRO256_IMPLEMENTATION

unsigned long long splitmix64(unsigned long long* seed) {
	unsigned long long z = (*seed += 0x9e3779b97f4a7c15);
	z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
	z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
	return z ^ (z >> 31);
}

XOSHIRO256 Xoshiro256_Init_Int(unsigned long long seed) {
	XOSHIRO256 data;
	unsigned long long s = seed;
	data.s[0] = splitmix64(&s);
	data.s[1] = splitmix64(&s);
	data.s[2] = splitmix64(&s);
	data.s[3] = splitmix64(&s);
	return data;
}
XOSHIRO256 Xoshiro256_Init_Array(unsigned long long* seed, int seed_length) {
	if (seed_length == 0) { XOSHIRO256 d; return d; }
	XOSHIRO256 data = Xoshiro256_Init_Int(seed[0]);
	unsigned long long s;
	for (int i = 1; i < seed_length; ++i) {
		s = seed[i];
		data.s[0] ^= splitmix64(&s);
		data.s[1] ^= splitmix64(&s);
		data.s[2] ^= splitmix64(&s);
		data.s[3] ^= splitmix64(&s);
	}
	return data;
}

#define rotl(x, k) ((x << k) | (x >> (64 - k)))

/* Xoshiro256pp */
unsigned long long Xoshiro256pp_Gen_Uint64(XOSHIRO256* data) {
	const unsigned long long result = rotl((data->s[0] + data->s[3]), 23) + data->s[0];
	const unsigned long long t = data->s[1] << 17;
	data->s[2] ^= data->s[0];
	data->s[3] ^= data->s[1];
	data->s[1] ^= data->s[2];
	data->s[0] ^= data->s[3];
	data->s[2] ^= t;
	data->s[3] = rotl(data->s[3], 45);
	return result;
}
long double Xoshiro256pp_Gen_Real(XOSHIRO256* data) { return Xoshiro256pp_Gen_Uint64(data) / 18446744073709551616.0L; } /* [0,1) */
unsigned long long Xoshiro256pp_Gen_Free(XOSHIRO256* data, unsigned long long num) { return num > 1 ? (unsigned long long)(Xoshiro256pp_Gen_Real(data) * num) : 0; } /* [0,num) */

/* Xoshiro256ss */
unsigned long long Xoshiro256ss_Gen_Uint64(XOSHIRO256* data) {
	const unsigned long long result = rotl((data->s[1] * 5), 7) * 9;
	const unsigned long long t = data->s[1] << 17;
	data->s[2] ^= data->s[0];
	data->s[3] ^= data->s[1];
	data->s[1] ^= data->s[2];
	data->s[0] ^= data->s[3];
	data->s[2] ^= t;
	data->s[3] = rotl(data->s[3], 45);
	return result;
}
long double Xoshiro256ss_Gen_Real(XOSHIRO256* data) { return Xoshiro256ss_Gen_Uint64(data) / 18446744073709551616.0L; } /* [0,1) */
unsigned long long Xoshiro256ss_Gen_Free(XOSHIRO256* data, unsigned long long num) { return num > 1 ? (unsigned long long)(Xoshiro256ss_Gen_Real(data) * num) : 0; } /* [0,num) */

/* Xoshiro256p */
unsigned long long Xoshiro256p_Gen_Uint64(XOSHIRO256* data) {
	const unsigned long long result = data->s[0] + data->s[3];
	const unsigned long long t = data->s[1] << 17;
	data->s[2] ^= data->s[0];
	data->s[3] ^= data->s[1];
	data->s[1] ^= data->s[2];
	data->s[0] ^= data->s[3];
	data->s[2] ^= t;
	data->s[3] = rotl(data->s[3], 45);
	return result;
}
long double Xoshiro256p_Gen_Real(XOSHIRO256* data) { return Xoshiro256p_Gen_Uint64(data) / 18446744073709551616.0L; } /* [0,1) */
unsigned long long Xoshiro256p_Gen_Free(XOSHIRO256* data, unsigned long long num) { return num > 1 ? (unsigned long long)(Xoshiro256p_Gen_Real(data) * num) : 0; } /* [0,num) */
#endif /* _XOSHIRO256_IMPLEMENTATION */

