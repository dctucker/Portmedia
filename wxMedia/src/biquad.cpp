/*
 *  biquad.cpp
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-01-28.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */

#include "biquad.h"

// Below this would be biquad.c
// Computes a BiQuad filter on a sample
smp_type BiQuad::filter(smp_type sample)
{
	smp_type result;
	
	// compute result
	result = a0_ * sample   +  a1_ * x1_  +  a2_ * x2_
	                        -  a3_ * y1_  -  a4_ * y2_;

	// shift x1 to x2, sample to x1
	x2_ = x1_;
	x1_ = sample;

	/// shift y1 to y2, result to y1
	y2_ = y1_;
	y1_ = result;

	//if( not on )
	//	return sample;

    return result;
}

smp_type BiQuad::filter_r(smp_type sample)
{
	smp_type result;

	// compute result
	result = a0_ * sample   +  a1_ * x1r_  +  a2_ * x2r_
	                        -  a3_ * y1r_  -  a4_ * y2r_;
	
	// shift x1 to x2, sample to x1
	x2r_ = x1r_;
	x1r_ = sample;

	/// shift y1 to y2, result to y1
	y2r_ = y1r_;
	y1r_ = result;

	//if( not on )
	//	return sample;

	return result;
}

void BiQuad::initCoeffs()
{
	switch (type)
	{
		case LPF: initLPF(); break;
		case HPF: initHPF(); break;
		case BPF: initBPF(); break;
		case NCH: initNCH(); break;
		case PEQ: initPEQ(); break;
		case LSH: initLSH(); break;
		case HSH: initHSH(); break;
	}

	// precompute the coefficients
	a0_ = b0 / a0;
	a1_ = b1 / a0;
	a2_ = b2 / a0;
	a3_ = a1 / a0;
	a4_ = a2 / a0;	
}

void BiQuad::initLPF() // lowpass
{
	register smp_type cs1 = 1 - cs;
	b0 = cs1 / 2;
	b1 = cs1;
	b2 = cs1 / 2;
	a0 =  1 + alpha;
	a1 = -2 * cs;
	a2 =  1 - alpha;
}
void BiQuad::initHPF() // highpass
{
	register smp_type cs1 = 1 + cs;
	b0 =  cs1 / 2;
	b1 = -cs1;
	b2 =  cs1 / 2;
	a0 =  1 + alpha;
	a1 = -2 * cs;
	a2 =  1 - alpha;
}
void BiQuad::initBPF() // bandpass
{
	b0 =  alpha;
	b1 = 0;
	b2 = -alpha;
	a0 =  1 + alpha;
	a1 = -2 * cs;
	a2 =  1 - alpha;
}

void BiQuad::initNCH() // notch
{
	b0 =  1;
	b1 = -2 * cs;
	b2 =  1;
	a0 =  1 + alpha;
	a1 = b1;
	a2 =  1 - alpha;
}
void BiQuad::initPEQ() // peaking
{
	register smp_type ama = alpha * A;
	register smp_type ada = alpha / A;
	b0 =  1 + ama;
	b1 = -2 * cs;
	b2 =  1 - ama;
	a0 =  1 + ada;
	a1 =  b1;
	a2 =  1 - ada;
}

void BiQuad::initLSH() // low shelf
{
	register smp_type
		bsn = beta * sn,
		ap1 = A + 1,
		am1 = A - 1,
		am1cs = am1 * cs,
		ap1cs = ap1 * cs;

	b0 =     A * (ap1 - am1cs + bsn);
	b1 = 2 * A * (am1 - ap1cs );
	b2 =     A * (ap1 - am1cs - bsn);
	a0 =          ap1 + am1cs + bsn;
	a1 =    -2 * (am1 + ap1cs );
	a2 =          ap1 + am1cs - bsn;
}

void BiQuad::initHSH() // high shelf
{
	register smp_type
		bsn = beta * sn,
		ap1 = A + 1,
		am1 = A - 1,
		am1cs = am1 * cs,
		ap1cs = ap1 * cs;

	b0 =      A * (ap1 + am1cs + bsn);
	b1 = -2 * A * (am1 + ap1cs );
	b2 =      A * (ap1 + am1cs - bsn);
	a0 =           ap1 - am1cs + bsn;
	a1 =      2 * (am1 - ap1cs );
	a2 =           ap1 - am1cs - bsn;
}

void BiQuad::mod(smp_type v)
{
	smp_type cf = freq;

	freq *= fmaxf(0.01f, 1.0f - v);
	freq = fmaxf( 30., freq );
	setup();
	
	freq = cf;
}

smp_type BiQuad::query(smp_type f)
{
	register smp_type w = 2.f * M_PI * f / (smp_type)SAMPLE_RATE;
	register smp_type phi = sin(0.5f * w);
	register smp_type b024 = 4.f * b0 * b2, a024 = 4.f * a0 * a2;
	register smp_type b012s = b0+b1+b2, a012s = a0+a1+a2;
	b012s *= b012s; a012s *= a012s;
	phi *= phi;
	// phi *= 4;
	register smp_type phi2 = phi * phi;

	return 10.f * (
		log10f( b012s + 4.f * ( b024 * phi2 - (b0*b1 + b024 + b1*b2) * phi ) ) -
		log10f( a012s + 4.f * ( a024 * phi2 - (a0*a1 + a024 + a1*a2) * phi ) )
	);

/*
20*log10[|H(e^jw)|] =
 10*log10[ (b0+b1+b2)^2 - 4*(b0*b1 + 4*b0*b2 + b1*b2)*phi + 16*b0*b2*phi^2 ]
-10*log10[ (a0+a1+a2)^2 - 4*(a0*a1 + 4*a0*a2 + a1*a2)*phi + 16*a0*a2*phi^2 ]

where phi = sin^2(w/2)
*/

	//return y; //20 * log10 ( fabs( H( e ^ jw ) ) );
}

void BiQuad::setup()
{
    // setup variables
	omega = 2. * M_PI * freq / (smp_type)SAMPLE_RATE;
	cs = cos(omega);
	sn = sin(omega);
	//alpha = sn * sinh( M_LN2 / 2. * reso * omega / sn );
	alpha = sn / (2*reso);

	if( type == PEQ  or  type == LSH  or  type == HSH )
	{
		A = pow(10, gain /40);
		beta = sqrt(A + A);
	}
	/* // from the cookbook:
		alpha
		  = sin(w0)/(2*Q)                                // (case: Q)
		  = sn *sinh( ln(2)/2 * reso * w0/sin(w0) )      // (case: BW)
		  = sin(w0)/2 * sqrt( (A + 1/A)*(1/S - 1) + 2 )  // (case: S)
	*/
	initCoeffs();
}

void BiQuad::reset()
{
	// zero initial samples
	x1_ = x2_ = 0;
	y1_ = y2_ = 0;
}

// sets up a BiQuad Filter
BiQuad::BiQuad(int t) :
	type(t),
	on(true)
{
	gain = 0;

	setup();
	reset();
}
