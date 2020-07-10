inlets = 1;
outlets = 1;

var seq: number[] = [];

function list()
{
    try {
        switch (inlet) {
            case 0:
                seq = arrayfromargs(arguments);
                break;
        }
        bang();
        
    } catch (error) {
        post(JSON.stringify(error));
    }
}

var asinT = function (a: number) {
    return Math.asin(a)/Math.PI + 0.5;
};

function bang()
{
    var len = seq.length;

    var outSeq = [];

    for(let i = 0; i < len; i++){
        outSeq.push(asinT(seq[i]));
    }    

    outlet(0, outSeq);
}

export {}