#include "ext.h"				// you must include this - it contains the external object's link to available Max functions
#include "ext_obex.h"			// this is required for all objects using the newer style for writing objects.

typedef struct _rotateSlider {		// defines our object's internal variables for each instance in a patch
	t_object i_ob;
	long i_in;          // space for the inlet number used by all the proxies
	void* i_proxy;

	t_atom_float* values;
	short values_ac;

	short viewLength;

	int steps;

	void* out0;
	void* out1;
} t_rotateSlider;

// these are prototypes for the methods that are defined below
void *rotateSlider_new(long dummy);
void rotateSlider_free(t_rotateSlider *x);
void rotateSlider_assist(t_rotateSlider *x, void *b, long m, long a, char *s);
void rotateSlider_setSteps(t_rotateSlider* x, long steps);
void rotateSlider_outputView(t_rotateSlider* x);
void rotateSlider_list(t_rotateSlider* x, t_symbol* s, short ac, t_atom* av);
void rotateSlider_rotate(t_rotateSlider* x, t_symbol* s, short ac, t_atom* av);
void rotateSlider_update(t_rotateSlider* x, t_symbol* s, short ac, t_atom* av);
void rotateSlider_resize_values(t_rotateSlider* x, short size);
bool arrayEquals(short ac, t_atom_float* av, short bc, t_atom_float* bv);

t_class *rotateSlider_class;		// global pointer to the object class - so max can reference the object

//--------------------------------------------------------------------------

void ext_main(void *r)
{
	t_class *c;

	c = class_new("rotateSlider", (method)rotateSlider_new, (method)rotateSlider_free, sizeof(t_rotateSlider), 0L, A_DEFLONG, 0);

	class_addmethod(c, (method)rotateSlider_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)rotateSlider_setSteps, "in1", A_LONG, 0);

	class_addmethod(c, (method)rotateSlider_assist,	"assist",	A_CANT, 0);	

	class_register(CLASS_BOX, c);
	rotateSlider_class = c;

	object_post(NULL, "rotateSlider object loaded...",0);	// post any important info to the max window when our class is loaded
}

//--------------------------------------------------------------------------

void *rotateSlider_new(long dummy)		// dummy = int argument typed into object box (A_DEFLONG) -- defaults to 0 if no args are typed
{
	t_rotateSlider* x;

	x = object_alloc(rotateSlider_class);

	x->i_proxy = proxy_new(&x->i_ob, 2, &x->i_in);
	intin(x, 1);

	x->viewLength = 12;
	x->steps = 0;

	x->out1 = listout(x);
	x->out0 = listout(x);
	return x;
}

void rotateSlider_free(t_rotateSlider *x)
{
	if (x->values)
		sysmem_freeptr(x->values);

	freeobject((t_object*)x->i_proxy);
}

//--------------------------------------------------------------------------

void rotateSlider_assist(t_rotateSlider* x, void* b, long m, long a, char* s) // 4 final arguments are always the same for the assistance method
{
	if (m == ASSIST_INLET){
		switch (a) {
		case 0:
			sprintf(s, "list to rotate");
			break;
		case 1:
			sprintf(s, "rotation steps");
			break;
		case 2:
			sprintf(s, "list values");
			break;
		}
	}
	else {
		switch (a) {
		case 0:
			sprintf(s, "rotated list");
			break;
		case 1:
			sprintf(s, "updated values");
			break;
		}
	}
}

void rotateSlider_setSteps(t_rotateSlider* x, long steps) {
	x->steps = (-steps + x->viewLength) % x->viewLength;
	rotateSlider_outputView(x);
}

void rotateSlider_outputView(t_rotateSlider *x)
{
	if (x->values != NULL) {
		short length = x->values_ac;
		short steps = ((x->steps) + length) % length;
		t_atom* output = (t_atom*)sysmem_newptr(length * sizeof(t_atom));
		short to_i = 0;
		for (short i = steps; i < length; i++, to_i++) {
			atom_setfloat(
				&output[to_i],
				*(x->values + i));
		}
		for (short i = 0; i < steps; i++, to_i++) {
			atom_setfloat(
				&output[to_i],
				*(x->values + i));
		}

		outlet_list(x->out0, NULL, length, output);
		sysmem_freeptr(output);
	}
}

void rotateSlider_list(t_rotateSlider* x, t_symbol* s, short ac, t_atom* av) {
	switch (proxy_getinlet(&x->i_ob)) {
	case 0:
		rotateSlider_rotate(x, s, ac, av);
		break;
	case 1:
		error("No list here pls");
		break;
	case 2:
		rotateSlider_update(x, s, ac, av);
		break;
	}
}


void rotateSlider_rotate(t_rotateSlider* x, t_symbol* s, short ac, t_atom* av)
{
	rotateSlider_resize_values(x, ac);

	short length = x->values_ac;
	short steps = ((-(x->steps)) + length) % length;
	short to_i = 0;
	for (short i = steps; i < length; i++, to_i++) {
		x->values[to_i] = atom_getfloat(av + i);
	}
	for (short i = 0; i < steps; i++, to_i++) {
		x->values[to_i] = atom_getfloat(av + i);
	}

	t_atom* out_atoms = (t_atom*)sysmem_newptr(length * sizeof(t_atom));
	atom_setdouble_array(length, out_atoms, length, x->values);

	outlet_list(x->out1, NULL, length, out_atoms);

	sysmem_freeptr(out_atoms);
}
void rotateSlider_resize_values(t_rotateSlider* x, short size)
{
	if (size != x->values_ac) {
		if (x->values)
			sysmem_freeptr(x->values);
		if (size)
			x->values = (t_atom_float*)sysmem_newptr(size * sizeof(t_atom_float));
		else
			x->values = NULL;
		x->values_ac = size;
	}
}




void rotateSlider_update(t_rotateSlider* x, t_symbol* s, short ac, t_atom* av)
{
	t_atom_float* input = (t_atom_float*)sysmem_newptr(ac * sizeof(t_atom_float));
	atom_getdouble_array(ac, av, ac, input);

	if (arrayEquals(ac, input, x->values_ac, x->values)) {
		return;
	}
	else {
		x->values_ac = ac;
		x->values = input;
		rotateSlider_outputView(x);
	}
}
bool arrayEquals(short ac, t_atom_float* av, short bc, t_atom_float* bv)
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