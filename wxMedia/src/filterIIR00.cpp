/* 
	Resonant low pass filter source code.
	Originally by baltrax@hotmail.com (Zxform)
	Adapted by D. Casey Tucker
*/

#include "filterIIR00.h"


void FilterIIR::initProto()
{
	//Setup filter s-domain coefficients
	// Section 1
	ProtoCoef[0].a0 = 1.0;
	ProtoCoef[0].a1 = 0;
	ProtoCoef[0].a2 = 0;
	ProtoCoef[0].b0 = 1.0;
	ProtoCoef[0].b1 = 0.765367;
	ProtoCoef[0].b2 = 1.0;

	// Section 2
	ProtoCoef[1].a0 = 1.0;
	ProtoCoef[1].a1 = 0;
	ProtoCoef[1].a2 = 0;
	ProtoCoef[1].b0 = 1.0;
	ProtoCoef[1].b1 = 1.847759;
	ProtoCoef[1].b2 = 1.0;

}

/*
Example main function to show how to update filter coefficients.
We create a 4th order filter (24 db/oct roloff), consisting
of two second order sections.
*/

// Allocate array of z-domain coefficients for each
// filter section plus filter gain variable

FilterIIR::FilterIIR(double fc, double Q) :
	length  ( FILTER_SECTIONS ),
	history ( new float[     2 * length ] ),
	coeffs  ( new float[ 1 + 4 * length ] ),
	resonance(Q),
	cutoff(fc),
	fs(SAMPLE_RATE)
	//pi( 4.0 * atan(1.0) )
{
	
	initProto();
	
	for( int i = 0; i <  2 * length; ++i ) history[i] = 0;
	for( int i = 0; i <= 4 * length; ++i ) coeffs [i] = 0;
	
	set(fc, Q);

	// it would be beneficial to pre-compute coef values given a frequency range
	// also, sharing filter coefficients between object would reduce memory load
}

FilterIIR::~FilterIIR()
{
	delete[] history;
	delete[] coeffs;
}

void FilterIIR::set(double fc, double Q, double k)
{
	float    *coef;
	double   a0, a1, a2, b0, b1, b2;
	
	cutoff = fc;
	resonance = Q;

	coef = coeffs + 1;     // Skip k, or gain

	
	// Compute z-domain coefficients for each biquad section
	// for new Cutoff Frequency and Resonance
	
	for (unsigned nInd = 0; nInd < length; nInd++)
	{
		a0 = ProtoCoef[nInd].a0;
		a1 = ProtoCoef[nInd].a1;
		a2 = ProtoCoef[nInd].a2;

		b0 = ProtoCoef[nInd].b0;
		b1 = ProtoCoef[nInd].b1 / Q;      /* Divide by resonance or Q */
		b2 = ProtoCoef[nInd].b2;
		
		szxform(&a0, &a1, &a2, &b0, &b1, &b2, fc, fs, &k, coef);
		coef += 4;               /* Point to next filter section */
	}

	/* Update overall filter gain in coef array */
	coeffs[0] = k;
}

float FilterIIR::iir_filter(float input)
{
    unsigned int i;
    float *hist1_ptr, *hist2_ptr, *coef_ptr;
    float output, new_hist, history1, history2;

    coef_ptr  = coeffs;         // coefficient pointer
    hist1_ptr = history;        // first history
    hist2_ptr = hist1_ptr + 1;  // next history

    // 1st number of coefficients array is 
	// overall input scale factor, or filter gain
    output = input * (*coef_ptr++);

    for( i = 0 ; i < length; i++)
	{
		history1 = *hist1_ptr;
		history2 = *hist2_ptr;

		output   = output - history1 * (*coef_ptr++);
		new_hist = output - history2 * (*coef_ptr++); // poles

		output = new_hist + history1 * (*coef_ptr++);
		output =   output + history2 * (*coef_ptr++); // zeros

		*hist2_ptr++ = *hist1_ptr;
		*hist1_ptr++ = new_hist;
		hist1_ptr++;
		hist2_ptr++;
    }

    return(output);
}

void FilterIIR::bilinear(
    double a0, double a1, double a2,    // numerator coefficients
    double b0, double b1, double b2,    // denominator coefficients
    double *k,           // overall gain factor
    double fs,           // sampling rate
    float *coef)         // pointer to 4 iir coefficients
{
    double ad, bd, fs2, a2fs2, b2fs2, a1fs, b1fs;
	
	// why compute more than once?
	fs2   = fs * fs;
	a2fs2 = 4. * a2 * fs2; a1fs  = 2. * a1 * fs;
	b2fs2 = 4. * b2 * fs2; b1fs  = 2. * b1 * fs;
	
	ad = a2fs2 + a1fs + a0; // alpha (Numerator in s-domain)	
	bd = b2fs2 + b1fs + b0; // beta (Denominator in s-domain)

	*k *= ad / bd;      // update gain constant for this section

	// Denominator
	*coef++ = (-2. * b2fs2        + 2. * b0) / bd; // beta1
	*coef++ = (      b2fs2 - b1fs +      b0) / bd; // beta2

	// Nominator
	*coef++ = (-2. * a2fs2        + 2. * a0) / ad; // alpha1
	*coef   = (      a2fs2 - a1fs +      a0) / ad; // alpha2
}


void FilterIIR::prewarp( double *a0, double *a1, double *a2, double fc, double fs)
{
    double wp = 2.0 * fs * tan(M_PI * fc / fs);

    *a2 /= (wp * wp);
    *a1 /= wp;
}


void FilterIIR::szxform(
    double *a0, double *a1, double *a2, // numerator coefficients
    double *b0, double *b1, double *b2, // denominator coefficients
    double  fc, double  fs, double *k , // Filter cutoff, sampling frequency, gain
    float *coef)                        // pointer to 4 iir coefficients
{
	// Calculate a1 and a2 and overwrite the original values
	prewarp(a0, a1, a2, fc, fs);
	prewarp(b0, b1, b2, fc, fs);
	bilinear(*a0, *a1, *a2, *b0, *b1, *b2, k, fs, coef);
}

///////////////////////////////////////////////////////////////////////////////

FilterIIR7::FilterIIR7()
{
	for( int i = 0; i < 128; ++i )
	{
		precoeffs[i] = new float[4 * length + 1];
	}
}

FilterIIR7::~FilterIIR7()
{
	for( int i = 0; i < 128; ++i )
	{
		delete precoeffs[i];
	}
	//delete precoeffs;
}

void FilterIIR7::setRange(double fc1, double fc2)
{
	lofreq = fc1; hifreq = fc2;
	double step = (fc2 - fc1) / 128.;
	double freq = fc1;
	for( int i = 0; i < 128; ++i )
	{
		FilterIIR::set( freq, 0.7 );
		for( int j = 0; j <= 4 * length; ++j ) // copy computed coeffs
		{
			precoeffs[i][j] = coeffs[j];
		}
		freq += step;
	}
}

void FilterIIR7::set(int v)
{
	coeffs = precoeffs[v];
}

void FilterIIR7::set(double fc)
{
	int v = 128. * ((fc - lofreq) / hifreq);
	if(v >= 128) v = 127;
	coeffs = precoeffs[v];
}

//////////////////////////////////////////////////////////////////////////////

#define RESOLUTION_FACTOR 1000

FilterIIR77::FilterIIR77()
{
	for(int i = 0; i < 128; ++i )
	{
		cutoffs[i] = targets[i] = 127 * RESOLUTION_FACTOR;
		histories[i] = new float[4 * length + 1];
		for(int j = 0; j <= 4 * length; ++j )
		{
			histories[i][j] = 0.;
		}
	}
}

FilterIIR77::~FilterIIR77()
{
	for(int i = 0; i < 128; ++i)
	{
		delete[] histories[i];
	}
	//delete histories;
}

void FilterIIR77::set(int note, int f)
{
	// this needs to become gradual
	targets[note] = f * RESOLUTION_FACTOR;
	//cutoffs[note] = f;
}

float FilterIIR77::iir_filter(int note, float sample)
{	
	// this is insufficient. consider mapping coefficients rather than cutoffs
	if( cutoffs[note] > targets[note] )
		--cutoffs[note];
	if( cutoffs[note] < targets[note] )
		++cutoffs[note];
	if( cutoffs[note] < 0 )
		cutoffs[note] = 0;
	
	history = histories[note];
	coeffs = precoeffs[cutoffs[note] / RESOLUTION_FACTOR];
	return FilterIIR::iir_filter(sample);
}
