inlets = 1;
outlets = 1;

let values: number[] = [];
let rotatedValues: number[] = [];
let steps = 0;

function msg_int(v: number)
{
    const diff = steps - v;
    steps = v;

    rotatedValues = values.slice(diff, values.length).concat(values.slice(0, diff));
    bang();
}

function list()
{
    values = arrayfromargs(arguments);
}

function bang()
{
    outlet(0, rotatedValues);
}


let module = {};
export = {};