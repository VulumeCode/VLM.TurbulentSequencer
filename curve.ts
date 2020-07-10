inlets = 3;
outlets = 1;

var beta = 1;
var center = 1;
var len = 0;


function msg_float(v: number)
{
	switch (inlet) {
		case 0:
			beta = v;
			break;
		case 1:
			center = v*len;
			break;
	}
	bang();
}

function msg_int(v: number)
{
	switch (inlet) {
		case 0:
			beta = v;
			break;
		case 1:
			center = v;
			break;
        case 2:
            len = v;
            break;
	}
	bang();
}

function bang()
{
    let curve: number[] = [];
    
    curve.push(1);

    var i = 1;
    for (; i < center; i++) {
        var e = len * ((center-i)/center);
        
        curve.push(1./Math.pow(e,beta));
    }


    for (; i < len; i++) {
        var e = i - center + 1;
        
        curve.push(1./Math.pow(e,beta));
    }
	outlet(0, curve);
}

let module = {};
export = {};