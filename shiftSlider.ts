inlets = 2;
outlets = 2;

let values: number[] = [];
let view: number[] = [];
let steps = 0;
let viewLength = 10;

function msg_int(v: number) {
    steps = -v;
    bang();
}

function arrays_equal(a: any[], b: any[]) { return !!a && !!b && !(a < b || b < a); }

function list() {
    switch (inlet) {
        case 0:
            const input = arrayfromargs(arguments);

            if (0 === values.length) {
                for (let i = 0; i < viewLength * 3; i++) {
                    values.push(0);
                }
            }

            values.splice(viewLength + steps, viewLength, ...input);
            outlet(1, values);
            outlet(0, "setlist " + view.join(' '));
            break;
        case 1:
            const updatedValues = arrayfromargs(arguments);
            if (arrays_equal(values, updatedValues)) {
                return;
            }
            else {
                values = updatedValues;
                bang();
            }
            break;
    }
}

function bang() {
    view = values.slice(steps + viewLength, steps + viewLength + viewLength);

    outlet(0, view);
}

let module = {};
export = {};