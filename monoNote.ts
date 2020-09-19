inlets = 1;

var lastNote: number | null = null;
var lastTie: boolean = false;

function list() {
    const [_, note, vel, duration] = arrayfromargs(arguments) as number[];

    const tie = duration == 120;
    const rest = vel == 0;

    if (
        lastNote === null
        && !rest
    ) {
        outlet(0, [note, vel]);
    }
    else if (
        lastTie
        && !rest
        && lastNote !== note
    ) {
        outlet(0, [note, vel]);
        outlet(0, [lastNote, 0]);
    }
    else if (
        lastNote !== null
        && rest
    ) {
        outlet(0, [lastNote, 0]);
    }
    else if (
        lastNote !== null
        && !lastTie
    ) {
        outlet(0, [lastNote, 0]);
        outlet(0, [note, vel]);
    }

    lastNote = rest ? null : note;
    lastTie = tie;
}

let module = {};
export = {};