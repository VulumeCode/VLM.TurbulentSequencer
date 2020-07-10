import * as FFT from './fft';

inlets = 3;
outlets = 4;

var seqReal: number[] = [];
var seqImg: number[] = [];
var curve: number[] = [];

function list()
{
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
}

function bang()
{
    const len = seqReal.length;
    if (len > 0) {
        let fft = new FFT.complex(len, false);
        let ffti = new FFT.complex(len, true);

        let inputTime = [];
        for(let i = 0; i < len; i++){
            inputTime.push(seqReal[i])
            inputTime.push(seqImg[i])
        }    
        let inputFreq: number[] = [];

        fft.simple(inputFreq, inputTime, 'complex');

        let outSeqFreqImg = [];
        let outSeqFreqReal = [];

        let outputFreq = [];
        let v;
        for(let i = 0; i < len; i++){
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
        for(let i = 0; i < len; i++){
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