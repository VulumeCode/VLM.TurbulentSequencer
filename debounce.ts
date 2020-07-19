inlets = 1;
outlets = 1;

let value: number | any[];
let t: Task | null;
let time = jsarguments.length > 1 ? jsarguments[1] : 1000;

function debounce()
{
    if(!t){
        t = new Task(bang);
        t.schedule(time);
    }
}

function msg_int(v: number)
{
    value = v;
    debounce();
}

function msg_float(v: number)
{
    value = v;
    debounce();
}

function list()
{
    value = arrayfromargs(arguments);
    debounce();
}

function bang()
{
    t = null;
    outlet(0, value);
}

let module = {};
export = {};