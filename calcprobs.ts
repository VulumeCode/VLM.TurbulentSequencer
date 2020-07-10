inlets = 3;
outlets = 2;

var octProbs: number[] = [];
var noteProbs: number[]  = [];
var noteToggles: number[]  = [1,1,1,1,1,1,1,1,1,1,1,1];

var probs: number[] = [];

function list()
{
	switch (inlet) {
		case 0:
			octProbs = arrayfromargs(arguments);
			break;
		case 1:
			noteProbs = arrayfromargs(arguments);
			break;
		case 2:
			noteToggles[arguments[0]] = +(arguments[1] == 0); 
			break;
	}
	bang();
}

function bang()
{
	const probs: number[] = [];
	const accumProbs: number[] = [];
	let accum = 0;
	for (let i = 0; i < octProbs.length; i++){
		for (let j = 0; j < noteProbs.length; j++){
			const prob=octProbs[i]*noteProbs[j]*noteToggles[j];
			probs.push(prob);
			accum += prob;
			accumProbs.push(accum);
		}
	}
	for (let i = 0; i < accumProbs.length; i++){
		accumProbs[i]=accumProbs[i]/accum;
	}
	
	outlet(0, probs);
	outlet(1, accumProbs);
}

let module = {};
export = {};