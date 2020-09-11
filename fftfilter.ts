import * as FFT from './fft';
import { postError } from './jsCommon';

inlets = 3;
outlets = 4;

var seqReal: number[] = [0, 0];
var seqImg: number[] = [0, 0];
var curve: number[] = [0, 0];

let fft: FFT.complex;
let ffti: FFT.complex;
let prevLen = -1;

function list() {
    try {
        switch (inlet) {
            case 0:
                seqReal = arrayfromargs(arguments);
                bang();
                break;
            case 1:
                seqImg = arrayfromargs(arguments);
                break;
            case 2:
                curve = arrayfromargs(arguments);
                bang();
                break;
        }
    } catch (e) {
        postError(e)
    }
}

function bang() {
    const len = seqReal.length;
    if (len === 1) {
        outlet(3, seqImg);
        outlet(2, seqReal);
        outlet(1, seqImg);
        outlet(0, seqReal);
    }
    else
        if (len > 1) {
            if (len != prevLen) {
                fft = new FFT.complex(len, false);
                ffti = new FFT.complex(len, true);
                prevLen = len;
            }

            let inputTime = [];
            for (let i = 0; i < len; i++) {
                inputTime.push(seqReal[i])
                inputTime.push(seqImg[i])
            }
            let inputFreq: number[] = [];

            fft.simple(inputFreq, inputTime, 'complex');

            let outSeqFreqImg = [];
            let outSeqFreqReal = [];

            let outputFreq = [];
            let v;
            for (let i = 0; i < len; i++) {
                v = inputFreq.shift()! * curve[i];
                outSeqFreqReal.push(v / len);
                outputFreq.push(v);
                v = inputFreq.shift()! * curve[i];
                outSeqFreqImg.push(v / len);
                outputFreq.push(v);
            }

            let outputTime: number[] = [];
            ffti.simple(outputTime, outputFreq, 'complex');


            let outSeqReal: number[] = [];
            let outSeqImg: number[] = [];
            for (let i = 0; i < len; i++) {
                outSeqReal.push(outputTime.shift()! / len);
                outSeqImg.push(outputTime.shift()! / len);
            }

            outlet(3, outSeqFreqImg);
            outlet(2, outSeqFreqReal);
            outlet(1, outSeqImg);
            outlet(0, outSeqReal);
        }
}
let module = {};
export = {};