inlets = 2;
outlets = 0;

var seqReal: number[] = [];
var seqImg: number[] = [];

function list()
{
    switch (inlet) {
        case 0:
            seqReal = arrayfromargs(arguments);
            break;
        case 1:
            seqImg = arrayfromargs(arguments);
            break;
    }
    bang();
}

function bang()
{
    draw();
    refresh();
}

function atSketch(f: (_:Sketch) => void){
    sketch.glpushmatrix();
    f(sketch);
    sketch.glpopmatrix();
}

function draw() {
    let len = seqReal.length;
    atSketch($ =>{
        $.glscale(2,2);
        $.gltranslate(-0.5,-0.5);
        $.gllinewidth(1.5);
        
        // set how the polygons are rendered
        $.glclearcolor(vbrgb[0], vbrgb[1], vbrgb[2], vbrgb[3]) // set the clear color
        $.glclear() // erase the background
        $.glcolor(vfrgb[0], vfrgb[1], vfrgb[2], vfrgb[3])

        $.moveto(seqReal[len-1], seqImg[len-1])

        for (
            let i = 0;
            i < seqReal.length;
            i++ // iterate through the columns
        ) {
            $.lineto(seqReal[i], seqImg[i]);
        }
    });
}

var vbrgb = [0,0,0,1];
var vmrgb = [0.9,0.5,0.5,0.75];
var vfrgb = [245./255, 111./255, 19./255, 1 ];

// set up jsui defaults to 2d
sketch.default2d();
// initialize graphics
draw();
refresh();

let module = {};
export = {};