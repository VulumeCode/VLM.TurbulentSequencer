#include "ext.h"				// you must include this - it contains the external object's link to available Max functions
#include "ext_obex.h"			// this is required for all objects using the newer style for writing objects.

typedef struct _rotate {		// defines our object's internal variables for each instance in a patch
	t_object i_ob;
	long i_in;          // space for the inlet number used by all the proxies
	void* proxy2;
	void* proxy1;

	double angle;
	short length;
	double* seqReal;
	double* seqImg;

	void* out0;
	void* out1;
} t_rotate;

// these are prototypes for the methods that are defined below
void* rotate_new(t_symbol* s, short ac, t_atom* av);
void rotate_free(t_rotate* x);
void rotate_assist(t_rotate* x, void* b, long m, long a, char* s);
void rotate_list(t_rotate* x, t_symbol* s, short ac, t_atom* av);
void rotate_float(t_rotate* x, double d);
void rotate_bang(t_rotate* x);

t_class* rotate_class;		// global pointer to the object class - so max can reference the object

//--------------------------------------------------------------------------

void ext_main(void* r)
{
	t_class* c;

	c = class_new("rotate", (method)rotate_new, (method)rotate_free, sizeof(t_rotate), 0L, A_GIMME, 0);

	class_addmethod(c, (method)rotate_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)rotate_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)rotate_bang, "bang", 0);


	class_addmethod(c, (method)rotate_assist, "assist", A_CANT, 0);

	class_register(CLASS_BOX, c);
	rotate_class = c;

	object_post(NULL, "rotate object loaded...", 0);	// post any important info to the max window when our class is loaded
}

//--------------------------------------------------------------------------

void* rotate_new(t_symbol* s, short ac, t_atom* av)
{
	t_rotate* x;

	x = object_alloc(rotate_class);

	x->proxy2 = proxy_new(&x->i_ob, 2, &x->i_in);
	x->proxy1 = proxy_new(&x->i_ob, 1, &x->i_in);

	x->out1 = listout(x);
	x->out0 = listout(x);

	return x;
}

void rotate_free(t_rotate* x)
{
	sysmem_freeptr(x->seqReal);
	sysmem_freeptr(x->seqImg);

	freeobject((t_object*)x->proxy1);
	freeobject((t_object*)x->proxy2);
}

//--------------------------------------------------------------------------

void rotate_assist(t_rotate* x, void* b, long m, long a, char* s) // 4 final arguments are always the same for the assistance method
{
	if (m == ASSIST_INLET) {
		switch (a) {
		case 0:
			sprintf(s, "reals");
			break;
		case 1:
			sprintf(s, "imgs");
			break;
		case 2:
			sprintf(s, "angle");
			break;
		}
	}
	else {
		switch (a) {
		case 0:
			sprintf(s, "reals");
			break;
		case 1:
			sprintf(s, "imgs");
			break;
		}
	}
}

void rotate_bang(t_rotate* obj) {
	if (obj->length > 0 && obj->seqReal != NULL && obj->seqImg != NULL) {
		int len = obj->length;

		t_atom* outSeqReal = (t_atom*)sysmem_newptr(len * sizeof(t_atom));
		t_atom* outSeqImg   = (t_atom*)sysmem_newptr(len * sizeof(t_atom));

		for (int i = 0; i < len; i++) {
			double x = obj->seqReal[i];
			double y = obj->seqImg[i];

			double t = atan2(y, x);
			double r = sqrt((x * x) + (y * y));

			x = r * cos(t - obj->angle);
			y = r * sin(t - obj->angle);

			atom_setfloat(outSeqReal + i, x);
			atom_setfloat(outSeqImg + i, y);
		}

		outlet_list(obj->out1, NULL, len, outSeqImg);
		outlet_list(obj->out0, NULL, len, outSeqReal);

		sysmem_freeptr(outSeqReal);
		sysmem_freeptr(outSeqImg);
	}
}

void rotate_list(t_rotate* x, t_symbol* s, short ac, t_atom* av) {
	switch (proxy_getinlet(&x->i_ob)) {
	case 0:
		sysmem_freeptr(x->seqReal);
		x->seqReal = (double*)sysmem_newptr(ac * sizeof(double));
		x->length = ac;
		atom_getdouble_array(ac, av, ac, x->seqReal);
		rotate_bang(x);
		break;
	case 1:
		sysmem_freeptr(x->seqImg);
		x->seqImg = (double*)sysmem_newptr(ac * sizeof(double));
		x->length = ac;
		atom_getdouble_array(ac, av, ac, x->seqImg);
		break;
	}
}

void rotate_float(t_rotate* x, double d) {
	switch (proxy_getinlet(&x->i_ob)) {
	case 2:
		x->angle = d;
		rotate_bang(x);
		break;
	}
}