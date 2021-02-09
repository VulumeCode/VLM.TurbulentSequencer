#include "ext.h"				// you must include this - it contains the external object's link to available Max functions
#include "ext_obex.h"			// this is required for all objects using the newer style for writing objects.

typedef struct _findnotes {		// defines our object's internal variables for each instance in a patch
	t_object i_ob;
	long i_in;          // space for the inlet number used by all the proxies
	void* i_proxy;

	t_atom_float* probs;
	short probs_ac;

	t_atom_float* atoms;
	short atoms_ac;

	int nudge;

} t_findnotes;

// these are prototypes for the methods that are defined below
void *findnotes_new();
void findnotes_free(t_findnotes *x);
void findnotes_assist(t_findnotes *x, void *b, long m, long a, char *s);
void findnotes_bang(t_findnotes *x);
void findnotes_setNudge(t_findnotes* x, long nudge);
void findnotes_list(t_findnotes* x, t_symbol* s, short ac, t_atom* av);
void findnotes_atoms(t_findnotes* x, t_symbol* s, short ac, t_atom* av);
void findnotes_probs(t_findnotes* x, t_symbol* s, short ac, t_atom* av);
void findnotes_resize_atoms(t_findnotes* x, short size);
void findnotes_resize_probs(t_findnotes* x, short size);


t_class *findnotes_class;		// global pointer to the object class - so max can reference the object

//--------------------------------------------------------------------------

void ext_main(void *r)
{
	t_class *c;

	c = class_new("findnotes", (method)findnotes_new, (method)findnotes_free, sizeof(t_findnotes), 0L, 0);

	class_addmethod(c, (method)findnotes_bang,	"bang",		0);
	class_addmethod(c, (method)findnotes_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)findnotes_setNudge, "setNudge", A_LONG, 0);
	
	class_addmethod(c, (method)findnotes_assist,	"assist",	A_CANT, 0);	

	class_register(CLASS_BOX, c);
	findnotes_class = c;

	object_post(NULL, "findnotes object loaded...",0);	// post any important info to the max window when our class is loaded
}

//--------------------------------------------------------------------------

void *findnotes_new()
{
	t_findnotes* x;

	x = object_alloc(findnotes_class);

	x->i_proxy = proxy_new((t_object*)x, 1, &x->i_in);

	listout(x);
	return x;
}

void findnotes_free(t_findnotes *x)
{
	if (x->probs)
		sysmem_freeptr(x->probs);
	if (x->atoms)
		sysmem_freeptr(x->atoms);
}

//--------------------------------------------------------------------------

void findnotes_assist(t_findnotes *x, void *b, long m, long a, char *s) // 4 final arguments are always the same for the assistance method
{
	if (m == ASSIST_INLET)
		switch (a) {
		case 0:
			sprintf(s, "note list");
			break;
		case 1:
			sprintf(s, "prob list");
			break;
		}
	else
		sprintf(s, "weighted notes.");
}

void findnotes_setNudge(t_findnotes* x, long nudge) {
	x->nudge = nudge;
	findnotes_bang(x);
}

void findnotes_bang(t_findnotes *x)
{
	if (x->atoms_ac > 0) {
		t_linklist* floats = linklist_new(); linklist_flags(floats, OBJ_FLAG_MEMORY);
		for (int i = 0; i < x->atoms_ac; i++) {
			t_atom_float* a = (t_atom_float*)sysmem_newptr(sizeof(t_atom_float));
			memcpy(a, x->atoms + i, sizeof(t_atom_float));
			linklist_append(floats, a);
		}
		linklist_reverse(floats);
		linklist_rotate(floats, (x->nudge));

		t_linklist* noteList = linklist_new(); linklist_flags(noteList, OBJ_FLAG_MEMORY);

		for (int i = 0; i < linklist_getsize(floats); i++) {
			t_atom_float v = *(t_atom_float*)linklist_getindex(floats, i);
			for (int j = 0; j < x->probs_ac; j++) {
				if ((v < 1.) ? (v < x->probs[j]) : (v == x->probs[j])) {

					int* a = (int*)sysmem_newptr(sizeof(int));
					memcpy(a, &j, sizeof(int));
					linklist_append(noteList, a);

					break;
				}
			}
		}

		short output_ac = (short) linklist_getsize(noteList);
		t_atom* output = (t_atom*)sysmem_newptr((short) linklist_getsize(noteList) * sizeof(t_atom));
		for (short i = 0; i < output_ac; i++) {
			atom_setlong(
				output + i,
				*(int*)linklist_getindex(noteList, i)
			);
		}
		outlet_list(x->i_ob.o_outlet, NULL, output_ac, output);
		sysmem_freeptr(output);
		object_free(noteList);
		object_free(floats);
	}
}

void findnotes_list(t_findnotes* x, t_symbol* s, short ac, t_atom* av) {
	switch (proxy_getinlet((t_object*)x)) {
	case 0:
		findnotes_atoms(x, s, ac, av);
		break;
	case 1:
		findnotes_probs(x, s, ac, av);
		break;
	}
	findnotes_bang(x);
}


void findnotes_atoms(t_findnotes* x, t_symbol* s, short ac, t_atom* av)
{
	findnotes_resize_atoms(x, ac);
	for (short i = 0; i < ac; i++, av++) {
		x->atoms[i] = atom_getfloat(av);
	 }
}
void findnotes_resize_atoms(t_findnotes* x, short size)
{
	if (size!=x->atoms_ac) {
		if (x->atoms)
			sysmem_freeptr(x->atoms);
		if (size)
			x->atoms = (t_atom_float*)sysmem_newptr(size*sizeof(t_atom_float));
		else
			x->atoms = NULL;
		x->atoms_ac = size;
	}
}

void findnotes_probs(t_findnotes* x, t_symbol* s, short ac, t_atom* av)
{
	findnotes_resize_probs(x, ac);
	for (short i = 0; i < ac; i++, av++) {
		x->probs[i] = atom_getfloat(av);
	 }
}
void findnotes_resize_probs(t_findnotes* x, short size)
{
	if (size!=x->probs_ac) {
		if (x->probs)
			sysmem_freeptr(x->probs);
		if (size)
			x->probs = (t_atom_float*)sysmem_newptr(size * sizeof(t_atom_float));
		else
			x->probs = NULL;
		x->probs_ac = size;
	}
}
