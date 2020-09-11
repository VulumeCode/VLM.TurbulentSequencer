var probs: number[] = [];

function list() {
    probs = arrayfromargs(arguments);
}

function msg_float(v: number) {
    for (let i = 0; i < probs.length; i++) {
        if ((v < 1.) ? (v < probs[i]) : (v == probs[i])) {
            outlet(0, i);
            break;
        }
    }
}

let module = {};
export = {};