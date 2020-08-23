inlets = 3;
outlets = 2;

var octProbs: number[] = [];
var noteProbs: number[]  = [];
var noteToggles: number[]  = [1,1,1,1,1,1,1,1,1,1,1,1];

function list()
{
	switch (inlet) {
		case 0:
			octProbs = arrayfromargs(arguments);
			octProbs.reverse();
			break;
		case 1:
			noteProbs = arrayfromargs(arguments);
			break;
		case 2:
			noteToggles = arrayfromargs(arguments);
			break;
	}
	bang();
}

function bang()
{
	const used: number[] = [];
	let note = 0;
	const accumProbs: number[] = [];
	let accum = 0;
	for (let i = 0; i < octProbs.length; i++){
		for (let j = 0; j < noteProbs.length; j++){
			const prob=octProbs[i]*noteProbs[j]*noteToggles[j];
			if(prob>0){
				used.push(note);
			}
			accum += prob;
			accumProbs.push(accum);
			note++;
		}
	}
	for (let i = 0; i < accumProbs.length; i++){
		accumProbs[i]=accumProbs[i]/accum;
	}
	
	outlet(0, used);
	outlet(1, accumProbs);
}

let module = {};
export = {};