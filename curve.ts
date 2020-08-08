inlets = 3;
outlets = 1;

var beta = 1;
var center = 1;
var len = 2;


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
    
	let max = 0;

    let i = 1;
    for (; i < center; i++) {
        const e = len * ((center-i)/center);
		const s = 1./Math.pow(e,beta);
		max = s > max ? s : max;
        curve.push(s);
    }


    for (; i < len; i++) {
        const e = i - center + 1;
        const s = 1./Math.pow(e,beta);
		max = s > max ? s : max;
        curve.push(s);
	}
	
	for (const i in curve) {
		curve[i] = curve[i]/max;
	}

    curve.unshift(1);

	outlet(0, curve);
}

let module = {};
export = {};