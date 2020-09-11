inlets = 1;
outlets = 1;

let value: number | any[] | string;
let t: Task | null;
let time = jsarguments.length > 1 ? jsarguments[1] : 1000;

function debounce() {
    if (!t) {
        t = new Task(bounce);
        t.schedule(time);
    }
}

function bounce() {
    t = null;
    outlet(0, value);
}

function msg_int(v: number) {
    value = v;
    debounce();
}

function msg_float(v: number) {
    value = v;
    debounce();
}

function list() {
    value = arrayfromargs(arguments);
    debounce();
}

function bang() {
    value = 'bang';
    debounce();
}

let module = {};
export = {};