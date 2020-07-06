inlets = 2;
outlets = 2;

var len = 16;
var seed = 0;

function Random() {
	this._normal = null
}

Random.prototype = {

	constructor: Random,

	get seed() {
		return this._seed
	},

	/**
	 * Sets the seed, resets the state
	 * @param  {Number} value
	 * @return {Number} value
	 */
	set seed(value) {
		this.constructor.call(this, value)
	},

	/**
	 * Get next random byte [0,255]
	 * @return {Number}
	 */
	next: function () {
		return 0 | (this.uniform() * 255)
	},

	/**
	 * Same as #uniform(), just to be
	 * compatible with the Math.random() style API
	 * @return {Number}
	 */
	random: function () {
		return this.uniform()
	},

	/**
	 * Get uniform random number between 0 and 1
	 * @return {Number}
	 */
	uniform: function () {
		return Math.random()
	},

	/**
	 * Get normally distributed number,
	 * with a mean 0, variance 1
	 * @return {Number}
	 */
	normal: function () {

		var x, y

		if (this._normal != null) {
			var num = this._normal
			this._normal = null
			return num
		}

		x = this.uniform() || Math.pow(2, -53)
		x = Math.sqrt(-2 * Math.log(x))
		y = 2 * Math.PI * this.uniform()

		this._normal = x * Math.sin(y)
		return x * Math.cos(y)

	},

	/**
	 * Get random integer in range [min,max]
	 * @param  {Number} min
	 * @param  {Number} max
	 * @return {Number} 
	 */
	range: function (min, max) {

		if (min == null) {
			return this.uniform()
		} else if (max == null) {
			max = min
			min = 0
		}

		return min + Math.floor(this.uniform() * (max - min))

	},

	/**
	 * Get exponentionally distributed
	 * number with lambda 1
	 * @return {Number}
	 */
	exp: function () {
		return -Math.log(
			this.uniform() || Math.pow(2, -53)
		)
	},

	/**
	 * Get poisson distributed number,
	 * the mean defaulting to 1
	 * @param  {Number} mean
	 * @return {Number} 
	 */
	poisson: function (mean) {

		var L = Math.exp(-(mean || 1))
		var k = 0, p = 1

		while (p > L) {
			p = p * this.uniform()
			k++
		}

		return k - 1

	},

	/**
	 * Get gamma distributed number,
	 * using uniform, normal and exp
	 * with the Marsaglia-Tsang method
	 * @param  {Number} a gamma
	 * @return {Number} 
	 */
	gamma: function (a) {

		var d, c, x, u, v

		d = (a < 1 ? 1 + a : a) - 1 / 3
		c = 1 / Math.sqrt(9 * d)

		while (true) {
			while (true) {
				x = this.normal()
				v = Math.pow(1 + c * x, 3)
				if (v > 0) break
			}
			u = this.uniform()
			if (u >= 1 - 0.331 * Math.pow(x, 4)) {
				if (Math.log(u) >= 0.5 * Math.pow(x, 2) + d * (1 - v + Math.log(v))) {
					break
				}
			}
		}

		return a > 1 ? d * v :
			d * v * Math.exp(this.exp() / -a)

	}

}

	(function () {

		var Random

		if (typeof module !== 'undefined') {
			module.exports = MersenneTwister
			if (typeof require === 'function') {
				var Random = require('../')
			}
		} else {
			Random = this.Random
			Random.MT = MersenneTwister
		}

		/**
		 * Mersenne Twister PRNG constructor
		 * @param {Number} seed
		 */
		function MersenneTwister(seed) {

			Random.call(this)

			this._index = 0
			this._state = new Array(624)
			this._state[0] = seed != null ?
				seed : (Math.random() * 0xFFFFFFFF) | 0
			this._seed = this._state[0]

			var i, MT = this._state
			for (i = 1; i < 624; i++) {
				MT[i] = MT[i - 1] ^ (MT[i - 1] >>> 30)
				MT[i] = 0x6C078965 * MT[i] + i // 1812433253
				MT[i] = MT[i] & ((MT[i] << 32) - 1)
			}

		}

		// Inherit from Random
		var $ = MersenneTwister.prototype =
			Object.create(Random.prototype)

		/**
		 * Prototype's constructor
		 * @type {Function}
		 */
		$.constructor = MersenneTwister

		/**
		 * Generate an array of 624 untempered numbers
		 * @return {Undefined}
		 */
		$._generateNumbers = function () {
			var i, y, MT = this._state
			for (i = 0; i < 624; i++) {
				// Bit 31 (32nd bit) of MT[i]
				y = (MT[i] & 0x80000000)
				// Bits 0-30 (first 31 bits) of MT[...]
				y = y + (MT[(i + 1) % 624] & 0x7FFFFFFF)
				// The new randomness
				MT[i] = MT[(i + 397) % 624] ^ (y >>> 1)
				// In case y is odd
				if ((y % 2) !== 0) {
					MT[i] = MT[i] ^ 0x9908B0DF // 2567483615
				}
			}
		}

		/**
		 * Extract a tempered pseudorandom number [0,1]
		 * based on the index-th value, calling
		 * #_generateNumbers() every 624 numbers
		 * @return {Number}
		 */
		$.uniform = function () {

			if (this._index === 0)
				this._generateNumbers()

			var y = this._state[this._index]

			y = y ^ (y >>> 11)
			y = y ^ ((y << 7) & 0x9D2C5680) // 2636928640
			y = y ^ ((y << 15) & 0xEFC60000) // 4022730752
			y = y ^ (y >>> 18)

			this._index = (this._index + 1) % 624

			return (y >>> 0) * (1.0 / 4294967296.0)

		}

	})()


function msg_int(a) {
	switch (inlet) {
		case 0:
			len = a;
			break;
		case 1:
			seed = a;
			break;
	}
	bang();
}

function bang() {
	as = [];
	bs = [];
	rng = new Random.MT(seed);
	var a, b;
	for (i = 0; i < len; i++) {
		do {
			a = (mt.uniform() - 0.5) * 2;
			b = (mt.uniform() - 0.5) * 2;
			tries++;
		} while (Math.sqrt((a * a) + (b * b)) > 1);
		as.push(a);
		bs.push(b);
	}

	outlet(1, as);
	outlet(0, bs);
}