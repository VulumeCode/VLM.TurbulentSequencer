inlets = 2;
outlets = 1;

var floats: number[] = [];

var choices: string[] = [];

function list()
{
    switch (inlet) {
        case 0:
            floats = arrayfromargs(arguments);
            bang();
            break;
        case 1:
            choices = arrayfromargs(arguments);
            bang();
            break;
    }
}

function bang(){
    const durations: number[] = []
    const velocities: number[] = []
    const addChoice = (choice :string ) => {
        velocities.push(
            choice.includes('R')
                ? 0 
                : choice.includes('A')
                    ? 127
                    : 63
        )
        durations.push(
            choice.includes('L')
                ? 60
                : 120
        )
    }
    floats.forEach(
        (f)=>addChoice(choices[Math.floor(f*5.)])
    )
    outlet(0, ["duration", 1, ...durations] );
    outlet(0, ["velocity", 1, ...velocities]);
}