import * as Random from './random';
import { postError } from './jsCommon';

inlets = 2;
outlets = 2;

var maxLength: number = 64;

var order: number[] = [0, maxLength - 1]

search:
for (let denominator = 2; denominator < 100; denominator++) {
    for (let numerator = 1; numerator < denominator; numerator++) {
        const fraction = numerator / denominator;
        const number = Math.round(fraction * maxLength);
        if (order.indexOf(number) === -1) {
            order.push(number);
            if (order.length === maxLength) {
                break search;
            }
        }
    }
}

let seed: number = 0;
let take: number = 16;

let allAs: number[] = [];
let allBs: number[] = [];

function msg_int(b: number) {
    try {
        switch (inlet) {
            case 0:
                take = b;
                break;
            case 1:
                seed = b;
                regenerate();
                break;
        }
        bang();
    } catch (e) {
        postError(e);
    }
}

function regenerate() {
    allAs = [];
    allBs = [];
    let rng = new Random.MT(seed);
    for (let i = 0; i < maxLength; i++) {
        do {
            var b = 2 * (rng.normal() - 0.5);
            var c = 2 * (rng.normal() - 0.5);
        } while (1 < Math.sqrt(b * b + c * c));
        allAs.push(b);
        allBs.push(c);
    }
}

function bang() {
    var indices = order.slice(0, take).sort(function (a, b) { return a - b });

    var as = [];
    var bs = [];
    for (let i = 0; i < take; i++) {
        as.push(allAs[indices[i]]);
        bs.push(allBs[indices[i]]);
    }
    outlet(1, as);
    outlet(0, bs);
}

function loadbang() {
    regenerate();
    bang();
}

let module = {};
export = {};