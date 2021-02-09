#include "ext.h"				// you must include this - it contains the external object's link to available Max functions
#include "ext_obex.h"			// this is required for all objects using the newer style for writing objects.

#define nrChoices 3
#define nrOptions 3
#define nrStages 3

typedef struct _findTiesAndAccents {		// defines our object's internal variables for each instance in a patch
	t_object i_ob;
	long i_in;          // space for the inlet number used by all the proxies
	void* i_proxy;

	char** choices;

	double* floats;
	short floats_ac;

	double bot;
	double top;

	int nudge;

	void* out0;
	void* out1;
} t_findTiesAndAccents;

// these are prototypes for the methods that are defined below
void *findTiesAndAccents_new(t_symbol* s, short ac, t_atom* av);
void findTiesAndAccents_free(t_findTiesAndAccents *x);
void findTiesAndAccents_assist(t_findTiesAndAccents *x, void *b, long m, long a, char *s);
void findTiesAndAccents_bang(t_findTiesAndAccents *x);
void findTiesAndAccents_setChoices(t_findTiesAndAccents* x, t_symbol* s, short ac, t_atom* av);
void findTiesAndAccents_setBot(t_findTiesAndAccents* x, double d);
void findTiesAndAccents_setTop(t_findTiesAndAccents* x, double d);
void findTiesAndAccents_setNudge(t_findTiesAndAccents* x, long l);
void findTiesAndAccents_list(t_findTiesAndAccents* x, t_symbol* s, short ac, t_atom* av);
void findTiesAndAccents_resize_floats(t_findTiesAndAccents* x, short size);

t_class *findTiesAndAccents_class;		// global pointer to the object class - so max can reference the object

//--------------------------------------------------------------------------

void ext_main(void *r)
{
	t_class *c;

	c = class_new("findTiesAndAccents", (method)findTiesAndAccents_new, (method)findTiesAndAccents_free, sizeof(t_findTiesAndAccents), 0L, A_GIMME, 0);

	class_addmethod(c, (method)findTiesAndAccents_bang,	"bang",		0);
	class_addmethod(c, (method)findTiesAndAccents_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)findTiesAndAccents_setChoices, "setChoices", A_GIMME, 0);
	class_addmethod(c, (method)findTiesAndAccents_setBot, "setBot", A_FLOAT, 0);
	class_addmethod(c, (method)findTiesAndAccents_setTop, "setTop", A_FLOAT, 0);
	class_addmethod(c, (method)findTiesAndAccents_setNudge, "setNudge", A_LONG, 0);
	
	class_addmethod(c, (method)findTiesAndAccents_assist,	"assist",	A_CANT, 0);	

	class_register(CLASS_BOX, c);
	findTiesAndAccents_class = c;

	object_post(NULL, "findTiesAndAccents object loaded...",0);	// post any important info to the max window when our class is loaded
}

//--------------------------------------------------------------------------

void *findTiesAndAccents_new(t_symbol* s, short ac, t_atom* av)
{
	t_findTiesAndAccents* x;

	x = object_alloc(findTiesAndAccents_class);

	x->i_proxy = proxy_new((t_object*)x, 1, &x->i_in);

	x->choices = (char**)sysmem_newptr(nrChoices * sizeof(char*));
	for (int i = 0; i < nrChoices; i++) {
		*(x->choices + 1) = (char*)sysmem_newptr(nrOptions * sizeof(char));
	}


	x->floats_ac = 0;
	x->nudge = 0;
	x->bot = 0;
	x->top = 0;

	x->out1 = listout(x);
	x->out0 = listout(x);
	return x;
}

void findTiesAndAccents_free(t_findTiesAndAccents *x)
{
	if (x->choices)
		sysmem_freeptr(x->choices);
	if (x->floats)
		sysmem_freeptr(x->floats);
}

//--------------------------------------------------------------------------

void findTiesAndAccents_assist(t_findTiesAndAccents *x, void *b, long m, long a, char *s) // 4 final arguments are always the same for the assistance method
{
	if (m == ASSIST_INLET)
		switch (a) {
		case 0:
			sprintf(s, "probs");
			break;
		case 1:
			sprintf(s, "messages");
			break;
		}
	else
		sprintf(s, "weighted notes.");
}

void findTiesAndAccents_list(t_findTiesAndAccents* x, t_symbol* s, short ac, t_atom* av){
	findTiesAndAccents_resize_floats(x, ac);
	atom_getdouble_array(ac, av, ac, x->floats);
	findTiesAndAccents_bang(x);
}
void findTiesAndAccents_resize_floats(t_findTiesAndAccents* x, short size)
{
	if (size != x->floats_ac) {
		if (x->floats)
			sysmem_freeptr(x->floats);
		if (size)
			x->floats = (double*)sysmem_newptr(size * sizeof(double));
		else
			x->floats = NULL;
		x->floats_ac = size;
	}
}

void findTiesAndAccents_setChoices(t_findTiesAndAccents* x, t_symbol* s, short ac, t_atom* av) {
	for (int i = 0; i < nrChoices; i++) {
		t_symbol* sym = atom_getsym(av + i);
		*(x->choices + i) = sym->s_name;
	}
	findTiesAndAccents_bang(x);
}

void findTiesAndAccents_setBot(t_findTiesAndAccents* x, double bot) {
	x->bot = bot;
	findTiesAndAccents_bang(x);
}
void findTiesAndAccents_setTop(t_findTiesAndAccents* x, double top) {
	x->top = top;
	findTiesAndAccents_bang(x);
}
void findTiesAndAccents_setNudge(t_findTiesAndAccents* x, long nudge) {
	x->nudge = nudge;
	findTiesAndAccents_bang(x);
}

void findTiesAndAccents_bang(t_findTiesAndAccents *x)
{
	if (x->floats_ac > 0 && x->floats != NULL && x->choices != NULL) {
		double stages[nrStages] = { 
			nextafter(x->bot, -INFINITY),
			nextafter(1 + x->top, INFINITY),
			INFINITY };

		t_atom* durations = (t_atom * )sysmem_newptr((1 + x->floats_ac) * sizeof(t_atom));
		atom_setlong(durations + 0, 1);
		t_atom * velocities = (t_atom * )sysmem_newptr((1 + x->floats_ac) * sizeof(t_atom));
		atom_setlong(velocities + 0, 1);
		t_atom * accents = (t_atom * )sysmem_newptr((x->floats_ac) * sizeof(t_atom));


		t_linklist* floats = linklist_new(); linklist_flags(floats, OBJ_FLAG_MEMORY);
		for (int i = 0; i < x->floats_ac; i++) {
			double* a = (double*)sysmem_newptr(sizeof(double));
			memcpy(a, x->floats + i, sizeof(double));
			linklist_append(floats, a);
		}
		linklist_rotate(floats, (x->nudge));
		
		for (int i = 0; i < linklist_getsize(floats); i++) {
			int j;
			double f = *(double*)linklist_getindex(floats, i);
			for (j = 0; j < nrStages; j++) {
				if (f<stages[j]) {
					break;
				}
			}

			char* choice = x->choices[j];

			atom_setlong(velocities + (i + 1), strstr(choice, "R") ? 0 : strstr(choice, "A") ? 127 : 63);
			atom_setlong(accents + (i), strstr(choice, "A") ? 1 : 0);
			atom_setlong(durations + (i + 1), strstr(choice, "T") ? 120 : 60);
		}

		outlet_anything(x->out0, gensym("duration"), 1 + x->floats_ac, durations);
		outlet_anything(x->out0, gensym("velocity"), 1 + x->floats_ac, velocities);

		outlet_list(x->out1, NULL, x->floats_ac, accents);

		sysmem_freeptr(durations);
		sysmem_freeptr(velocities);
		sysmem_freeptr(accents);
	}
}