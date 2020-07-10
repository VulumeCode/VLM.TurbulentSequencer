
class Random {
    _normal: null | number = null;
    
    next() {
        return 0 | 255 * this.uniform();
    }
    random() {
        return this.uniform();
    }
    uniform() {
        return Math.random();
    }
    normal() {
        if (null != this._normal) {
            var b = this._normal;
            this._normal = null;
            return b;
        }
        b = this.uniform() || Math.pow(2, -53);
        b = Math.sqrt(-2 * Math.log(b));
        var c = 2 * Math.PI * this.uniform();
        this._normal = b * Math.sin(c);
        return b * Math.cos(c);
    }
    range(b: number, c: number) {
        if (null == b) {
            return this.uniform();
        }
        null == c && (c = b, b = 0);
        return b + Math.floor(this.uniform() * (c - b));
    }
    exp() {
        return -Math.log(this.uniform() || Math.pow(2, -53));
    }
    poisson(b: number) {
        b = Math.exp(-(b || 1));
        for (var c = 0, e = 1; e > b;) {
            e *= this.uniform(), c++;
        }
        return c - 1;
    }
    gamma(b: number) {
        var c;
        var e = (1 > b ? 1 + b : b) - 1 / 3;
        for (c = 1 / Math.sqrt(9 * e); ;) {
            for (; ;) {
                var a = this.normal();
                var d = Math.pow(1 + c * a, 3);
                if (0 < d) {
                    break;
                }
            }
            var f = this.uniform();
            if (f >= 1 - 0.331 * Math.pow(a, 4) && Math.log(f) >= 0.5 * Math.pow(a, 2) + e * (1 - d + Math.log(d))) {
                break;
            }
        }
        return 1 < b ? e * d : e * d * Math.exp(this.exp() / -b);
    }
};

class MT extends Random {
    _index: number
    _state: number[]
    _seed: number

    constructor(a: number) {
        super()
        this._index = 0;
        this._state = Array(624);
        this._state[0] = null != a ? a : 4294967295 * Math.random() | 0;
        this._seed = this._state[0];
        var d = this._state;
        for (a = 1; 624 > a; a++) {
            d[a] = d[a - 1] ^ d[a - 1] >>> 30, d[a] = 1812433253 * d[a] + a, d[a] &= (d[a] << 32) - 1;
        }
    }

    _generateNumbers() {
        var a, d = this._state;
        for (a = 0; 624 > a; a++) {
            var f = d[a] & 2147483648;
            f += d[(a + 1) % 624] & 2147483647;
            d[a] = d[(a + 397) % 624] ^ f >>> 1;
            0 !== f % 2 && (d[a] ^= 2567483615);
        }
    };
    uniform() {
        0 === this._index && this._generateNumbers();
        var a = this._state[this._index];
        a ^= a >>> 11;
        a ^= a << 7 & 2636928640;
        a ^= a << 15 & 4022730752;
        this._index = (this._index + 1) % 624;
        return 1.0 / 4294967296.0 * ((a ^ a >>> 18) >>> 0);
    };
}

export { Random, MT}