inlets = 2;
outlets = 1;

let floats: number[] = [];

let choices: string[] = [];

let bot: number;
let mid: number;
let top: number;


function list() {
    switch (inlet) {
        case 0:
            floats = arrayfromargs(arguments);
            bang();
            break;
    }
}

function setChoices() {
    choices = arrayfromargs(arguments);
    bang();
}

function setBot(n: number) {
    bot = n;
    bang();
}

function setMid(n: number) {
    mid = n;
    bang();
}

function setTop(n: number) {
    top = n;
    bang();
}

function bang() {
    const stages = [
        bot,
        0.5 - (mid / 2),
        0.5 + (mid / 2),
        1 - top,
        1
    ]
    const durations: number[] = []
    const velocities: number[] = []
    const addChoice = (choice: string) => {
        velocities.push(
            choice.indexOf('R') >= 0
                ? 0
                : choice.indexOf('A') >= 0
                    ? 127
                    : 63
        )
        durations.push(
            choice.indexOf('L') >= 0
                ? 120
                : 60
        )
    }
    for (let f of floats) {
        let i: number
        f = f * nextFloatDown;
        for (i = 0; i < stages.length; i++) {
            if (f < stages[i]) {
                break;
            }
        }
        addChoice(choices[i]);
    }
    outlet(0, ["duration", 1, ...durations]);
    outlet(0, ["velocity", 1, ...velocities]);
}

const nextFloatDown = (1 - 2 ** -53);