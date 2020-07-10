inlets = 2;

var probs: number[] = [];
var floats: number[] = [];
var notes: number[] = [];

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
			for (let i = 0; i < floats.length; i++){
				const v = floats[i];
				for (let j = 0; j < probs.length; j++){
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

let module = {};
export = {};