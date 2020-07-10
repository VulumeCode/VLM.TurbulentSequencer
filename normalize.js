inlets = 1;
outlets = 1;

var input = [];


function list()
{
	switch (inlet) {
		case 0:
			input = arrayfromargs(arguments);
			break;
	}
	bang();
}

function bang()
{
    var sum = 0;
    var min = Infinity;
    var max = -Infinity;
    for (var i = 0; i < input.length; i++) {
        sum += input[i];
        if ((input[i]) < min) {
            min = (input[i]);
        }
        if ((input[i]) > max) {
            max = (input[i]);
        }
    }
    var avg = sum/input.length;

    var deviation = Math.max(max-avg, avg-min);

    var output = [];

    for (var i = 0; i < input.length; i++) {
        output.push((((input[i]-avg)/deviation)+1)/2);
    }
    outlet(0, output);
}