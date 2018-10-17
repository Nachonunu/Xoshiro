/*@license Xoshiro128 JS v0.1 | CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/deed) */
function Xoshiro(seed) {
	this.imul = (typeof Math.imul == "function") ? Math.imul : function(a, b) { return ((a * (b >>> 16)) << 16) + (a * (b & 0xffff)); };
	this.s = new Uint32Array([123456789, 362436069, 521288629, 88675123]);
	if (arguments.length >= 1) {
		if (typeof seed === "number") {
			if (seed >= 0) this.Init_Int(seed);
			else this.Init_int( Math.floor(0xffffffff * Math.random()) );
		} else if (typeof seed === "object") {
			if (seed instanceof Array === true) this.Init_Array(seed);
			else this.Init_int( Math.floor(0xffffffff * Math.random()) );
		}
	}
}

Xoshiro.prototype.splitmix64 = function(seed) {
	var temp = ((((seed[0] & 0xffff) + 0x7c15) >>> 16) + (seed[0] >>> 16) + 0x7f4a) >>> 16, z0l, z0h;
	var z = new Uint32Array(2);
	z[0] = (seed[0] +=  0x7f4a7c15);
	z[1] = (seed[1] += (0x9e3779b9 + temp));

	z[0] ^= (z[0] >>> 30) + (z[1] << 2); z[1] ^= z[1] >>> 30;
	z0l = z[0] & 0xffff; z0h = z[0] >>> 16;
	temp = ((((z0l * 0xe5b9) >>> 16) + (z0l * 0x1ce4) + (z0h * 0xe5b9)) >>> 16) + (z0h * 0x1ce4) + this.imul(z[0], 0xbf58476d) + this.imul(z[1], 0x1ce4e5b9);
	z[0] = this.imul(z[0], 0x1ce4e5b9); z[1] = temp;

	z[0] ^= (z[0] >>> 27) + (z[1] << 5); z[1] ^= z[1] >>> 27;
	z0l = z[0] & 0xffff; z0h = z[0] >>> 16;
	temp = ((((z0l * 0x11eb) >>> 16) + (z0l * 0x1331) + (z0h * 0x11eb)) >>> 16) + (z0h * 0x1331) + this.imul(z[0], 0x94d049bb) + this.imul(z[1], 0x133111eb);
	z[0] = this.imul(z[0], 0x133111eb); z[1] = temp;

	z[0] ^= (z[0] >>> 31) + (z[1] << 1); z[1] ^= z[1] >>> 31;
	return z;
}
Xoshiro.prototype.Init_Int = function(seed) {
	var sp = new Uint32Array([seed, 0]);
	var out = this.splitmix64(sp);
	this.s[0] = out[0]; this.s[1] = out[1];
	out = this.splitmix64(sp);
	this.s[2] = out[0]; this.s[3] = out[1];
}
Xoshiro.prototype.Init_Array = function(seed) {
	if (seed.length <= 0) { return this.Init_Int(0); }
	else if (seed.length == 1) { return this.Init_Int(seed[0]); }
	var out;
	var sp = new Uint32Array([seed[0], seed[1]]);
	out = this.splitmix64(sp); this.s[0] = out[0]; this.s[1] = out[1];
	out = this.splitmix64(sp); this.s[2] = out[0]; this.s[3] = out[1];
	for (var i = 3; i < seed.length; i += 2) {
		sp[0] = seed[i - 1], sp[1] = seed[i];
		out = this.splitmix64(sp); this.s[0] ^= out[0]; this.s[1] ^= out[1];
		out = this.splitmix64(sp); this.s[2] ^= out[0]; this.s[3] ^= out[1];
	}
	if (seed.length & 1) {
		sp[0] = seed[seed.length - 1], sp[1] = 0;
		out = this.splitmix64(sp); this.s[0] ^= out[0]; this.s[1] ^= out[1];
		out = this.splitmix64(sp); this.s[2] ^= out[0]; this.s[3] ^= out[1];
	}
}
Xoshiro.prototype.GenUint32 = function() {
	var s0 = this.s[0], s1 = this.s[1], s2 = this.s[2], s3 = this.s[3];
	var rotl = function(x, k) { return (x << k) | (x >>> (32 - k)); };

	var result_starstar = rotl(s0 * 5, 7) * 9;
	var t = s1 << 9;
	s2 = s2 ^ s0;
	s3 = s3 ^ s1;
	s1 = s1 ^ s2;
	s0 = s0 ^ s3;
	s2 = s2 ^ t;
	s3 = rotl(s3, 11);

	this.s[0] = s0; this.s[1] = s1; this.s[2] = s2; this.s[3] = s3;
	return result_starstar >>> 0;
}
Xoshiro.prototype.GenReal = function() { return this.GenUint32() * (1.0 / 4294967296.0); } //[0,1)
Xoshiro.prototype.GenFree = function(num) { return num > 1 ? Math.floor(this.GenReal() * num) : 0; } //[0, num)

