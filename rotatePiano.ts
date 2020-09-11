inlets = 2;
outlets = 3;

var noteToggles: number[] = [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1];

var noteView: number[] = [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1];
var noteViewPrev: (number | undefined)[] = [undefined, undefined, undefined, undefined, undefined, undefined, undefined, undefined, undefined, undefined, undefined, undefined];
let viewLength = 12;
let steps = 0;

function msg_int(v: number) {
    const l = viewLength;
    steps = (-v + l) % l;
    setPiano();
}

function arrays_equal(a: any[], b: any[]) { return !!a && !!b && !(a < b || b < a); }

function list() {
    switch (inlet) {
        case 0:
            const i = (arguments[0] + steps) % viewLength;
            const v = +(arguments[1] == 0);
            if (noteToggles[i] === v) {
                return;
            }
            else {
                noteToggles[i] = v;
                outlet(2, noteToggles);
                calcView();
                outlet(1, noteView);
            }

            break;
        case 1:
            const updatedValues = arrayfromargs(arguments);
            if (arrays_equal(noteToggles, updatedValues)) {
                return;
            }
            else {
                noteToggles = updatedValues;
                setPiano();
            }
            break;
    }
}


function calcView() {
    noteView = noteToggles.slice(steps, noteToggles.length).concat(noteToggles.slice(0, steps));
}

function setPiano() {
    calcView();
    noteView.forEach((v, i) => {
        if (v !== noteViewPrev[i]) {
            outlet(0, ["set", i, +!v]);
        }
    });
    noteViewPrev = noteView;
    outlet(1, noteView);
}


let module = {};
export = {};