#include "ext.h"				// you must include this - it contains the external object's link to available Max functions
#include "ext_obex.h"			// this is required for all objects using the newer style for writing objects.

#define maxLength 64

typedef struct _randomCs {		// defines our object's internal variables for each instance in a patch
	t_object i_ob;
	long i_in;          // space for the inlet number used by all the proxies
	void* proxy1;

	short order[maxLength];

	long seed;
	int take;

	unsigned long int next;



	double allAs[maxLength];
	double allBs[maxLength];




	void* out0;
	void* out1;
} t_randomCs;

// these are prototypes for the methods that are defined below
void *randomCs_new(long dummy);
void randomCs_free(t_randomCs *x);
void randomCs_assist(t_randomCs *x, void *b, long m, long a, char *s);
void randomCs_int(t_randomCs* x, long d);
void randomCs_bang(t_randomCs* x);
void randomCs_regenerate(t_randomCs* x);


t_class *randomCs_class;		// global pointer to the object class - so max can reference the object

//--------------------------------------------------------------------------

void ext_main(void *r)
{
	t_class *c;

	c = class_new("randomCs", (method)randomCs_new, (method)randomCs_free, sizeof(t_randomCs), 0L, A_DEFLONG, 0);

	class_addmethod(c, (method)randomCs_int, "int", A_LONG, 0);
	class_addmethod(c, (method)randomCs_bang, "bang", A_LONG, 0);

	class_addmethod(c, (method)randomCs_assist,	"assist",	A_CANT, 0);	

	class_register(CLASS_BOX, c);
	randomCs_class = c;

	object_post(NULL, "randomCs object loaded...",0);	// post any important info to the max window when our class is loaded
}

//--------------------------------------------------------------------------

bool randomCs_contains(short value, short a[], int n)
{
	int i = 0;
	while (i < n && a[i] != value) i++;
	return !(i == n);
}


void *randomCs_new(long dummy)		// dummy = int argument typed into object box (A_DEFLONG) -- defaults to 0 if no args are typed
{
	t_randomCs* x;

	x = object_alloc(randomCs_class);

	x->proxy1 = proxy_new(&x->i_ob, 1, &x->i_in);

	x->out1 = listout(x);
	x->out0 = listout(x);

	x->order[0] = 0;
	x->order[1] = maxLength - 1;
	int pos = 2;
	for (double denominator = 2; denominator < 100; denominator++) {
		for (double numerator = 1; numerator < denominator; numerator++) {
			double fraction = numerator / denominator;
			short number = (short) round(fraction * maxLength);
			
			if (!randomCs_contains(number, x->order, maxLength)) {
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

	return x;
}

void randomCs_free(t_randomCs *x)
{
	freeobject((t_object*)x->proxy1);
}

//--------------------------------------------------------------------------

void randomCs_assist(t_randomCs* x, void* b, long m, long a, char* s) // 4 final arguments are always the same for the assistance method
{
	if (m == ASSIST_INLET){
		switch (a) {
		case 0:
			sprintf(s, "take");
			break;
		case 1:
			sprintf(s, "seed");
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


int randomCs_comp(const void* elem1, const void* elem2)
{
	short f = *((short*)elem1);
	short s = *((short*)elem2);
	return (f > s) - (f < s);
}

void randomCs_bang(t_randomCs* x) {
	short* indices = (short*)sysmem_newptr(x->take * sizeof(short));
	t_atom* as = (t_atom*)sysmem_newptr(x->take * sizeof(t_atom));
	t_atom* bs = (t_atom*)sysmem_newptr(x->take * sizeof(t_atom));

	for(short i = 0; i < x->take; i++){
		*(indices + i) = x->order[i];
	}

	qsort(indices, x->take, sizeof(*indices), randomCs_comp);

	for (short i = 0; i < x->take; i++) {
		atom_setfloat(as + i, x->allAs[indices[i]]);
		atom_setfloat(bs + i, x->allBs[indices[i]]);
	}

	outlet_list(x->out1, NULL, x->take, as);
	outlet_list(x->out0, NULL, x->take, bs);
	sysmem_freeptr(indices);
	sysmem_freeptr(as);
	sysmem_freeptr(bs);
}

void randomCs_int(t_randomCs* x, long d){
	switch (proxy_getinlet(&x->i_ob)) {
	case 0:
		x->take = d;
		break;
	case 1:
		x->seed = d;
		randomCs_regenerate(x);
 		break;
	}
	randomCs_bang(x);
}

#define RANDMAX 32767

int randomCs_rand(t_randomCs* x) // RAND_MAX assumed to be 32767
{
	x->next = x->next * 1103515245 + 12345;
	return (unsigned int)(x->next / 65536) % 32768;
}
void randomCs_srand(t_randomCs* x,  unsigned int seed)
{
	x->next = seed;
}


void randomCs_regenerate(t_randomCs* x) {
	randomCs_srand(x, x->seed);
	for (int i = 0; i < maxLength; i++) {
		double a, b;
		do {
			a = 2 * ((((double)randomCs_rand(x)) / ((double)RANDMAX)) - 0.5);
			b = 2 * ((((double)randomCs_rand(x)) / ((double)RANDMAX)) - 0.5);
		} while (1 < sqrt(a * a + b * b));
		x->allAs[i] = a;
		x->allBs[i] = b;
	}
}