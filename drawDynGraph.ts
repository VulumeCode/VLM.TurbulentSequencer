import { postError, atSketch } from './jsCommon';

inlets = 3;
outlets = 0;

var seq: number[] = [];

var bot: number;
var top: number;

function list() {
    switch (inlet) {
        case 0:
            seq = arrayfromargs(arguments);
            break;
    }
    bang();
}

function setBot(n: number) {
    bot = n;
    bang();
}

function setTop(n: number) {
    top = n;
    bang();
}

function bang() {
    draw();
    refresh();
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

    return [r, g, b];
}


function draw() {
    let len = seq.length;
    atSketch($ => {
        $.glscale(1.8, 1.8);
        $.gltranslate(-0.5, -0.5);
        // set how the polygons are rendered
        $.glclearcolor(vbrgb[0], vbrgb[1], vbrgb[2], vbrgb[3]) // set the clear color
        $.glclear() // erase the background

        $.gllinewidth(5);
        $.moveto(seq[0], 1);

        $.glcolor(0, 0, 0, 0.7);
        for (
            let i = 1;
            i < len;
            i++ // iterate through the columns
        ) {
            $.lineto(seq[i], (len - i) / len);
        }


        $.gllinewidth(2);
        $.moveto(seq[0], 1);

        for (
            let i = 1;
            i < len;
            i++ // iterate through the columns
        ) {
            const [r, g, b] = hsvToRgb(0.83 - ((0.83 - 0.5) * (i / len)), 1, 0.9)
            $.glcolor(r, g, b, 1)
            $.lineto(seq[i], (len - i) / len);
        }

        $.gllinewidth(1);
        $.glcolor(0, 0, 0, 0.7);
        $.moveto(bot, 0);
        $.lineto(bot, 1);

        $.gllinewidth(1);
        $.glcolor(0, 0, 0, 0.7);
        $.moveto(1 + top, 0);
        $.lineto(1 + top, 1);

    });
}

var vbrgb = [0, 0, 0, 0];
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