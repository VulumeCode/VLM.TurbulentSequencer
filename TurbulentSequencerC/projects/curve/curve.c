#include "ext.h"				// you must include this - it contains the external object's link to available Max functions
#include "ext_obex.h"			// this is required for all objects using the newer style for writing objects.

typedef struct _curve {		// defines our object's internal variables for each instance in a patch
	t_object i_ob;
	long i_in;          // space for the inlet number used by all the proxies
	void* proxy2;
	void* proxy1;

	double beta;
	double centerpct;
	short len;

	void* out0;
} t_curve;

// these are prototypes for the methods that are defined below
void *curve_new(long dummy);
void curve_free(t_curve *x);
void curve_assist(t_curve *x, void *b, long m, long a, char *s);
void curve_bang(t_curve *x);
void curve_int(t_curve *x, long n);
void curve_float(t_curve* x, double f);

t_class *curve_class;		// global pointer to the object class - so max can reference the object

//--------------------------------------------------------------------------

void ext_main(void *r)
{
	t_class *c;

	c = class_new("curve", (method)curve_new, (method)curve_free, sizeof(t_curve), 0L, A_DEFLONG, 0);

	class_addmethod(c, (method)curve_float, "float", A_FLOAT, 0);		
	class_addmethod(c, (method)curve_int, "int", A_LONG, 0);
	class_addmethod(c, (method)curve_bang, "bang", A_LONG, 0);

	class_addmethod(c, (method)curve_assist,	"assist",	A_CANT, 0);	

	class_register(CLASS_BOX, c);
	curve_class = c;

	object_post(NULL, "curve object loaded...",0);	// post any important info to the max window when our class is loaded
}

//--------------------------------------------------------------------------

void *curve_new(long dummy)		// dummy = int argument typed into object box (A_DEFLONG) -- defaults to 0 if no args are typed
{
	t_curve* x;

	x = object_alloc(curve_class);

	x->proxy2 = proxy_new(&x->i_ob, 2, &x->i_in);
	x->proxy1 = proxy_new(&x->i_ob, 1, &x->i_in);

	x->beta  = -1;
	x->centerpct = -1;
	x->len =0;

	x->out0 = listout(x);
	return x;
}

void curve_free(t_curve *x)
{
	freeobject((t_object*)x->proxy1);
	freeobject((t_object*)x->proxy2);
}

//--------------------------------------------------------------------------

void curve_assist(t_curve *x, void *b, long m, long a, char *s) // 4 final arguments are always the same for the assistance method
{
	if (m == ASSIST_INLET) {
		switch (a) {
		case 0:
			sprintf(s, "Sequenced data input");
			break;
		case 1:
			sprintf(s, "Velocity scale");
			break;
		}
	}
	else {
		sprintf(s, "MIDI note.");
	}
}


void curve_float(t_curve* x, double n)
{
	switch (proxy_getinlet(&x->i_ob)) {
	case 0:
		x->beta = n == 10.0 ? 100.0 : n;
		break;
	case 1:
		x->centerpct = n;
		break;
	case 2:
		error("no float here pls");
		break;
	}
	curve_bang(x);
}



void curve_int(t_curve *x, long n)
{
	switch (proxy_getinlet(&x->i_ob)) {
	case 0:
		error("no int here pls");
		break;
	case 1:
		error("must be float");
		break;
	case 2:
		x->len = (short) n;
		break;
	}
	curve_bang(x);
}

void curve_bang(t_curve* x) {
	if (x->len > 0 && x->centerpct >= 0 && x->beta >= 0) {
		t_atom* curve = (t_atom*)sysmem_newptr(x->len * sizeof(t_atom));

		double max = 0;

		double center = x->centerpct * x->len;

		int i = 1;
		for (; i < center; i++) {
			double e = x->len * ((center - i) / center);
			double s = (double)(1. / pow(e, x->beta));
			max = s > max ? s : max;
			atom_setfloat(
				&curve[i], s
			);
		}

		for (; i < x->len; i++) {
			double e = i - center + 1;
			double s = (double)(1. / pow(e, x->beta));
			max = s > max ? s : max;
			atom_setfloat(
				&curve[i], s
			);
		}

		for (int j = 1; j < x->len; j++) {
			atom_setfloat(
				&curve[j], atom_getfloat(&curve[j]) / max
			);
		}

		atom_setfloat(
			&curve[0], 1.
		);

		outlet_list(x->out0, NULL, x->len, curve);
		sysmem_freeptr(curve);
	}
}


