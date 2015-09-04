/*
 *  filter.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-01-16.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */
#ifndef FILTER_H
#define FILTER_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "main.h"


// FILTER INFORMATION STRUCTURE FOR FILTER ROUTINES

/*
typedef struct {
	unsigned int length;     // Number of filter sections
	float        *history;   // pointer to history in filter
	float        *coef;      // pointer to coefficients of filter
} FILTER;
*/

#define FILTER_SECTIONS   2  // two filter sections for 24 db/oct filter

typedef struct {
	double a0, a1, a2;       // numerator coefficients
	double b0, b1, b2;       // denominator coefficients
} BIQUAD;

class FilterIIR
{
public:
	//FilterIIR();
	FilterIIR(double fc = 2000.0, double Q = 0.707 );
	~FilterIIR();
	void set(double fc, double Q, double k = 1.0); // fc in Hz, 1.0 < Q < 1000

	// To process audio samples, call function iir_filter() for each audio sample
	/* iir_filter - Perform IIR filtering sample by sample on floats

	Implements cascaded direct form II second order sections.
	Requires FILTER structure for history and coefficients.
	The length in the filter structure specifies the number of sections.
	The size of the history array is 2*iir->length.
	The size of the coefficient array is 4*iir->length + 1 because
	the first coefficient is the overall scale factor for the filter.
	Returns one output sample for each input sample.  Allocates history
	array if not previously allocated.

	float iir_filter(float input,FILTER *iir)

	float input        new float input sample
	FILTER *iir        pointer to FILTER structure

	Returns float value giving the current output.

	Allocation errors cause an error message and a call to exit.  */
	float iir_filter(float input);

protected:
	double fs;
	//FILTER   iir;
	unsigned int length;
	float *history;
	float *coeffs;

	void initProto();
	BIQUAD ProtoCoef[FILTER_SECTIONS]; // Filter prototype coefficients, 1 for each filter section
	
	double cutoff, resonance;
	
	
	// Pre-warp the coefficients of a numerator or denominator.
	// Note that a0 is assumed to be 1, so there is no wrapping of it.
	inline void prewarp  (double *a0, double *a1, double *a2,
	               double  fc, double  fs );

	/*
	Transform from s to z domain using bilinear transform
	with prewarp.

	Arguments:
		For argument description look at bilinear()

		coef - pointer to array of floating point coefficients,
				corresponding to output of bilinear transofrm
				(z domain).

	Note: frequencies are in Hz. */

	void szxform  (double *a0, double *a1, double *a2, 
	               double *b0, double *b1, double *b2,
	               double  fc, double  fs, double *k , float *coef );


	/*
	Transform the numerator and denominator coefficients
	of s-domain biquad section into corresponding
	z-domain coefficients.

		Store the 4 IIR coefficients in array pointed by coef
		in following order:
			beta1, beta2    (denominator)
			alpha1, alpha2  (numerator)

	Arguments:
		a0-a2   - s-domain numerator coefficients
		b0-b2   - s-domain denominator coefficients
		k       - filter gain factor. initially set to 1
					and modified by each biquad section in such
					a way, as to make it the coefficient by
					which to multiply the overall filter gain
					in order to achieve a desired overall filter gain,
					specified in initial value of k.
		fs      - sampling rate (Hz)
		coef    - array of z-domain coefficients to be filled in.

	Return:
		On return, set coef z-domain coefficients   */

	void bilinear (double  a0, double  a1, double  a2,
	               double  b0, double  b1, double  b2,
	               double *k , double  fs, float *coef );
};


/*
	Perform bilinear transformation on s-domain coefficients
	of 2nd order biquad section.
	First design an analog filter and use s-domain coefficients
	as input to szxform() to convert them to z-domain.

	Here's the butterworth polinomials for 2nd, 4th and 6th order sections.
	When we construct a 24 db/oct filter, we take to 2nd order
	sections and compute the coefficients separately for each section.

   n       Polinomials
 -------------------------------------------------------------------
   2       s^2 + 1.4142s +1
   4       (s^2 + 0.765367s + 1) (s^2 + 1.847759s + 1)
   6       (s^2 + 0.5176387s + 1) (s^2 + 1.414214 + 1) (s^2 + 1.931852s + 1)

	Where n is a filter order.
	For n=4, or two second order sections, we have following equasions for each
	2nd order stage:

	(1 / (s^2 + (1/Q) * 0.765367s + 1)) * (1 / (s^2 + (1/Q) * 1.847759s + 1))

	Where Q is filter quality factor in the range of
	1 to 1000. The overall filter Q is a product of all
	2nd order stages. For example, the 6th order filter
	(3 stages, or biquads) with individual Q of 2 will
	have filter Q = 2 * 2 * 2 = 8.

	The nominator part is just 1.
	The denominator coefficients for stage 1 of filter are:
	b2 = 1; b1 = 0.765367; b0 = 1;
	numerator is
	a2 = 0; a1 = 0; a0 = 1;

	The denominator coefficients for stage 1 of filter are:
	b2 = 1; b1 = 1.847759; b0 = 1;
	numerator is
	a2 = 0; a1 = 0; a0 = 1;

	These coefficients are used directly by the szxform()
	and bilinear() functions. For all stages the numerator
	is the same and the only thing that is different between
	different stages is 1st order coefficient. The rest of
	coefficients are the same for any stage and equal to 1.

	Any filter could be constructed using this approach.

	References:
		Van Valkenburg, "Analog Filter Design"
		Oxford University Press 1982
		ISBN 0-19-510734-9

		C Language Algorithms for Digital Signal Processing
		Paul Embree, Bruce Kimble
		Prentice Hall, 1991
		ISBN 0-13-133406-9

		Digital Filter Designer's Handbook
		With C++ Algorithms
		Britton Rorabaugh
		McGraw Hill, 1997
		ISBN 0-07-053806-9
----------------------------------------------------------
 */

// Filter with 128 precomputed cutoffs
class FilterIIR7 : public FilterIIR
{
public:
	FilterIIR7();
	~FilterIIR7();
	void setRange(double fc1, double fc2);
	void set(int v), set(double);
protected:
	float *precoeffs[128];
	double lofreq, hifreq;
};


// Filter appropriate for 128 keys, 128 precomputed cutoffs
class FilterIIR77 : public FilterIIR7
{
public:
	FilterIIR77();
	~FilterIIR77();
	float iir_filter(int note, float sample);
	void set(int,int);
private:
	float *histories[128];
	int cutoffs[128], targets[128];
};

#endif
