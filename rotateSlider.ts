inlets = 2;
outlets = 2;

let values: number[] = [];

let view: number[] = [];
let viewLength = 12;
let steps = 0;

function msg_int(v: number)
{
    const l = viewLength;
    steps = (-v + l) % l;
    bang();
}

function arrays_equal(a: any[],b: any[]) { return !!a && !!b && !(a<b || b<a); }

function list()
{
    switch (inlet) {
		case 0:
            const input = arrayfromargs(arguments);

            values = input.slice(-steps, viewLength).concat(input.slice(0, -steps));

            outlet(1, values);
            outlet(0, "setlist " + view.join(' '));
            break;
        case 1:
            const updatedValues = arrayfromargs(arguments);
            if(arrays_equal(values, updatedValues)){
                return;
            }
            else {
                values = updatedValues;
                bang();
            }
            break;
    }
}

function bang()
{
    view = values.slice(steps, values.length).concat(values.slice(0, steps));
    outlet(0, view);
}


let module = {};
export = {};