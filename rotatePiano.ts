inlets = 2;
outlets = 2;

var noteToggles: number[]  = [1,1,1,1,1,1,1,1,1,1,1,1];

var noteView: number[]  = [1,1,1,1,1,1,1,1,1,1,1,1];
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
            const i = (arguments[0] + steps) % viewLength;
            const v = +(arguments[1] == 0); 
			if(noteToggles[i] === v){
                return;
            }
            else {
                noteToggles[i] = v;
                post(noteToggles.map(n=>n+".").join(' ') + "\n")
                outlet(1, noteToggles);
            }

            break;
        case 1:
            const updatedValues = arrayfromargs(arguments);
            if(arrays_equal(noteToggles, updatedValues)){
                return;
            }
            else {
                noteToggles = updatedValues;
                bang();
            }
            break;
    }
}

function bang()
{
    noteView = noteToggles.slice(steps, noteToggles.length).concat(noteToggles.slice(0, steps));
    noteView.forEach((v,i)=>{
        outlet(0, [i,!v]);
    })
}


let module = {};
export = {};