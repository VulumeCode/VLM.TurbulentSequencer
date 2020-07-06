inlets = 3;
outlets = 4;

FFT = require("fft.js");

var seqReal = [];
var seqImg = [];
var curve = [];

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
    var len = seqReal.length;
    if (len > 0) {
        var fft = new FFT.complex(len, false);
        var ffti = new FFT.complex(len, true);

        var inputTime = [];
        for(var i = 0; i < len; i++){
            inputTime.push(seqReal[i])
            inputTime.push(seqImg[i])
        }    
        var inputFreq = [];

        fft.simple(inputFreq, inputTime, 'complex');

        var outSeqFreqImg = [];
        var outSeqFreqReal = [];

        var outputFreq = [];
        var v;
        for(var i = 0; i < len; i++){
            v = inputFreq.shift() * curve[i];
            outSeqFreqReal.push(v / len);
            outputFreq.push(v);
            v = inputFreq.shift() * curve[i];
            outSeqFreqImg.push(v / len);
            outputFreq.push(v);
        }    

        var outputTime = [];
        ffti.simple(outputTime, outputFreq, 'complex');
        

        var outSeqReal = [];
        var outSeqImg = [];
        for(var i = 0; i < len; i++){
            outSeqReal.push(outputTime.shift() / len);
            outSeqImg.push(outputTime.shift() / len);
        }  

        outlet(3, outSeqFreqImg);
        outlet(2, outSeqFreqReal);
        outlet(1, outSeqImg);
        outlet(0, outSeqReal);
    }
}