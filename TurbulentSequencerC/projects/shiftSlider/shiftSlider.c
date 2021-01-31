#include "ext.h"				// you must include this - it contains the external object's link to available Max functions
#include "ext_obex.h"			// this is required for all objects using the newer style for writing objects.

#define viewLength 10
#define valuesLength 30

typedef struct _shiftSlider {		// defines our object's internal variables for each instance in a patch
	t_object i_ob;
	long i_in;          // space for the inlet number used by all the proxies
	void* i_proxy;

	double values[valuesLength];

	short steps;

	void* out0;
	void* out1;
} t_shiftSlider;

// these are prototypes for the methods that are defined below
void *shiftSlider_new(long dummy);
void shiftSlider_free(t_shiftSlider *x);
void shiftSlider_assist(t_shiftSlider *x, void *b, long m, long a, char *s);
void shiftSlider_int(t_shiftSlider* x, long steps);
void shiftSlider_outputView(t_shiftSlider* x);
void shiftSlider_list(t_shiftSlider* x, t_symbol* s, short ac, t_atom* av);
void shiftSlider_update(t_shiftSlider* x, t_symbol* s, short ac, t_atom* av);
bool arrayEquals(short ac, t_atom_float* av, short bc, t_atom_float* bv);

t_class *shiftSlider_class;		// global pointer to the object class - so max can reference the object

//--------------------------------------------------------------------------

void ext_main(void *r)
{
	t_class *c;

	c = class_new("shiftSlider", (method)shiftSlider_new, (method)shiftSlider_free, sizeof(t_shiftSlider), 0L, A_DEFLONG, 0);

	class_addmethod(c, (method)shiftSlider_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)shiftSlider_int, "int", A_LONG, 0);

	class_addmethod(c, (method)shiftSlider_assist,	"assist",	A_CANT, 0);	

	class_register(CLASS_BOX, c);
	shiftSlider_class = c;

	object_post(NULL, "shiftSlider object loaded...",0);	// post any important info to the max window when our class is loaded
}

//--------------------------------------------------------------------------

void *shiftSlider_new(long dummy)		// dummy = int argument typed into object box (A_DEFLONG) -- defaults to 0 if no args are typed
{
	t_shiftSlider* x;

	x = object_alloc(shiftSlider_class);

	proxy_new(&x->i_ob, 2, &x->i_in);
	proxy_new(&x->i_ob, 1, &x->i_in);

	x->steps = 0;

	x->out1 = listout(x);
	x->out0 = listout(x);


	// initialize with zeroes
	for (int i = 0; i < valuesLength; i++) {
		x->values[i] = 0;
	}

	return x;
}

void shiftSlider_free(t_shiftSlider *x)
{
	if (x->values)
		sysmem_freeptr(x->values);
}

//--------------------------------------------------------------------------

void shiftSlider_assist(t_shiftSlider* x, void* b, long m, long a, char* s) // 4 final arguments are always the same for the assistance method
{
	if (m == ASSIST_INLET){
		switch (a) {
		case 0:
			sprintf(s, "list to shift");
			break;
		case 1:
			sprintf(s, "shift steps");
			break;
		case 2:
			sprintf(s, "list values");
			break;
		}
	}
	else {
		switch (a) {
		case 0:
			sprintf(s, "shifted list");
			break;
		case 1:
			sprintf(s, "updated values");
			break;
		}
	}
}

void shiftSlider_int(t_shiftSlider* x, long steps) {
	switch (proxy_getinlet(&x->i_ob)) {
	case 0:
		error("No int here pls");
		break;
	case 1:
		x->steps = (short) -steps;
		shiftSlider_outputView(x);
		break;
	case 2:
		error("No int here pls");
		break;
	}
}

void shiftSlider_outputView(t_shiftSlider *x)
{
	t_atom* output = (t_atom*)sysmem_newptr(viewLength * sizeof(t_atom));

	atom_setdouble_array(viewLength, output, viewLength, x->values + x->steps + viewLength);

	outlet_list(x->out0, NULL, viewLength, output);
	sysmem_freeptr(output);
}

void shiftSlider_list(t_shiftSlider* x, t_symbol* s, short ac, t_atom* av) {
	switch (proxy_getinlet(&x->i_ob)) {
	case 0:
		atom_getdouble_array(ac, av, ac, x->values + x->steps + viewLength);
		t_atom* output = (t_atom*)sysmem_newptr(valuesLength * sizeof(t_atom));
		atom_setdouble_array(valuesLength, output, valuesLength, x->values);
		outlet_list(x->out1, NULL, valuesLength, output);
		sysmem_freeptr(output);

		break;
	case 1:
		error("No list here pls");
		break;
	case 2:
		shiftSlider_update(x, s, ac, av);
		break;
	}
}

void shiftSlider_update(t_shiftSlider* x, t_symbol* s, short ac, t_atom* av)
{
	double* input = (double*)sysmem_newptr(ac * sizeof(double));
	atom_getdouble_array(ac, av, ac, input);

	if (arrayEquals(ac, input, valuesLength, x->values)) {
		return;
	}
	else {
		*(x->values) = *input;
		shiftSlider_outputView(x);
	}
}
bool arrayEquals(short ac, double* av, short bc, double* bv)
{
	if (av == NULL|| bv == NULL|| ac != bc) {
		return false;
	}
	for (int i = 0; i < ac; i++) {
		double av_i = av[i];
		double bv_i = bv[i];
		if (av_i != bv_i) {
			return false;
		}
	}
	return true;
}