inlets = 1;
outlets = 1;

let values: number[] = [];
let view: number[] = [];
let steps = 0;
let viewLength: number;

function msg_int(v: number)
{
    steps = v;

    view = values.slice(steps + viewLength, steps + viewLength + viewLength);
    bang();
}

function list()
{
    const input = arrayfromargs(arguments);
    
    viewLength = input.length;

    if(0===values.length){
        for (let i = 0; i < viewLength*3; i++) {
            values.push(0);
        }
    }

    values.splice(viewLength + steps, viewLength, ...input);
}

function bang()
{
    outlet(0, view);
}

let module = {};
export = {};