#include "ext.h"				// you must include this - it contains the external object's link to available Max functions
#include "ext_obex.h"			// this is required for all objects using the newer style for writing objects.

typedef struct _normalize {		// defines our object's internal variables for each instance in a patch
	t_object i_ob;
	t_atom_float* input;
	short i_ac;
} t_normalize;

// these are prototypes for the methods that are defined below
void *normalize_new(t_symbol* s, short ac, t_atom* av);
void normalize_free(t_normalize *x);
void normalize_assist(t_normalize *x, void *b, long m, long a, char *s);
void normalize_bang(t_normalize *x);
void normalize_list(t_normalize* x, t_symbol* s, short ac, t_atom* av);
void normalize_resize(t_normalize* x, short size);


t_class *normalize_class;		// global pointer to the object class - so max can reference the object

//--------------------------------------------------------------------------

void ext_main(void *r)
{
	t_class *c;

	c = class_new("normalize", (method)normalize_new, (method)normalize_free, sizeof(t_normalize), 0L, A_GIMME, 0);

	class_addmethod(c, (method)normalize_bang,	"bang",		0);				
	class_addmethod(c, (method)normalize_list,		"list", A_GIMME, 0);		
	class_addmethod(c, (method)normalize_assist,	"assist",	A_CANT, 0);	

	class_register(CLASS_BOX, c);
	normalize_class = c;

	object_post(NULL, "Normalize object loaded...",0);	// post any important info to the max window when our class is loaded
}

//--------------------------------------------------------------------------

void *normalize_new(t_symbol* s, short ac, t_atom* av)
{
	t_normalize* x;

	x = object_alloc(normalize_class);
	x->i_ac = 0;
	x->input = NULL;
	listout(x);
	return x;
}

void normalize_free(t_normalize *x)
{
	if (x->input)
		sysmem_freeptr(x->input);
}

//--------------------------------------------------------------------------

void normalize_assist(t_normalize *x, void *b, long m, long a, char *s) // 4 final arguments are always the same for the assistance method
{
	if (m == ASSIST_INLET)
		sprintf(s, "List to be normalized.");
	else
		sprintf(s, "Normalized input.");
}


void normalize_bang(t_normalize *x)
{
	double sum = 0;
	double min = INFINITY;
	double max = -INFINITY;

	short length = x->i_ac;

	for (short i = 0; i < length; i++) {
		double d = x->input[i];
		sum += d;
		if (d < min) {
			min = d;
		}
		else if (d > max) {
			max = d;
		}
	}
	double avg = sum / length;
	double deviation = max(max - avg, avg - min);

	t_atom* output = (t_atom*) sysmem_newptr(length * sizeof(t_atom));
	for (short i = 0; i < length; i++) {
		atom_setfloat(
			output + i,
			(((x->input[i] - avg) / deviation) + 1) / 2
		);
	}
	outlet_list(x->i_ob.o_outlet, NULL, length, output);
	sysmem_freeptr(output);
}

void normalize_list(t_normalize* x, t_symbol* s, short ac, t_atom* av)
{
	short i;

	normalize_resize(x, ac);
	for (i = 0; i < ac; i++, av++) {
		if (atom_gettype(av) == A_FLOAT) {
			x->input[i] = atom_getfloat(av);
		}
		else {
			object_error(&x->i_ob, "Not a float");
		}
	}
	x->i_ac = ac;
	normalize_bang(x);
}

void normalize_resize(t_normalize* x, short size)
{
	if (size!=x->i_ac) {
		if (x->input)
			sysmem_freeptr(x->input);
		if (size)
			x->input = (t_atom_float *)sysmem_newptr(size*sizeof(t_atom_float));
		else
			x->input = NULL;
		x->i_ac = size;
	}
}
