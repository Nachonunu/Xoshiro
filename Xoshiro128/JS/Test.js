window.onload = function() {
	var ta = document.getElementById("TestArea");
	var seed = [1,2,3,4,5];
	var xoshiro = new Xoshiro(seed);
	for (var i = 0; i < 100; ++i) ta.value += xoshiro.GenUint32() + '\n';
	ta.value += "seed " + xoshiro.s[0] + '\n';
	ta.value += "seed " + xoshiro.s[1] + '\n';
	ta.value += "seed " + xoshiro.s[2] + '\n';
	ta.value += "seed " + xoshiro.s[3] + '\n';
}
