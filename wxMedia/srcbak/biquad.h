/*
 *  biquad.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-01-28.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */

#ifndef BIQUAD_H
#define BIQUAD_H

/* Simple implementation of Biquad filters -- Tom St Denis
 *
 * Based on the work

Cookbook formulae for audio EQ biquad filter coefficients
---------------------------------------------------------
by Robert Bristow-Johnson, pbjrbj@viconet.com  a.k.a. robert@audioheads.com

 * Available on the web at
 * http://www.smartelectronix.com/musicdsp/text/filters005.txt
 * Enjoy.
 *
 * This work is hereby placed in the public domain for all purposes, whether
 * commercial, free [as in speech] or educational, etc.  Use the code and please
 * give me credit if you wish.
 *
 * Tom St Denis -- http://tomstdenis.home.dhs.org
*/

#include <math.h>
#include <stdlib.h>
#include "main.h"

#ifndef M_LN2
#define M_LN2	   0.69314718055994530942
#endif

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

// whatever sample type you want
typedef float smp_type;

// filter types
enum {
    LPF,	// low pass filter
    HPF,	// High pass filter
    BPF,	// band pass filter
    NCH,	// Notch Filter
    PEQ,	// Peaking band EQ filter
    LSH,	// Low shelf filter
    HSH		// High shelf filter
};


// this holds the data required to update samples thru a filter
class BiQuad
{
public:
	BiQuad(int t,	smp_type f = 2000.,
					smp_type q = 0.707,
					smp_type g = 0.9
				);
	void setup();
	void initLPF();
	void initHPF();
	void initBPF();
	void initNCH();
	void initPEQ();
	void initLSH();
	void initHSH();
	void initCoeffs();
	smp_type filter(smp_type sample);
	void set(smp_type f = -1, smp_type q = -1, smp_type g = -1);
	void mod(smp_type v);
	
protected:
	bool on;
	smp_type a0_, a1_, a2_, a3_, a4_;
	smp_type x1_, x2_, y1_, y2_;
    smp_type a0, a1, a2, b0, b1, b2;
    smp_type A, omega, sn, cs, alpha, beta;
	smp_type freq, bw, gain;
	int type;
};


#endif

