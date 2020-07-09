"use strict";
exports.__esModule = true;
inlets = 3;
outlets = 2;
var seqReal = [];
var seqImg = [];
var angle = 0;
function msg_float(v) {
    switch (inlet) {
        case 2:
            angle = v;
            break;
    }
    bang();
}
function list() {
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
    return ({
        t: Math.atan2(y, x),
        r: Math.sqrt((x * x) + (y * y))
    });
};
var poltocar = function (t, r) {
    return ({
        x: r * Math.cos(t),
        y: r * Math.sin(t)
    });
};
function bang() {
    var len = seqReal.length;
    var outSeqReal = [];
    var outSeqImg = [];
    for (var i = 0; i < len; i++) {
        var pol = cartopol(seqReal[i], seqImg[i]);
        var outCar = poltocar(pol.t - angle, pol.r);
        outSeqReal.push(outCar.x);
        outSeqImg.push(outCar.y);
    }
    outlet(1, outSeqImg);
    outlet(0, outSeqReal);
}
