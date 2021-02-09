#include "ext.h"				// you must include this - it contains the external object's link to available Max functions
#include "ext_obex.h"			// this is required for all objects using the newer style for writing objects.
#include "kiss_fft.h"

typedef struct _fftfilter {		// defines our object's internal variables for each instance in a patch
	t_object i_ob;
	long i_in;          // space for the inlet number used by all the proxies
	void* proxy2;
	void* proxy1;

	short prevLen;

	double* seqReal;
	short seqReal_ac;
	double* seqImg;
	short seqImg_ac;
	double* curve;
	short curve_ac;

	kiss_fft_cfg fft;
	kiss_fft_cfg ffti;

	void* out0;
	void* out1;
	void* out2;
	void* out3;
} t_fftfilter;

// these are prototypes for the methods that are defined below
void* fftfilter_new(t_symbol* s, short ac, t_atom* av);
void fftfilter_free(t_fftfilter* x);
void fftfilter_assist(t_fftfilter* x, void* b, long m, long a, char* s);
void fftfilter_list(t_fftfilter* x, t_symbol* s, short ac, t_atom* av);
void fftfilter_bang(t_fftfilter* x);
void fftfilter_resize_list(double** values, short* values_ac, short size);

t_class* fftfilter_class;		// global pointer to the object class - so max can reference the object

//--------------------------------------------------------------------------

void ext_main(void* r)
{
	t_class* c;

	c = class_new("fftfilter", (method)fftfilter_new, (method)fftfilter_free, sizeof(t_fftfilter), 0L, A_GIMME, 0);

	class_addmethod(c, (method)fftfilter_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)fftfilter_bang, "bang", 0);


	class_addmethod(c, (method)fftfilter_assist, "assist", A_CANT, 0);

	class_register(CLASS_BOX, c);
	fftfilter_class = c;

	object_post(NULL, "fftfilter object loaded...", 0);	// post any important info to the max window when our class is loaded
}

//--------------------------------------------------------------------------

void* fftfilter_new(t_symbol* s, short ac, t_atom* av)
{
	t_fftfilter* x;

	x = object_alloc(fftfilter_class);

	x->proxy2 = proxy_new(&x->i_ob, 2, &x->i_in);
	x->proxy1 = proxy_new(&x->i_ob, 1, &x->i_in);

	x->prevLen = -1;

	x->out3 = listout(x);
	x->out2 = listout(x);
	x->out1 = listout(x);
	x->out0 = listout(x);

	return x;
}

void fftfilter_free(t_fftfilter* x)
{
	sysmem_freeptr(x->seqReal);
	sysmem_freeptr(x->seqImg);

	freeobject((t_object*)x->proxy1);
	freeobject((t_object*)x->proxy2);

	kiss_fft_free(x->fft);
	kiss_fft_free(x->ffti);
}

//--------------------------------------------------------------------------

void fftfilter_assist(t_fftfilter* x, void* b, long m, long a, char* s) // 4 final arguments are always the same for the assistance method
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
			sprintf(s, "curve");
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
		case 2:
			sprintf(s, "f reals");
			break;
		case 3:
			sprintf(s, "f imgs");
			break;
		}
	}
}

void fftfilter_bang(t_fftfilter* x) {
	if (x->curve == NULL
		|| x->seqImg == NULL
		|| x->seqReal == NULL 
		|| x->curve_ac != x->seqReal_ac
		|| x->curve_ac != x->seqImg_ac) {
		return;
	}

	short len = x->seqReal_ac;
	if (len == 1) {
		t_atom* out_i_r = (t_atom*)sysmem_newptr(2 * sizeof(t_atom));
		atom_setfloat(&out_i_r [0], x->seqImg[0]);
		atom_setfloat(&out_i_r [1], x->seqReal[0]);
		outlet_list(x->out3, NULL, 1, out_i_r + 0);
		outlet_list(x->out2, NULL, 1, out_i_r + 1);
		outlet_list(x->out1, NULL, 1, out_i_r + 0);
		outlet_list(x->out0, NULL, 1, out_i_r + 1);
	}
	else if (len > 0) {
		if (len != x->prevLen) {
			x->fft = kiss_fft_alloc(len, false, 0, 0);
			x->ffti = kiss_fft_alloc(len, false, 0, 0);
			x->prevLen = len;
		}

		kiss_fft_cpx* input = (kiss_fft_cpx*)sysmem_newptr(len * sizeof(kiss_fft_cpx));
		for (short i = 0; i < len; i++) {
			*(input + i) = (kiss_fft_cpx){
				(float) x->seqReal[i],
				(float) x->seqImg[i]
			};
		}

		kiss_fft(x->fft, input, input);

		t_atom* outSeqFreqImg = (t_atom*)sysmem_newptr(len * sizeof(t_atom));
		t_atom* outSeqFreqReal = (t_atom*)sysmem_newptr(len * sizeof(t_atom));
		kiss_fft_cpx* output = (kiss_fft_cpx*)sysmem_newptr(len * sizeof(kiss_fft_cpx));


		for (short i = 0; i < len; i++) {
			double real = input[i].r * x->curve[i];
			atom_setfloat(
				&outSeqFreqReal[i], real / len);

			double img = input[i].i * x->curve[i];
			atom_setfloat(
				&outSeqFreqImg[i], img / len);

			*(output + i) = (kiss_fft_cpx){
				(float)real,
				(float)img
			};
		}

		kiss_fft(x->ffti, output, output);

		t_atom* outSeqImg = (t_atom*)sysmem_newptr(len * sizeof(t_atom));
		t_atom* outSeqReal = (t_atom*)sysmem_newptr(len * sizeof(t_atom));
		for (short i = 0; i < len; i++) {
			atom_setfloat(
				&outSeqReal[i], output[i].r/ len);
			atom_setfloat(
				&outSeqImg[i], output[i].i/ len);
		}

		outlet_list(x->out3, NULL, len, outSeqFreqImg);
		outlet_list(x->out2, NULL, len, outSeqFreqReal);
		outlet_list(x->out1, NULL, len, outSeqImg);
		outlet_list(x->out0, NULL, len, outSeqReal);

		sysmem_freeptr(outSeqFreqReal);
		sysmem_freeptr(outSeqFreqImg);
		sysmem_freeptr(outSeqReal);
		sysmem_freeptr(outSeqImg);
		sysmem_freeptr(output);
		sysmem_freeptr(input);
	}
}

void fftfilter_list(t_fftfilter* x, t_symbol* s, short ac, t_atom* av) {
	switch (proxy_getinlet(&x->i_ob)) {
	case 0:
		fftfilter_resize_list(&x->seqReal, &x->seqReal_ac, ac);
		atom_getdouble_array(ac, av, ac, x->seqReal);
		fftfilter_bang(x);
		break;
	case 1:
		fftfilter_resize_list(&x->seqImg, &x->seqImg_ac, ac);
		atom_getdouble_array(ac, av, ac, x->seqImg);
		break;
	case 2:
		fftfilter_resize_list(&x->curve, &x->curve_ac, ac);
		atom_getdouble_array(ac, av, ac, x->curve);
		fftfilter_bang(x);
		break;
	}
}

void fftfilter_resize_list(double** values, short* values_ac, short size)
{
	if (size != *values_ac) {
		if (*values)
			sysmem_freeptr(*values);
		if (size)
			*values = (double*)sysmem_newptr(size * sizeof(double));
		else
			*values = NULL;
		*values_ac = size;
	}
}