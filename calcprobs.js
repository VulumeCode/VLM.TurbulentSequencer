inlets = 3;
outlets = 2;

var octProbs = [];
var noteProbs = [];
var noteToggles = [1,1,1,1,1,1,1,1,1,1,1,1];

var probs=[];

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
			noteToggles[arguments[0]] = arguments[1] == 0; 
			break;
	}
	bang();
}

function bang()
{
	probs=[];
	accumProbs=[];
	accum=0;
	for (i = 0; i < octProbs.length; i++){
		for (j = 0; j < noteProbs.length; j++){
			prob=octProbs[i]*noteProbs[j]*noteToggles[j];
			probs.push(prob);
			accum += prob;
			accumProbs.push(accum);
		}
	}
	for (i = 0; i < accumProbs.length; i++){
		accumProbs[i]=accumProbs[i]/accum;
	}
	
	outlet(0, probs);
	outlet(1, accumProbs);
}