inlets = 1;
outlets = 1;

function msg_int(v: number) {
    let out: number[] = [];
    for (let i = 0; i < v; i++) {
        out.push(+((i % 8) < 4));
    }
    outlet(0, out);
}