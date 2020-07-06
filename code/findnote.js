var probs=[];

function list()
{
	probs = arrayfromargs(arguments);
}

function msg_float(v)
{
	for (i = 0; i < probs.length; i++){
		if((v < 1.) ? (v < probs[i]) : (v == probs[i])){
			outlet(0, i);
			break;
		}	
	}
}