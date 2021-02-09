#include "ext.h"				// you must include this - it contains the external object's link to available Max functions
#include "ext_obex.h"			// this is required for all objects using the newer style for writing objects.

#define viewLength 12

typedef struct _rotatePiano {		// defines our object's internal variables for each instance in a patch
	t_object i_ob;
	long i_in;          // space for the inlet number used by all the proxies
	void* i_proxy;

	t_atom_long noteToggles[viewLength];
	t_atom_long noteView[viewLength];
	t_atom_long noteViewPrev[viewLength];

	int steps;

	void* out0;
	void* out1;
	void* out2;
} t_rotatePiano;

// these are prototypes for the methods that are defined below
void *rotatePiano_new(void);
void rotatePiano_free(t_rotatePiano *x);
void rotatePiano_assist(t_rotatePiano *x, void *b, long m, long a, char *s);
void rotatePiano_setSteps(t_rotatePiano* x, long steps);
void rotatePiano_setPiano(t_rotatePiano* x);
void rotatePiano_list(t_rotatePiano* x, t_symbol* s, short ac, t_atom* av);
void rotatePiano_rotate(t_rotatePiano* x, t_symbol* s, short ac, t_atom* av);
void rotatePiano_update(t_rotatePiano* x, t_symbol* s, short ac, t_atom* av);
void rotatePiano_calcView(t_rotatePiano* x);
bool arrayEquals(short ac, t_atom_long* av, short bc, t_atom_long* bv);

t_class* rotatePiano_class;		// global pointer to the object class - so max can reference the object

//--------------------------------------------------------------------------

void ext_main(void* r)
{
	t_class* c;

	c = class_new("rotatePiano", (method)rotatePiano_new, (method)rotatePiano_free, sizeof(t_rotatePiano), 0L, 0);

	class_addmethod(c, (method)rotatePiano_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)rotatePiano_setSteps, "in1", A_LONG, 0);

	class_addmethod(c, (method)rotatePiano_assist, "assist", A_CANT, 0);

	class_register(CLASS_BOX, c);
	rotatePiano_class = c;

	object_post(NULL, "rotatePiano object loaded...", 0);	// post any important info to the max window when our class is loaded
}

//--------------------------------------------------------------------------

void* rotatePiano_new()
{
	t_rotatePiano* x;

	x = object_alloc(rotatePiano_class);

	x->i_proxy = proxy_new(&x->i_ob, 2, &x->i_in);
	intin(x, 1);

	x->steps = 0;

	for (int i = 0; i < viewLength; i++) {
		x->noteToggles[i] = 1;
		x->noteView[i] = 1;
		x->noteViewPrev[i] = -1;
	}

	x->out2 = listout(x);
	x->out1 = listout(x);
	x->out0 = listout(x);
	return x;
}

void rotatePiano_free(t_rotatePiano* x)
{
	sysmem_freeptr(x->noteToggles);
	sysmem_freeptr(x->noteView);
	sysmem_freeptr(x->noteViewPrev);

	freeobject((t_object*)x->i_proxy);
}

//--------------------------------------------------------------------------

void rotatePiano_assist(t_rotatePiano* x, void* b, long m, long a, char* s) // 4 final arguments are always the same for the assistance method
{
	if (m == ASSIST_INLET) {
		switch (a) {
		case 0:
			sprintf(s, "paino to rotate");
			break;
		case 1:
			sprintf(s, "rotation steps");
			break;
		case 2:
			sprintf(s, "piano values");
			break;
		}
	}
	else {
		switch (a) {
		case 0:
			sprintf(s, "rotated piano");
			break;
		case 1:
			sprintf(s, "updated values");
			break;
		case 2:
			sprintf(s, "updated toggles");
			break;
		}
	}
}

void rotatePiano_setSteps(t_rotatePiano* x, long steps) {
	x->steps = (-steps + viewLength) % viewLength;
	rotatePiano_setPiano(x);
}


bool arrayEquals(short ac, t_atom_long* av, short bc, t_atom_long* bv)
{
	if (av == NULL || bv == NULL || ac != bc) {
		return false;
	}
	for (int i = 0; i < ac; i++) {
		t_atom_long av_i = av[i];
		t_atom_long bv_i = bv[i];
		if (av_i != bv_i) {
			return false;
		}
	}
	return true;
}


void rotatePiano_list(t_rotatePiano* x, t_symbol* s, short ac, t_atom* av) {
	switch (proxy_getinlet(&x->i_ob)) {
	case 0:
		rotatePiano_rotate(x, s, ac, av);
		break;
	case 1:
		error("No list here pls");
		break;
	case 2:
		rotatePiano_update(x, s, ac, av);
		break;
	}
}


void rotatePiano_rotate(t_rotatePiano* x, t_symbol* s, short ac, t_atom* av)
{
	t_atom_long in_0 = atom_getlong(av + 0);
	t_atom_long i = (in_0 + x->steps) % viewLength;
	t_atom_long v = (atom_getlong(av + 1) == 0 ? 1 : 0);

	x->noteViewPrev[in_0] = v;

	if (x->noteToggles[i] == v) {
		return;
	}
	else {
		x->noteToggles[i] = v;

		t_atom noteToggles_out[viewLength];
		atom_setlong_array(viewLength, noteToggles_out, viewLength, x->noteToggles);
		outlet_list(x->out2, NULL, viewLength, noteToggles_out);

		rotatePiano_calcView(x);

		t_atom noteView_out[viewLength];
		atom_setlong_array(viewLength, noteView_out, viewLength, x->noteView);
		outlet_list(x->out1, NULL, viewLength, noteView_out);
	}
}



void rotatePiano_update(t_rotatePiano* x, t_symbol* s, short ac, t_atom* av)
{
	t_atom_long updatedValues[viewLength];
	atom_getlong_array(viewLength, av, viewLength, updatedValues);

	if (arrayEquals(viewLength, x->noteToggles, viewLength, updatedValues)) {
		return;
	}
	else {
		memcpy(x->noteToggles, updatedValues, viewLength * sizeof(t_atom_long));
		rotatePiano_setPiano(x);
	}
}



void rotatePiano_calcView(t_rotatePiano* x) {
	short steps = ((x->steps) + viewLength) % viewLength;
	short to_i = 0;
	for (short i = steps; i < viewLength; i++, to_i++) {
		x->noteView[to_i] = x->noteToggles[i];
	}
	for (short i = 0; i < steps; i++, to_i++) {
		x->noteView[to_i] = x->noteToggles[i];
	}
}

void rotatePiano_setPiano(t_rotatePiano* x)
{
	rotatePiano_calcView(x);

	for (int i = 0; i < viewLength; i++) {
		if (x->noteView[i] != x->noteViewPrev[i]) {
			t_atom set_out[2];
			atom_setlong(set_out + 0, i);
			atom_setlong(set_out + 1, x->noteView[i] != 0 ? 0 : 1);
			outlet_anything(x->out0, gensym("set"), 2, set_out);
		}
	}

	memcpy(x->noteViewPrev, x->noteView, viewLength * sizeof(t_atom_long));

	t_atom noteView_out[viewLength];
	atom_setlong_array(viewLength, noteView_out, viewLength, x->noteView);
	outlet_list(x->out1, NULL, viewLength, noteView_out);
}