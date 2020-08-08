import { postError } from './jsCommon';

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

function hsvToRgb(h: number, s: number, v: number) {
    var r, g, b;
  
    var i = Math.floor(h * 6);
    var f = h * 6 - i;
    var p = v * (1 - s);
    var q = v * (1 - f * s);
    var t = v * (1 - (1 - f) * s);
  
    switch (i % 6) {
      case 0: r = v, g = t, b = p; break;
      case 1: r = q, g = v, b = p; break;
      case 2: r = p, g = v, b = t; break;
      case 3: r = p, g = q, b = v; break;
      case 4: r = t, g = p, b = v; break;
      case 5: r = v, g = p, b = q; break;
      default: throw "impossible";
    }
  
    return [ r, g, b ];
}


function draw() {
    let len = seqReal.length;
    atSketch($ =>{
        $.glscale(1.9,1.9);
        $.gltranslate(-0.5,-0.5);
        $.gllinewidth(2);
        
        // set how the polygons are rendered
        $.glclearcolor(vbrgb[0], vbrgb[1], vbrgb[2], vbrgb[3]) // set the clear color
        $.glclear() // erase the background

        $.moveto(seqReal[len-1], seqImg[len-1])

        for (
            let i = 0;
            i < seqReal.length;
            i++ // iterate through the columns
        ) {
            const [r,g,b] = hsvToRgb(0.545666667,1,i / seqReal.length)
            $.glcolor(r,g,b,1)
            $.lineto(seqReal[i], seqImg[i]);
        }
    });
}

var vbrgb = [0,0,0,0];
var vfrgb = [0.118, 0.118, 0.118, 1.000];

try {
    // set up jsui defaults to 2d
    sketch.default2d();
    // initialize graphics
    draw();
    refresh();
} catch (e) {
    postError(e, "Error initializing drawCircle");
}


let module = {};
export = {};