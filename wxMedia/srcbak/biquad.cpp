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

void BiQuad::initLPF() // cs, alpha
{
	b0 = (1 - cs) /2;
	b1 =  1 - cs;
	b2 = (1 - cs) /2;
	a0 =  1 + alpha;
	a1 = -2 * cs;
	a2 =  1 - alpha;
}
void BiQuad::initHPF()
{
	b0 =  (1 + cs) /2;
	b1 = -(1 + cs);
	b2 =  (1 + cs) /2;
	a0 =  1 + alpha;
	a1 = -2 * cs;
	a2 =  1 - alpha;
}
void BiQuad::initBPF()
{
	b0 =  alpha;
	b1 = 0;
	b2 = -alpha;
	a0 =  1 + alpha;
	a1 = -2 * cs;
	a2 =  1 - alpha;
}

void BiQuad::initNCH()
{
	b0 =  1;
	b1 = -2 * cs;
	b2 =  1;
	a0 =  1 + alpha;
	a1 = -2 * cs;
	a2 =  1 - alpha;
}
void BiQuad::initPEQ() // cs, alpha, A
{
	b0 =  1 + (alpha * A);
	b1 = -2 * cs;
	b2 =  1 - (alpha * A);
	a0 =  1 + (alpha / A);
	a1 = -2 * cs;
	a2 =  1 - (alpha / A);
}

void BiQuad::initLSH() // cs, beta, A, sn
{
	b0 =     A * ((A + 1) - (A - 1) * cs + beta * sn);
	b1 = 2 * A * ((A - 1) - (A + 1) * cs );
	b2 =     A * ((A + 1) - (A - 1) * cs - beta * sn);
	a0 =          (A + 1) + (A - 1) * cs + beta * sn;
	a1 =    -2 * ((A - 1) + (A + 1) * cs );
	a2 =          (A + 1) + (A - 1) * cs - beta * sn;
}

void BiQuad::initHSH()
{
	b0 =      A * ((A + 1) + (A - 1) * cs + beta * sn);
	b1 = -2 * A * ((A - 1) + (A + 1) * cs );
	b2 =      A * ((A + 1) + (A - 1) * cs - beta * sn);
	a0 =           (A + 1) - (A - 1) * cs + beta * sn;
	a1 =      2 * ((A - 1) - (A + 1) * cs );
	a2 =           (A + 1) - (A - 1) * cs - beta * sn;
}

void BiQuad::set(smp_type f, smp_type q, smp_type g)
{
	//on = (f > 0.0);
	if(f != -1)   freq = f;
	if(q != -1)   bw = q;
	if(g != -1)   gain = g;
	
	setup();
	initCoeffs();
}

void BiQuad::mod(smp_type v){
	smp_type cf = freq;
	
	freq *= 1.0 - v;
	setup();
	initCoeffs();
	
	freq = cf;
}

void BiQuad::setup()
{
    // setup variables
    omega = 2 * M_PI * freq / (smp_type)SAMPLE_RATE;
	cs = cos(omega);
	sn = sin(omega);
    alpha = sn * sinh( M_LN2 / 2. * bw * omega / sn );

	if( type == PEQ or type == LSH or type == HSH) {
		A = pow(10, gain /40);
		beta = sqrt(A + A);
	}
}


// sets up a BiQuad Filter
BiQuad::BiQuad(int t, smp_type f, smp_type q, smp_type g) :
	type(t),
	freq(f),
	bw(q),
	gain(g),
	on(true)
{

	setup();
	initCoeffs();

    // zero initial samples
    x1_ = x2_ = 0;
    y1_ = y2_ = 0;

}
