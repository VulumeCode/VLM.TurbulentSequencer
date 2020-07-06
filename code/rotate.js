inlets = 3;
outlets = 2;

var seqReal = [];
var seqImg = [];
var angle = 0;

function msg_float(v)
{
	switch (inlet) {
        case 2:
            angle = v;
            break;
	}
	bang();
}

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
	}
}

var cartopol = function (x, y) {
    if ( x === void 0 ) x = 0;
    if ( y === void 0 ) y = 0;

    return ({
        t: Math.atan2(y, x),
        r: Math.sqrt((x * x) + (y * y))
    });
};

var poltocar = function (t, r) {
    if ( t === void 0 ) t = 0;
    if ( r === void 0 ) r = 1;
  
    return ({
        x: r * Math.cos(t),
        y: r * Math.sin(t)
    });
};

function bang()
{
    var len = seqReal.length;
    var outSeqReal = [];
    var outSeqImg = [];

    for(i = 0; i < len; i++){
        var pol = cartopol(seqReal[i], seqImg[i]);
        var outCar = poltocar(pol.t - angle, pol.r);
        outSeqReal.push(outCar.x);
        outSeqImg.push(outCar.y);
    }    

    outlet(1, outSeqImg);
	outlet(0, outSeqReal);
}