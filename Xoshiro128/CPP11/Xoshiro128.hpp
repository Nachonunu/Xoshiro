/*! Xoshiro128 CPP11 v0.2 | CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/deed) */
#ifndef _XOSHIRO_HPP
#define _XOSHIRO_HPP

#include <random>
using namespace std;

typedef struct { unsigned long s[4]; } XOSHIRO128;

template <class SEED = XOSHIRO128>
class Xoshiro128pp {
	SEED data;
	inline unsigned long rotl(unsigned long x, unsigned long k) { return (x << k) | (x >> (32 - k)); }

#ifdef _32BIT_ONLY
	inline unsigned long long splitmix64_32(unsigned long* low, unsigned long* high) {
		unsigned long temp = ((((*low & 0xffff) + 0x7c15) >> 16) + (*low >> 16) + 0x7f4a) >> 16, z0l, z0h;
		unsigned long z[2];
		z[0] = (*low  +=  0x7f4a7c15);
		z[1] = (*high += (0x9e3779b9 + temp));

		z[0] ^= (z[0] >> 30) + (z[1] << 2); z[1] ^= z[1] >> 30;
		z0l = z[0] & 0xffff; z0h = z[0] >> 16;
		temp = ((((z0l * 0xe5b9) >> 16) + (z0l * 0x1ce4) + (z0h * 0xe5b9)) >> 16) + (z0h * 0x1ce4) + z[0] * 0xbf58476d + z[1] * 0x1ce4e5b9;
		z[0] = z[0] * 0x1ce4e5b9; z[1] = temp;

		z[0] ^= (z[0] >> 27) + (z[1] << 5); z[1] ^= z[1] >> 27;
		z0l = z[0] & 0xffff; z0h = z[0] >> 16;
		temp = ((((z0l * 0x11eb) >> 16) + (z0l * 0x1331) + (z0h * 0x11eb)) >> 16) + (z0h * 0x1331) + z[0] * 0x94d049bb + z[1] * 0x133111eb;
		z[0] = z[0] * 0x133111eb; z[1] = temp;

		z[0] ^= (z[0] >> 31) + (z[1] << 1); z[1] ^= z[1] >> 31;
		return z[0] + ((unsigned long long)z[1] << 32);
	}
#else
	inline unsigned long long splitmix64(unsigned long long* seed) {
		unsigned long long z = (*seed += 0x9e3779b97f4a7c15);
		z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
		z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
		return z ^ (z >> 31);
	}
#endif
public:
	using result_type = unsigned long;
	Xoshiro128pp() {}
	Xoshiro128pp(result_type seed) { Init_Int(seed); }
	Xoshiro128pp(unsigned long seed[], unsigned long seed_length) { Init_Array(seed, seed_length); }
	Xoshiro128pp(SEED* seed) { Init_Data(seed); }
	~Xoshiro128pp() {}

	//operator
	static constexpr result_type min() { return 0; }
	static constexpr result_type max() { return 0xffffffffUL; }
	result_type operator () () { return Random(); }

	void discard(uint64_t z) { for (uint64_t i = 0; i < z; ++i) Random(); }

	//seed値生成
	void Init_Int(unsigned long seed) {
		unsigned long long sp_out;
#ifdef _32BIT_ONLY
		unsigned long sp_seed32[2] = { seed, 0 };
		sp_out = splitmix64_32(&sp_seed32[0], &sp_seed32[1]);
		data.s[0] = sp_out & 0xffffffff; data.s[1] = sp_out >> 32;
		sp_out = splitmix64_32(&sp_seed32[0], &sp_seed32[1]);
		data.s[2] = sp_out & 0xffffffff; data.s[3] = sp_out >> 32;
#else
		unsigned long long sp_seed = seed;
		sp_out = splitmix64(&sp_seed);
		data.s[0] = sp_out & 0xffffffff; data.s[1] = sp_out >> 32;
		sp_out = splitmix64(&sp_seed);
		data.s[2] = sp_out & 0xffffffff; data.s[3] = sp_out >> 32;
#endif
	}
	void Init_Array(unsigned long seed[], unsigned long seed_length) {
		if (seed_length == 0) { return Init_Int(0); }
		else if (seed_length == 1) { return Init_Int(seed[0]); }
		unsigned long long out;
#ifdef _32BIT_ONLY
		unsigned long sl = seed[0], sh = seed[1];
		out = splitmix64_32(&sl, &sh); data.s[0] = out & 0xffffffff; data.s[1] = out >> 32;
		out = splitmix64_32(&sl, &sh); data.s[2] = out & 0xffffffff; data.s[3] = out >> 32;
		for (unsigned long i = 3; i < seed_length; i += 2) {
			sl = seed[i - 1], sh = seed[i];
			out = splitmix64_32(&sl, &sh); data.s[0] ^= out & 0xffffffff; data.s[1] ^= out >> 32;
			out = splitmix64_32(&sl, &sh); data.s[2] ^= out & 0xffffffff; data.s[3] ^= out >> 32;
		}
		if (seed_length & 1) {
			sl = seed[seed_length - 1], sh = 0;
			out = splitmix64_32(&sl, &sh); data.s[0] ^= out & 0xffffffff; data.s[1] ^= out >> 32;
			out = splitmix64_32(&sl, &sh); data.s[2] ^= out & 0xffffffff; data.s[3] ^= out >> 32;
		}
#else
		unsigned long long s = seed[0] + ((unsigned long long)seed[1] << 32);
		out = splitmix64(&s); data.s[0] = out & 0xffffffff; data.s[1] = out >> 32;
		out = splitmix64(&s); data.s[2] = out & 0xffffffff; data.s[3] = out >> 32;
		for (unsigned long i = 3; i < seed_length; i += 2) {
			s = seed[i - 1] + ((unsigned long long)seed[i] << 32);
			out = splitmix64(&s); data.s[0] ^= out & 0xffffffff; data.s[1] ^= out >> 32;
			out = splitmix64(&s); data.s[2] ^= out & 0xffffffff; data.s[3] ^= out >> 32;
		}
		if (seed_length & 1) {
			s = seed[seed_length - 1];
			out = splitmix64(&s); data.s[0] ^= out & 0xffffffff; data.s[1] ^= out >> 32;
			out = splitmix64(&s); data.s[2] ^= out & 0xffffffff; data.s[3] ^= out >> 32;
		}
#endif
	}
	void Init_Data(SEED* seed) {
		data.s[0] = seed->s[0];
		data.s[1] = seed->s[1];
		data.s[2] = seed->s[2];
		data.s[3] = seed->s[3];
	}
	//データ出力
	SEED SaveData() { return data; }

	//乱数生成
	result_type Random() {
		const unsigned long result = rotl(data.s[0] + data.s[3], 7) + data.s[0];
		const unsigned long t = data.s[1] << 9;
		data.s[2] ^= data.s[0];
		data.s[3] ^= data.s[1];
		data.s[1] ^= data.s[2];
		data.s[0] ^= data.s[3];
		data.s[2] ^= t;
		data.s[3] = rotl(data.s[3], 11);
		return result;
	}
};


template <class SEED = XOSHIRO128>
class Xoshiro128ss {
	SEED data;
	inline unsigned long rotl(unsigned long x, unsigned long k) { return (x << k) | (x >> (32 - k)); }

#ifdef _32BIT_ONLY
	inline unsigned long long splitmix64_32(unsigned long* low, unsigned long* high) {
		unsigned long temp = ((((*low & 0xffff) + 0x7c15) >> 16) + (*low >> 16) + 0x7f4a) >> 16, z0l, z0h;
		unsigned long z[2];
		z[0] = (*low  +=  0x7f4a7c15);
		z[1] = (*high += (0x9e3779b9 + temp));

		z[0] ^= (z[0] >> 30) + (z[1] << 2); z[1] ^= z[1] >> 30;
		z0l = z[0] & 0xffff; z0h = z[0] >> 16;
		temp = ((((z0l * 0xe5b9) >> 16) + (z0l * 0x1ce4) + (z0h * 0xe5b9)) >> 16) + (z0h * 0x1ce4) + z[0] * 0xbf58476d + z[1] * 0x1ce4e5b9;
		z[0] = z[0] * 0x1ce4e5b9; z[1] = temp;

		z[0] ^= (z[0] >> 27) + (z[1] << 5); z[1] ^= z[1] >> 27;
		z0l = z[0] & 0xffff; z0h = z[0] >> 16;
		temp = ((((z0l * 0x11eb) >> 16) + (z0l * 0x1331) + (z0h * 0x11eb)) >> 16) + (z0h * 0x1331) + z[0] * 0x94d049bb + z[1] * 0x133111eb;
		z[0] = z[0] * 0x133111eb; z[1] = temp;

		z[0] ^= (z[0] >> 31) + (z[1] << 1); z[1] ^= z[1] >> 31;
		return z[0] + ((unsigned long long)z[1] << 32);
	}
#else
	inline unsigned long long splitmix64(unsigned long long* seed) {
		unsigned long long z = (*seed += 0x9e3779b97f4a7c15);
		z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
		z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
		return z ^ (z >> 31);
	}
#endif
public:
	using result_type = unsigned long;
	Xoshiro128ss() {}
	Xoshiro128ss(result_type seed) { Init_Int(seed); }
	Xoshiro128ss(unsigned long seed[], unsigned long seed_length) { Init_Array(seed, seed_length); }
	Xoshiro128ss(SEED* seed) { Init_Data(seed); }
	~Xoshiro128ss() {}

	//operator
	static constexpr result_type min() { return 0; }
	static constexpr result_type max() { return 0xffffffffUL; }
	result_type operator () () { return Random(); }

	void discard(uint64_t z) { for (uint64_t i = 0; i < z; ++i) Random(); }

	//seed値生成
	void Init_Int(unsigned long seed) {
		unsigned long long sp_out;
#ifdef _32BIT_ONLY
		unsigned long sp_seed32[2] = { seed, 0 };
		sp_out = splitmix64_32(&sp_seed32[0], &sp_seed32[1]);
		data.s[0] = sp_out & 0xffffffff; data.s[1] = sp_out >> 32;
		sp_out = splitmix64_32(&sp_seed32[0], &sp_seed32[1]);
		data.s[2] = sp_out & 0xffffffff; data.s[3] = sp_out >> 32;
#else
		unsigned long long sp_seed = seed;
		sp_out = splitmix64(&sp_seed);
		data.s[0] = sp_out & 0xffffffff; data.s[1] = sp_out >> 32;
		sp_out = splitmix64(&sp_seed);
		data.s[2] = sp_out & 0xffffffff; data.s[3] = sp_out >> 32;
#endif
	}
	void Init_Array(unsigned long seed[], unsigned long seed_length) {
		if (seed_length == 0) { return Init_Int(0); }
		else if (seed_length == 1) { return Init_Int(seed[0]); }
		unsigned long long out;
#ifdef _32BIT_ONLY
		unsigned long sl = seed[0], sh = seed[1];
		out = splitmix64_32(&sl, &sh); data.s[0] = out & 0xffffffff; data.s[1] = out >> 32;
		out = splitmix64_32(&sl, &sh); data.s[2] = out & 0xffffffff; data.s[3] = out >> 32;
		for (unsigned long i = 3; i < seed_length; i += 2) {
			sl = seed[i - 1], sh = seed[i];
			out = splitmix64_32(&sl, &sh); data.s[0] ^= out & 0xffffffff; data.s[1] ^= out >> 32;
			out = splitmix64_32(&sl, &sh); data.s[2] ^= out & 0xffffffff; data.s[3] ^= out >> 32;
		}
		if (seed_length & 1) {
			sl = seed[seed_length - 1], sh = 0;
			out = splitmix64_32(&sl, &sh); data.s[0] ^= out & 0xffffffff; data.s[1] ^= out >> 32;
			out = splitmix64_32(&sl, &sh); data.s[2] ^= out & 0xffffffff; data.s[3] ^= out >> 32;
		}
#else
		unsigned long long s = seed[0] + ((unsigned long long)seed[1] << 32);
		out = splitmix64(&s); data.s[0] = out & 0xffffffff; data.s[1] = out >> 32;
		out = splitmix64(&s); data.s[2] = out & 0xffffffff; data.s[3] = out >> 32;
		for (unsigned long i = 3; i < seed_length; i += 2) {
			s = seed[i - 1] + ((unsigned long long)seed[i] << 32);
			out = splitmix64(&s); data.s[0] ^= out & 0xffffffff; data.s[1] ^= out >> 32;
			out = splitmix64(&s); data.s[2] ^= out & 0xffffffff; data.s[3] ^= out >> 32;
		}
		if (seed_length & 1) {
			s = seed[seed_length - 1];
			out = splitmix64(&s); data.s[0] ^= out & 0xffffffff; data.s[1] ^= out >> 32;
			out = splitmix64(&s); data.s[2] ^= out & 0xffffffff; data.s[3] ^= out >> 32;
		}
#endif
	}
	void Init_Data(SEED* seed) {
		data.s[0] = seed->s[0];
		data.s[1] = seed->s[1];
		data.s[2] = seed->s[2];
		data.s[3] = seed->s[3];
	}
	//データ出力
	SEED SaveData() { return data; }

	//乱数生成
	result_type Random() {
		const unsigned long result = rotl(data.s[0] * 5, 7) * 9;
		const unsigned long t = data.s[1] << 9;
		data.s[2] ^= data.s[0];
		data.s[3] ^= data.s[1];
		data.s[1] ^= data.s[2];
		data.s[0] ^= data.s[3];
		data.s[2] ^= t;
		data.s[3] = rotl(data.s[3], 11);
		return result;
	}
};

template <class SEED = XOSHIRO128>
class Xoshiro128p {
	SEED data;
	inline unsigned long rotl(unsigned long x, unsigned long k) { return (x << k) | (x >> (32 - k)); }

#ifdef _32BIT_ONLY
	inline unsigned long long splitmix64_32(unsigned long* low, unsigned long* high) {
		unsigned long temp = ((((*low & 0xffff) + 0x7c15) >> 16) + (*low >> 16) + 0x7f4a) >> 16, z0l, z0h;
		unsigned long z[2];
		z[0] = (*low  +=  0x7f4a7c15);
		z[1] = (*high += (0x9e3779b9 + temp));

		z[0] ^= (z[0] >> 30) + (z[1] << 2); z[1] ^= z[1] >> 30;
		z0l = z[0] & 0xffff; z0h = z[0] >> 16;
		temp = ((((z0l * 0xe5b9) >> 16) + (z0l * 0x1ce4) + (z0h * 0xe5b9)) >> 16) + (z0h * 0x1ce4) + z[0] * 0xbf58476d + z[1] * 0x1ce4e5b9;
		z[0] = z[0] * 0x1ce4e5b9; z[1] = temp;

		z[0] ^= (z[0] >> 27) + (z[1] << 5); z[1] ^= z[1] >> 27;
		z0l = z[0] & 0xffff; z0h = z[0] >> 16;
		temp = ((((z0l * 0x11eb) >> 16) + (z0l * 0x1331) + (z0h * 0x11eb)) >> 16) + (z0h * 0x1331) + z[0] * 0x94d049bb + z[1] * 0x133111eb;
		z[0] = z[0] * 0x133111eb; z[1] = temp;

		z[0] ^= (z[0] >> 31) + (z[1] << 1); z[1] ^= z[1] >> 31;
		return z[0] + ((unsigned long long)z[1] << 32);
	}
#else
	inline unsigned long long splitmix64(unsigned long long* seed) {
		unsigned long long z = (*seed += 0x9e3779b97f4a7c15);
		z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
		z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
		return z ^ (z >> 31);
	}
#endif
public:
	using result_type = unsigned long;
	Xoshiro128p() {}
	Xoshiro128p(result_type seed) { Init_Int(seed); }
	Xoshiro128p(unsigned long seed[], unsigned long seed_length) { Init_Array(seed, seed_length); }
	Xoshiro128p(SEED* seed) { Init_Data(seed); }
	~Xoshiro128p() {}

	//operator
	static constexpr result_type min() { return 0; }
	static constexpr result_type max() { return 0xffffffffUL; }
	result_type operator () () { return Random(); }

	void discard(uint64_t z) { for (uint64_t i = 0; i < z; ++i) Random(); }

	//seed値生成
	void Init_Int(unsigned long seed) {
		unsigned long long sp_out;
#ifdef _32BIT_ONLY
		unsigned long sp_seed32[2] = { seed, 0 };
		sp_out = splitmix64_32(&sp_seed32[0], &sp_seed32[1]);
		data.s[0] = sp_out & 0xffffffff; data.s[1] = sp_out >> 32;
		sp_out = splitmix64_32(&sp_seed32[0], &sp_seed32[1]);
		data.s[2] = sp_out & 0xffffffff; data.s[3] = sp_out >> 32;
#else
		unsigned long long sp_seed = seed;
		sp_out = splitmix64(&sp_seed);
		data.s[0] = sp_out & 0xffffffff; data.s[1] = sp_out >> 32;
		sp_out = splitmix64(&sp_seed);
		data.s[2] = sp_out & 0xffffffff; data.s[3] = sp_out >> 32;
#endif
	}
	void Init_Array(unsigned long seed[], unsigned long seed_length) {
		if (seed_length == 0) { return Init_Int(0); }
		else if (seed_length == 1) { return Init_Int(seed[0]); }
		unsigned long long out;
#ifdef _32BIT_ONLY
		unsigned long sl = seed[0], sh = seed[1];
		out = splitmix64_32(&sl, &sh); data.s[0] = out & 0xffffffff; data.s[1] = out >> 32;
		out = splitmix64_32(&sl, &sh); data.s[2] = out & 0xffffffff; data.s[3] = out >> 32;
		for (unsigned long i = 3; i < seed_length; i += 2) {
			sl = seed[i - 1], sh = seed[i];
			out = splitmix64_32(&sl, &sh); data.s[0] ^= out & 0xffffffff; data.s[1] ^= out >> 32;
			out = splitmix64_32(&sl, &sh); data.s[2] ^= out & 0xffffffff; data.s[3] ^= out >> 32;
		}
		if (seed_length & 1) {
			sl = seed[seed_length - 1], sh = 0;
			out = splitmix64_32(&sl, &sh); data.s[0] ^= out & 0xffffffff; data.s[1] ^= out >> 32;
			out = splitmix64_32(&sl, &sh); data.s[2] ^= out & 0xffffffff; data.s[3] ^= out >> 32;
		}
#else
		unsigned long long s = seed[0] + ((unsigned long long)seed[1] << 32);
		out = splitmix64(&s); data.s[0] = out & 0xffffffff; data.s[1] = out >> 32;
		out = splitmix64(&s); data.s[2] = out & 0xffffffff; data.s[3] = out >> 32;
		for (unsigned long i = 3; i < seed_length; i += 2) {
			s = seed[i - 1] + ((unsigned long long)seed[i] << 32);
			out = splitmix64(&s); data.s[0] ^= out & 0xffffffff; data.s[1] ^= out >> 32;
			out = splitmix64(&s); data.s[2] ^= out & 0xffffffff; data.s[3] ^= out >> 32;
		}
		if (seed_length & 1) {
			s = seed[seed_length - 1];
			out = splitmix64(&s); data.s[0] ^= out & 0xffffffff; data.s[1] ^= out >> 32;
			out = splitmix64(&s); data.s[2] ^= out & 0xffffffff; data.s[3] ^= out >> 32;
		}
#endif
	}
	void Init_Data(SEED* seed) {
		data.s[0] = seed->s[0];
		data.s[1] = seed->s[1];
		data.s[2] = seed->s[2];
		data.s[3] = seed->s[3];
	}
	//データ出力
	SEED SaveData() { return data; }

	//乱数生成
	result_type Random() {
		const unsigned long result = data.s[0] + data.s[3];
		const unsigned long t = data.s[1] << 9;
		data.s[2] ^= data.s[0];
		data.s[3] ^= data.s[1];
		data.s[1] ^= data.s[2];
		data.s[0] ^= data.s[3];
		data.s[2] ^= t;
		data.s[3] = rotl(data.s[3], 11);
		return result;
	}
};
#endif /* _XOSHIRO_HPP */

