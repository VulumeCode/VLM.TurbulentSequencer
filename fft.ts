/* Copyright (c) 2012, Jens Nockert <jens@ofmlabs.org>, Jussi Kalliokoski <jussi@ofmlabs.org>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


function butterfly2(output: number[] | Float64Array, outputOffset: number, outputStride: number, fStride: number, state: State, m: number) {
    var t = state.twiddle
    
    for (var i = 0; i < m; i++) {
        var s0_r = output[2 * ((outputOffset) + (outputStride) * (i))], s0_i = output[2 * ((outputOffset) + (outputStride) * (i)) + 1]
        var s1_r = output[2 * ((outputOffset) + (outputStride) * (i + m))], s1_i = output[2 * ((outputOffset) + (outputStride) * (i + m)) + 1]
        
        var t1_r = t[2 * ((0) + (fStride) * (i))], t1_i = t[2 * ((0) + (fStride) * (i)) + 1]
        
        var v1_r = s1_r * t1_r - s1_i * t1_i, v1_i = s1_r * t1_i + s1_i * t1_r
        
        var r0_r = s0_r + v1_r, r0_i = s0_i + v1_i
        var r1_r = s0_r - v1_r, r1_i = s0_i - v1_i
        
        output[2 * ((outputOffset) + (outputStride) * (i))] = r0_r, output[2 * ((outputOffset) + (outputStride) * (i)) + 1] = r0_i
        output[2 * ((outputOffset) + (outputStride) * (i + m))] = r1_r, output[2 * ((outputOffset) + (outputStride) * (i + m)) + 1] = r1_i
    }
}

function butterfly3(output: number[] | Float64Array, outputOffset: number, outputStride: number, fStride: number, state: State, m: number) {
    var t = state.twiddle
    var m1 = m, m2 = 2 * m
    var fStride1 = fStride, fStride2 = 2 * fStride
    
    var e = t[2 * ((0) + (fStride) * (m)) + 1]
    
    for (var i = 0; i < m; i++) {
        var s0_r = output[2 * ((outputOffset) + (outputStride) * (i))], s0_i = output[2 * ((outputOffset) + (outputStride) * (i)) + 1]
        
        var s1_r = output[2 * ((outputOffset) + (outputStride) * (i + m1))], s1_i = output[2 * ((outputOffset) + (outputStride) * (i + m1)) + 1]
        var t1_r = t[2 * ((0) + (fStride1) * (i))], t1_i = t[2 * ((0) + (fStride1) * (i)) + 1]
        var v1_r = s1_r * t1_r - s1_i * t1_i, v1_i = s1_r * t1_i + s1_i * t1_r
        
        var s2_r = output[2 * ((outputOffset) + (outputStride) * (i + m2))], s2_i = output[2 * ((outputOffset) + (outputStride) * (i + m2)) + 1]
        var t2_r = t[2 * ((0) + (fStride2) * (i))], t2_i = t[2 * ((0) + (fStride2) * (i)) + 1]
        var v2_r = s2_r * t2_r - s2_i * t2_i, v2_i = s2_r * t2_i + s2_i * t2_r
        
        var i0_r = v1_r + v2_r, i0_i = v1_i + v2_i
        
        var r0_r = s0_r + i0_r, r0_i = s0_i + i0_i
        output[2 * ((outputOffset) + (outputStride) * (i))] = r0_r, output[2 * ((outputOffset) + (outputStride) * (i)) + 1] = r0_i
        
        var i1_r = s0_r - i0_r * 0.5
        var i1_i = s0_i - i0_i * 0.5
        
        var i2_r = (v1_r - v2_r) * e
        var i2_i = (v1_i - v2_i) * e
        
        var r1_r = i1_r - i2_i
        var r1_i = i1_i + i2_r
        output[2 * ((outputOffset) + (outputStride) * (i + m1))] = r1_r, output[2 * ((outputOffset) + (outputStride) * (i + m1)) + 1] = r1_i
        
        var r2_r = i1_r + i2_i
        var r2_i = i1_i - i2_r
        output[2 * ((outputOffset) + (outputStride) * (i + m2))] = r2_r, output[2 * ((outputOffset) + (outputStride) * (i + m2)) + 1] = r2_i
    }
}

function butterfly4(output: number[] | Float64Array, outputOffset: number, outputStride: number, fStride: number, state: State, m: number) {
    let t = state.twiddle
    let m1 = m, m2 = 2 * m, m3 = 3 * m
    let fStride1 = fStride, fStride2 = 2 * fStride, fStride3 = 3 * fStride
    
    for (let i = 0; i < m; i++) {
        let s0_r = output[2 * ((outputOffset) + (outputStride) * (i))], s0_i = output[2 * ((outputOffset) + (outputStride) * (i)) + 1]
        
        let s1_r = output[2 * ((outputOffset) + (outputStride) * (i + m1))], s1_i = output[2 * ((outputOffset) + (outputStride) * (i + m1)) + 1]
        let t1_r = t[2 * ((0) + (fStride1) * (i))], t1_i = t[2 * ((0) + (fStride1) * (i)) + 1]
        let v1_r = s1_r * t1_r - s1_i * t1_i, v1_i = s1_r * t1_i + s1_i * t1_r
        
        let s2_r = output[2 * ((outputOffset) + (outputStride) * (i + m2))], s2_i = output[2 * ((outputOffset) + (outputStride) * (i + m2)) + 1]
        let t2_r = t[2 * ((0) + (fStride2) * (i))], t2_i = t[2 * ((0) + (fStride2) * (i)) + 1]
        let v2_r = s2_r * t2_r - s2_i * t2_i, v2_i = s2_r * t2_i + s2_i * t2_r
        
        let s3_r = output[2 * ((outputOffset) + (outputStride) * (i + m3))], s3_i = output[2 * ((outputOffset) + (outputStride) * (i + m3)) + 1]
        let t3_r = t[2 * ((0) + (fStride3) * (i))], t3_i = t[2 * ((0) + (fStride3) * (i)) + 1]
        let v3_r = s3_r * t3_r - s3_i * t3_i, v3_i = s3_r * t3_i + s3_i * t3_r
        
        let i0_r = s0_r + v2_r, i0_i = s0_i + v2_i
        let i1_r = s0_r - v2_r, i1_i = s0_i - v2_i
        let i2_r = v1_r + v3_r, i2_i = v1_i + v3_i
        let i3_r = v1_r - v3_r, i3_i = v1_i - v3_i
        
        let r0_r = i0_r + i2_r, r0_i = i0_i + i2_i
        
        let r1_r: number;
        let r1_i: number;

        if (state.inverse) {
            r1_r = i1_r - i3_i
            r1_i = i1_i + i3_r
        } else {
            r1_r = i1_r + i3_i
            r1_i = i1_i - i3_r
        }
        
        let r2_r = i0_r - i2_r, r2_i = i0_i - i2_i
        
        let r3_r: number;
        let r3_i: number;

        if (state.inverse) {
            r3_r = i1_r + i3_i
            r3_i = i1_i - i3_r
        } else {
            r3_r = i1_r - i3_i
            r3_i = i1_i + i3_r
        }
        
        output[2 * ((outputOffset) + (outputStride) * (i))] = r0_r, output[2 * ((outputOffset) + (outputStride) * (i)) + 1] = r0_i
        output[2 * ((outputOffset) + (outputStride) * (i + m1))] = r1_r, output[2 * ((outputOffset) + (outputStride) * (i + m1)) + 1] = r1_i
        output[2 * ((outputOffset) + (outputStride) * (i + m2))] = r2_r, output[2 * ((outputOffset) + (outputStride) * (i + m2)) + 1] = r2_i
        output[2 * ((outputOffset) + (outputStride) * (i + m3))] = r3_r, output[2 * ((outputOffset) + (outputStride) * (i + m3)) + 1] = r3_i
    }
}

function butterfly(output: number[] | Float64Array, outputOffset: number, outputStride: number, fStride: number, state: State, m: number, p: number) {
    let t = state.twiddle, n = state.n, scratch = new Float64Array(2 * p)
    
    for (let u = 0; u < m; u++) {
        for (let q1 = 0, k = u; q1 < p; q1++, k += m) {
            let x0_r = output[2 * ((outputOffset) + (outputStride) * (k))], x0_i = output[2 * ((outputOffset) + (outputStride) * (k)) + 1]
            scratch[2 * (q1)] = x0_r, scratch[2 * (q1) + 1] = x0_i
        }
        
        for (let q1 = 0, k = u; q1 < p; q1++, k += m) {
            let tOffset = 0
            
            let x0_r = scratch[2 * (0)], x0_i = scratch[2 * (0) + 1]
            output[2 * ((outputOffset) + (outputStride) * (k))] = x0_r, output[2 * ((outputOffset) + (outputStride) * (k)) + 1] = x0_i
            
            for (let q = 1; q < p; q++) {
                tOffset = (tOffset + fStride * k) % n
                
                let s0_r = output[2 * ((outputOffset) + (outputStride) * (k))], s0_i = output[2 * ((outputOffset) + (outputStride) * (k)) + 1]
                
                let s1_r = scratch[2 * (q)], s1_i = scratch[2 * (q) + 1]
                let t1_r = t[2 * (tOffset)], t1_i = t[2 * (tOffset) + 1]
                let v1_r = s1_r * t1_r - s1_i * t1_i, v1_i = s1_r * t1_i + s1_i * t1_r
                
                let r0_r = s0_r + v1_r, r0_i = s0_i + v1_i
                output[2 * ((outputOffset) + (outputStride) * (k))] = r0_r, output[2 * ((outputOffset) + (outputStride) * (k)) + 1] = r0_i
            }
        }
    }
}

function work(output: number[] | Float64Array, outputOffset: number, outputStride: number, f: number[] | Float64Array, fOffset: number, fStride: number, inputStride: number, factors: number[], state: State) {
    let p = factors.shift()!
    let m = factors.shift()!
    
    if (m == 1) {
        for (let i = 0; i < p * m; i++) {
            let x0_r = f[2 * ((fOffset) + (fStride * inputStride) * (i))], x0_i = f[2 * ((fOffset) + (fStride * inputStride) * (i)) + 1]
            output[2 * ((outputOffset) + (outputStride) * (i))] = x0_r, output[2 * ((outputOffset) + (outputStride) * (i)) + 1] = x0_i
        }
    } else {
        for (let i = 0; i < p; i++) {
            work(output, outputOffset + outputStride * i * m, outputStride, f, fOffset + i * fStride * inputStride, fStride * p, inputStride, factors.slice(), state)
        }
    }
    
    switch (p) {
        case 2: butterfly2(output, outputOffset, outputStride, fStride, state, m); break
        case 3: butterfly3(output, outputOffset, outputStride, fStride, state, m); break
        case 4: butterfly4(output, outputOffset, outputStride, fStride, state, m); break
        default: butterfly(output, outputOffset, outputStride, fStride, state, m, p); break
    }
}

interface State {
    n: number,
    inverse: boolean,
    factors: number[] ,
    twiddle: Float64Array,
    scratch: Float64Array
}

class complex {

    state: State;

    constructor(n: number, inverse?: boolean) {
        if (arguments.length < 2) {
            throw new RangeError("You didn't pass enough arguments, passed `" + arguments.length + "'")
        }
        
        n = ~~n;
        inverse = !!inverse;
        
        if (n < 1) {
            throw new RangeError("n is outside range, should be positive integer, was `" + n + "'")
        }
        
        let state: State = {
            n: n,
            inverse: inverse,
            
            factors: [],
            twiddle: new Float64Array(2 * n),
            scratch: new Float64Array(2 * n)
        }
        
        let t = state.twiddle, theta = 2 * Math.PI / n
        
        for (let i = 0; i < n; i++) {
            let phase: number;
            if (inverse) {
                phase =  theta * i
            } else {
                phase = -theta * i
            }
            
            t[2 * (i)] = Math.cos(phase)
            t[2 * (i) + 1] = Math.sin(phase)
        }
        
        let p = 4, v = Math.floor(Math.sqrt(n))
        
        while (n > 1) {
            while (n % p) {
                switch (p) {
                    case 4: p = 2; break
                    case 2: p = 3; break
                    default: p += 2; break
                }
                
                if (p > v) {
                    p = n
                }
            }
            
            n /= p
            
            state.factors.push(p)
            state.factors.push(n)
        }
        this.state = state
    }


    simple (output: number[], input: number[], t: string): void {
        this.process(output, 0, 1, input, 0, 1, t)
    }

    process(output: number[], outputOffset: number, outputStride: number, input: number[], inputOffset: number, inputStride: number, t: string): void {
        outputStride = ~~outputStride, inputStride = ~~inputStride
        
        let type = t == 'real' ? t : 'complex'
        
        if (outputStride < 1) {
            throw new RangeError("outputStride is outside range, should be positive integer, was `" + outputStride + "'")
        }
        
        if (inputStride < 1) {
            throw new RangeError("inputStride is outside range, should be positive integer, was `" + inputStride + "'")
        }
        
        if (type == 'real') {
            for (let i = 0; i < this.state.n; i++) {
                let x0_r = input[inputOffset + inputStride * i]
                let x0_i = 0.0
                
                this.state.scratch[2 * (i)] = x0_r, this.state.scratch[2 * (i) + 1] = x0_i
            }
            
            work(output, outputOffset, outputStride, this.state.scratch, 0, 1, 1, this.state.factors.slice(), this.state)
        } else {
            if (input == output) {
                work(this.state.scratch, 0, 1, input, inputOffset, 1, inputStride, this.state.factors.slice(), this.state)
                
                for (let i = 0; i < this.state.n; i++) {
                    let x0_r = this.state.scratch[2 * (i)]
                    let x0_i = this.state.scratch[2 * (i) + 1]
                    
                    output[2 * ((outputOffset) + (outputStride) * (i))] = x0_r, output[2 * ((outputOffset) + (outputStride) * (i)) + 1] = x0_i
                }
            } else {
                work(output, outputOffset, outputStride, input, inputOffset, 1, inputStride, this.state.factors.slice(), this.state)
            }
        }
    }
}

export { complex }