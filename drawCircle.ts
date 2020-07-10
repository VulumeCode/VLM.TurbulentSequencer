inlets = 2;
outlets = 0;


var seqReal: number[] = [];
var seqImg: number[] = [];

function list()
{
    try {
        switch (inlet) {
            case 0:
                seqReal = arrayfromargs(arguments);
                break;
            case 1:
                seqImg = arrayfromargs(arguments);
                break;
        }
        bang();
        
    } catch (error) {
        post(JSON.stringify(error));
    }
}

function bang()
{
    draw();
    refresh();
}

function draw() {
    let len = seqReal.length;
    sketch.glpushmatrix();
        sketch.glscale(2,2);
        sketch.gltranslate(-0.5,-0.5);
        sketch.gllinewidth(1.5);
        
        // set how the polygons are rendered
        sketch.glclearcolor(vbrgb[0], vbrgb[1], vbrgb[2], vbrgb[3]) // set the clear color
        sketch.glclear() // erase the background
        sketch.glcolor(vfrgb[0], vfrgb[1], vfrgb[2], vfrgb[3])

        sketch.moveto(seqReal[len-1], seqImg[len-1])

        for (
            let i = 0;
            i < seqReal.length;
            i++ // iterate through the columns
        ) {
            sketch.lineto(seqReal[i], seqImg[i]);
        }
    sketch.glpopmatrix();
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