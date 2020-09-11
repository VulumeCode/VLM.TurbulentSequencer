inlets = 2;
outlets = 1;

var floats: number[] = [];

var choices: string[] = [];

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

function bang() {
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
                ? 60
                : 120
        )
    }
    for (const f of floats) {
        addChoice(choices[Math.floor(f * (5 * nextFloatDown))]);
    }
    outlet(0, ["duration", 1, ...durations]);
    outlet(0, ["velocity", 1, ...velocities]);
}

const nextFloatDown = (1 - 2 ** -53);