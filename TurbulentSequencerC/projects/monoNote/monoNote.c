#include "ext.h"				// you must include this - it contains the external object's link to available Max functions
#include "ext_obex.h"			// this is required for all objects using the newer style for writing objects.

typedef struct _monoNote {		// defines our object's internal variables for each instance in a patch
	t_object i_ob;
	long lastNote;
	bool lastTie;
	double velScale;
} t_monoNote;

// these are prototypes for the methods that are defined below
void *monoNote_new(t_symbol* s, short ac, t_atom* av);
void monoNote_free(t_monoNote *x);
void monoNote_assist(t_monoNote *x, void *b, long m, long a, char *s);
void monoNote_int(t_monoNote *x, long n);
void monoNote_list(t_monoNote* x, t_symbol* s, short ac, t_atom* av);

void noteOut(t_monoNote* x, long note, long vel);

t_class *monoNote_class;		// global pointer to the object class - so max can reference the object

//--------------------------------------------------------------------------

void ext_main(void *r)
{
	t_class *c;

	c = class_new("monoNote", (method)monoNote_new, (method)monoNote_free, sizeof(t_monoNote), 0L, A_GIMME, 0);

	class_addmethod(c, (method)monoNote_list, "list", A_GIMME, 0);		
	class_addmethod(c, (method)monoNote_int, "in1", A_LONG, 0);

	class_addmethod(c, (method)monoNote_assist,	"assist",	A_CANT, 0);	

	class_register(CLASS_BOX, c);
	monoNote_class = c;

	object_post(NULL, "monoNote object loaded...",0);	// post any important info to the max window when our class is loaded
}

//--------------------------------------------------------------------------

void *monoNote_new(t_symbol* s, short ac, t_atom* av)
{
	t_monoNote* x;

	x = object_alloc(monoNote_class);

	intin(x, 1);

	x->lastNote = -1;
	x->lastTie = false;
	x->velScale = 1.0;
	listout(x);
	return x;
}

void monoNote_free(t_monoNote *x)
{
}

//--------------------------------------------------------------------------

void monoNote_assist(t_monoNote *x, void *b, long m, long a, char *s) // 4 final arguments are always the same for the assistance method
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


void monoNote_int(t_monoNote *x, long n)
{
	x->velScale = ((double)n) / (127.0);
}

void monoNote_list(t_monoNote* x, t_symbol* s, short ac, t_atom* av)
{
	long note = (long) atom_getlong(&av[1]);
	long vel = (long) atom_getlong(&av[2]);
	double duration = atom_getfloat(&av[2]);

	boolean tie = duration == 120;
	boolean rest = vel == 0;

	long outVel = (long) (vel * x->velScale);

	if (x->lastNote == -1 && !rest) {
		noteOut(x, note, outVel);
	}
	else if (x->lastTie && !rest && x->lastNote != note) {
		noteOut(x, note, outVel);
		noteOut(x, x->lastNote, 0);
	}
	else if (x->lastNote != -1 && rest) {
		noteOut(x, x->lastNote, 0);
	}
	else if (x->lastNote != -1 && !x->lastTie) {
		noteOut(x, x->lastNote, 0);
		noteOut(x, note, outVel);
	}

	x->lastNote = rest ? -1 : note;
	x->lastTie = tie;
}


void noteOut(t_monoNote* x, long note, long vel) {
	t_atom out[2];

	atom_setlong(
		out,
		note
	);
	atom_setlong(
		out + 1,
		vel
	);

	outlet_list(x->i_ob.o_outlet, NULL, 2, out);
}

