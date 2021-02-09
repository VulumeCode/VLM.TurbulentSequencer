#include "ext.h"				// you must include this - it contains the external object's link to available Max functions
#include "ext_obex.h"			// this is required for all objects using the newer style for writing objects.

typedef struct _rotary {		// defines our object's internal variables for each instance in a patch
	t_object i_ob;
	long i_in;          // space for the inlet number used by all the proxies
	void* proxy2;
	void* proxy1;

	double from;
	double to;
	double in;

	void* out0;
	void* out1;
} t_rotary;

// these are prototypes for the methods that are defined below
void *rotary_new(t_symbol* s, short ac, t_atom* av);
void rotary_free(t_rotary *x);
void rotary_assist(t_rotary *x, void *b, long m, long a, char *s);
double rotary_wrap(t_rotary *x);
void rotary_int(t_rotary *x, long n);
void rotary_float(t_rotary* x, double f);

t_class *rotary_class;		// global pointer to the object class - so max can reference the object

//--------------------------------------------------------------------------

void ext_main(void *r)
{
	t_class *c;

	c = class_new("rotary", (method)rotary_new, (method)rotary_free, sizeof(t_rotary), 0L, A_GIMME, 0);

	class_addmethod(c, (method)rotary_float, "float", A_FLOAT, 0);		
	class_addmethod(c, (method)rotary_int, "int", A_LONG, 0);

	class_addmethod(c, (method)rotary_assist,	"assist",	A_CANT, 0);	

	class_register(CLASS_BOX, c);
	rotary_class = c;

	object_post(NULL, "rotary object loaded...",0);	// post any important info to the max window when our class is loaded
}

//--------------------------------------------------------------------------

void *rotary_new(t_symbol* s, short ac, t_atom* av)
{
	t_rotary* x;

	x = object_alloc(rotary_class);

	x->proxy2 = proxy_new(&x->i_ob, 2, &x->i_in);
	x->proxy1 = proxy_new(&x->i_ob, 1, &x->i_in);

	if (ac == 2) {
		x->from = atom_getfloat(av);
		x->to = atom_getfloat(av+1);
	} else 
	if (ac == 1) {
		x->from = -atom_getfloat(av);
		x->to = atom_getfloat(av);
	} else 
	if (ac == 0) {
		x->from = 0;
		x->to = 128;
	}

	x->out1 = outlet_new(x, NULL);
	x->out0 = outlet_new(x, NULL);
	return x;
}

void rotary_free(t_rotary *x)
{
	freeobject((t_object*)x->proxy1);
	freeobject((t_object*)x->proxy2);
}

//--------------------------------------------------------------------------

void rotary_assist(t_rotary *x, void *b, long m, long a, char *s) // 4 final arguments are always the same for the assistance method
{
	if (m == ASSIST_INLET) {
		switch (a) {
		case 0:
			sprintf(s, "input");
			break;
		case 1:
			sprintf(s, "from");
			break;	
		case 2:
			sprintf(s, "to");
			break;
		}
	}
	else {
		switch (a) {
		case 0:
			sprintf(s, "set rotary");
			break;
		case 1:
			sprintf(s, "output value");
			break;
		}
	}
}

void rotary_int(t_rotary* x, long v)
{
	switch (proxy_getinlet(&x->i_ob)) {
	case 0:
		x->in = v;
		break;
	case 1:
		x->from = v;
		break;
	case 2:
		x->to = v;
		break;
	}
	if (x->in > x->to || x->in < x->from) {
		x->in = rotary_wrap(x);
		t_atom* output = (t_atom*)sysmem_newptr(sizeof(t_atom));
		atom_setlong(output, (long) x->in);
		outlet_anything(x->out0, gensym("set"), 1, output);
	}
	outlet_int(x->out1, (long) x->in);
}

void rotary_float(t_rotary* x, double v)
{
	switch (proxy_getinlet(&x->i_ob)) {
	case 0:
		x->in = v;
		break;
	case 1:
		x->from = v;
		break;
	case 2:
		x->to = v;
		break;
	}
	if (x->in > x->to || x->in < x->from) {
		x->in = rotary_wrap(x);
		t_atom* output = (t_atom*)sysmem_newptr(sizeof(t_atom));
		atom_setfloat(output, x->in);
		outlet_anything(x->out0, gensym("set"), 1, output);
	}
	outlet_float(x->out1, x->in);
}

double rotary_wrap(t_rotary* x)
{
	if (x->in == x->from) {
		return x->to;
	}
	else if (x->in == x->to) {
		return x->from;
	}
	else if (x->in >= x->to) {
		x->in = (x->in - (x->to - x->from));
		return rotary_wrap(x);
	}
	else if (x->in <= x->from) {
		x->in = (x->in + (x->to - x->from));
		return rotary_wrap(x);
	}
	else {
		return x->in;
	}
}
