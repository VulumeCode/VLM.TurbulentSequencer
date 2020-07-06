inlets = 2;

var probs=[];
var floats=[];
var notes=[];

function list()
{
	switch (inlet) {
		case 0:
			floats = arrayfromargs(arguments);
			break;
		case 1:
			probs = arrayfromargs(arguments);
			break;
	}
	calc();
	bang();
}

function calc()
{
			notes=[];
			for (i = 0; i < floats.length; i++){
				v = floats[i];
				for (j = 0; j < probs.length; j++){
					if((v < 1.) ? (v < probs[j]) : (v == probs[j])){
						notes.push(j);
						break;
					}
				}	
			}
}


function bang()
{
	outlet(0, notes);
}