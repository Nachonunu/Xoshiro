/*! Xoshiro256 CPP v0.2 | CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/deed) */
#ifndef _XOSHIRO256_HPP
#define _XOSHIRO256_HPP

#include <random>

typedef struct { unsigned long long s[4]; } XOSHIRO256;

inline unsigned long long splitmix64(unsigned long long& seed) {
	unsigned long long z = (seed += 0x9e3779b97f4a7c15);
	z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
	z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
	return z ^ (z >> 31);
}

template <class SEED = XOSHIRO256>
class Xoshiro256pp {
	SEED data;
	inline unsigned long long rotl(unsigned long long x, unsigned long long k) { return (x << k) | (x >> (64 - k)); }
public:
	using result_type = unsigned long long;
	Xoshiro256pp();
	Xoshiro256pp(result_type seed) { Init_Int(seed); }
	Xoshiro256pp(const unsigned long long seed[], unsigned long long seed_length) { Init_Array(seed, seed_length); }
	Xoshiro256pp(const SEED& seed) { Init_Data(seed); }
	~Xoshiro256pp() {}

	static constexpr result_type min() { return 0; }
	static constexpr result_type max() { return 0xffffffffffffffff; }
	result_type operator () () { return Random(); }

	void discard(unsigned long long z) { for (unsigned long long i = 0; i < z; ++i) Random(); }

	void Init_Int(result_type seed) {
		unsigned long long s = seed;
		data.s[0] = splitmix64(s);
		data.s[1] = splitmix64(s);
		data.s[2] = splitmix64(s);
		data.s[3] = splitmix64(s);
	}
	void Init_Array(const unsigned long long seed[], unsigned long long seed_length) {
		if (seed_length == 0) return;
		Init_Int(seed[0]);
		unsigned long long s;
		for (unsigned long long i = 1; i < seed_length; ++i) {
			s = seed[i];
			data.s[0] ^= splitmix64(s);
			data.s[1] ^= splitmix64(s);
			data.s[2] ^= splitmix64(s);
			data.s[3] ^= splitmix64(s);
		}
	}
	void Init_Data(const SEED& seed) { data = seed; }

	SEED SaveData() { return data; }

	result_type Random() {
		const unsigned long long result = rotl(data.s[0] + data.s[3], 23) + data.s[0];
		const unsigned long long t = data.s[1] << 17;
		data.s[2] ^= data.s[0];
		data.s[3] ^= data.s[1];
		data.s[1] ^= data.s[2];
		data.s[0] ^= data.s[3];
		data.s[2] ^= t;
		data.s[3] = rotl(data.s[3], 45);
		return result;
	}
};

template <class SEED = XOSHIRO256>
class Xoshiro256ss {
	SEED data;
	inline unsigned long long rotl(unsigned long long x, unsigned long long k) { return (x << k) | (x >> (64 - k)); }
public:
	using result_type = unsigned long long;
	Xoshiro256ss();
	Xoshiro256ss(result_type seed) { Init_Int(seed); }
	Xoshiro256ss(const unsigned long long seed[], unsigned long long seed_length) { Init_Array(seed, seed_length); }
	Xoshiro256ss(const SEED& seed) { Init_Data(seed); }
	~Xoshiro256ss() {}

	static constexpr result_type min() { return 0; }
	static constexpr result_type max() { return 0xffffffffffffffff; }
	result_type operator () () { return Random(); }

	void discard(unsigned long long z) { for (unsigned long long i = 0; i < z; ++i) Random(); }

	void Init_Int(result_type seed) {
		unsigned long long s = seed;
		data.s[0] = splitmix64(s);
		data.s[1] = splitmix64(s);
		data.s[2] = splitmix64(s);
		data.s[3] = splitmix64(s);
	}
	void Init_Array(const unsigned long long seed[], unsigned long long seed_length) {
		if (seed_length == 0) return;
		Init_Int(seed[0]);
		unsigned long long s;
		for (unsigned long long i = 1; i < seed_length; ++i) {
			s = seed[i];
			data.s[0] ^= splitmix64(s);
			data.s[1] ^= splitmix64(s);
			data.s[2] ^= splitmix64(s);
			data.s[3] ^= splitmix64(s);
		}
	}
	void Init_Data(const SEED& seed) { data = seed; }

	SEED SaveData() { return data; }

	result_type Random() {
		const unsigned long long result = rotl(data.s[1] * 5, 7) * 9;
		const unsigned long long t = data.s[1] << 17;
		data.s[2] ^= data.s[0];
		data.s[3] ^= data.s[1];
		data.s[1] ^= data.s[2];
		data.s[0] ^= data.s[3];
		data.s[2] ^= t;
		data.s[3] = rotl(data.s[3], 45);
		return result;
	}
};

template <class SEED = XOSHIRO256>
class Xoshiro256p {
	SEED data;
	inline unsigned long long rotl(unsigned long long x, unsigned long long k) { return (x << k) | (x >> (64 - k)); }
public:
	using result_type = unsigned long long;
	Xoshiro256p();
	Xoshiro256p(result_type seed) { Init_Int(seed); }
	Xoshiro256p(const unsigned long long seed[], unsigned long long seed_length) { Init_Array(seed, seed_length); }
	Xoshiro256p(const SEED& seed) { Init_Data(seed); }
	~Xoshiro256p() {}

	static constexpr result_type min() { return 0; }
	static constexpr result_type max() { return 0xffffffffffffffff; }
	result_type operator () () { return Random(); }

	void discard(unsigned long long z) { for (unsigned long long i = 0; i < z; ++i) Random(); }

	void Init_Int(result_type seed) {
		unsigned long long s = seed;
		data.s[0] = splitmix64(s);
		data.s[1] = splitmix64(s);
		data.s[2] = splitmix64(s);
		data.s[3] = splitmix64(s);
	}
	void Init_Array(const unsigned long long seed[], unsigned long long seed_length) {
		if (seed_length == 0) return;
		Init_Int(seed[0]);
		unsigned long long s;
		for (int i = 1; i < seed_length; ++i) {
			s = seed[i];
			data.s[0] ^= splitmix64(s);
			data.s[1] ^= splitmix64(s);
			data.s[2] ^= splitmix64(s);
			data.s[3] ^= splitmix64(s);
		}
	}
	void Init_Data(const SEED& seed) { data = seed; }

	SEED SaveData() { return data; }

	result_type Random() {
		const unsigned long long result = data.s[0] + data.s[3];
		const unsigned long long t = data.s[1] << 17;
		data.s[2] ^= data.s[0];
		data.s[3] ^= data.s[1];
		data.s[1] ^= data.s[2];
		data.s[0] ^= data.s[3];
		data.s[2] ^= t;
		data.s[3] = rotl(data.s[3], 45);
		return result;
	}
};
#endif /* _XOSHIRO256_HPP */

