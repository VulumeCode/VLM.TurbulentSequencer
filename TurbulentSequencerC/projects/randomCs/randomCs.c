#include "ext.h"				// you must include this - it contains the external object's link to available Max functions
#include "ext_obex.h"			// this is required for all objects using the newer style for writing objects.

#define maxLength 64

typedef struct _randomCs {		// defines our object's internal variables for each instance in a patch
	t_object i_ob;
	long i_in;          // space for the inlet number used by all the proxies
	void* proxy2;
	void* proxy1;

	long order[maxLength];

	long seed;
	int take;

	double allAs[maxLength];
	double allBs[maxLength];

	void* out0;
	void* out1;
} t_randomCs;

// these are prototypes for the methods that are defined below
void *randomCs_new(long dummy);
void randomCs_free(t_randomCs *x);
void randomCs_assist(t_randomCs *x, void *b, long m, long a, char *s);
void randomCs_int(t_randomCs* x, double d);

t_class *randomCs_class;		// global pointer to the object class - so max can reference the object

//--------------------------------------------------------------------------

void ext_main(void *r)
{
	t_class *c;

	c = class_new("randomCs", (method)randomCs_new, (method)randomCs_free, sizeof(t_randomCs), 0L, A_DEFLONG, 0);

	class_addmethod(c, (method)randomCs_int, "int", A_LONG, 0);

	class_addmethod(c, (method)randomCs_assist,	"assist",	A_CANT, 0);	

	class_register(CLASS_BOX, c);
	randomCs_class = c;

	object_post(NULL, "randomCs object loaded...",0);	// post any important info to the max window when our class is loaded
}

//--------------------------------------------------------------------------

bool contains(long value, long a[], int n)
{
	int i = 0;
	while (i < n && a[i] != value) i++;
	return !(i == n);
}


void *randomCs_new(long dummy)		// dummy = int argument typed into object box (A_DEFLONG) -- defaults to 0 if no args are typed
{
	t_randomCs* x;

	x = object_alloc(randomCs_class);

	x->proxy2 = proxy_new(&x->i_ob, 2, &x->i_in);
	x->proxy1 = proxy_new(&x->i_ob, 1, &x->i_in);

	x->out1 = listout(x);
	x->out0 = listout(x);

	x->order[0] = 0;
	x->order[1] = maxLength - 1;
	int pos = 2;
	for (double denominator = 2; denominator < 100; denominator++) {
		for (double numerator = 1; numerator < denominator; numerator++) {
			double fraction = numerator / denominator;
			long number = (long) round(fraction * maxLength);
			
			if (!contains(number, x->order, maxLength)) {
				x->order[pos++] = number;
				if (pos == maxLength) {
					goto search;
				}
			}
		}
	}
	search:

	x->seed = 0;
	x->take = 16;
	rnd
	return x;
}

void randomCs_free(t_randomCs *x)
{
	sysmem_freeptr(x->octProbs);
	sysmem_freeptr(x->noteProbs);
	sysmem_freeptr(x->noteToggles);

	freeobject((t_object*)x->proxy1);
	freeobject((t_object*)x->proxy2);
}

//--------------------------------------------------------------------------

void randomCs_assist(t_randomCs* x, void* b, long m, long a, char* s) // 4 final arguments are always the same for the assistance method
{
	if (m == ASSIST_INLET){
		switch (a) {
		case 0:
			sprintf(s, "octProbs");
			break;
		case 1:
			sprintf(s, "noteProbs");
			break;
		case 2:
			sprintf(s, "noteToggles");
			break;
		}
	}
	else {
		switch (a) {
		case 0:
			sprintf(s, "used notes");
			break;
		case 1:
			sprintf(s, "accumProbs");
			break;
		}
	}
}

void randomCs_bang(t_randomCs* x) {
	t_linklist* usedList = linklist_new(); linklist_flags(usedList, OBJ_FLAG_MEMORY);
	int note = 0;
	t_linklist* accumProbsList = linklist_new(); linklist_flags(accumProbsList, OBJ_FLAG_MEMORY);
	double accum = 0;
	
	for (int i = octsLength-1; i >= 0; i--) {
		for (int j = 0; j < notesLength; j++) {
			double prob = x->octProbs[i] * x->noteProbs[j] * x->noteToggles[j];
			if (prob > 0) {

				int* a = (int*)sysmem_newptr(sizeof(int));
				memcpy(a, &note, sizeof(int));
				linklist_append(usedList, a);
			}
			accum += prob;

			double* a = (double*)sysmem_newptr(sizeof(double));
			memcpy(a, &accum, sizeof(double));
			linklist_append(accumProbsList, a);

			note++;
		}
	}

	short usedList_ac = (short)linklist_getsize(usedList);
	t_atom* usedList_out = (t_atom*)sysmem_newptr(usedList_ac * sizeof(t_atom));
	for (short i = 0; i < usedList_ac; i++) {
		atom_setlong(
			usedList_out + i,
			*(int*)linklist_getindex(usedList, i)
		);
	}
	outlet_list(x->out0, NULL, usedList_ac, usedList_out);
	sysmem_freeptr(usedList_out);
	object_free(usedList);


	short accumProbsList_ac = (short)linklist_getsize(accumProbsList);
	t_atom* accumProbsList_out = (t_atom*)sysmem_newptr(accumProbsList_ac * sizeof(t_atom));
	for (short i = 0; i < accumProbsList_ac; i++) {
		atom_setfloat(
			accumProbsList_out + i,
			(*(double*)linklist_getindex(accumProbsList, i)) / accum
		);
	}
	outlet_list(x->out1, NULL, accumProbsList_ac, accumProbsList_out);
	sysmem_freeptr(accumProbsList_out);
	object_free(accumProbsList);
}

void randomCs_list(t_randomCs* x, t_symbol* s, short ac, t_atom* av) {
	switch (proxy_getinlet(&x->i_ob)) {
	case 0:
		atom_getdouble_array(octsLength, av, octsLength, x->octProbs);
		break;
	case 1:
		atom_getdouble_array(notesLength, av, notesLength, x->noteProbs);
		break;
	case 2:
		atom_getdouble_array(notesLength, av, notesLength, x->noteToggles);
		break;
	}
	randomCs_bang(x);
}