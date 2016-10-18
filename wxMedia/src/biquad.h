/*
 *  biquad.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-01-28.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */
#pragma once

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
#include "filter.h"

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
class BiQuad : public Filter
{
public:
	BiQuad(int t//,	smp_type f = 2000.,
				//	smp_type q = 0.707,
				//	smp_type g = 0.9
				);
	virtual void setup();
	virtual smp_type filter(smp_type sample);
	virtual smp_type filter_r(smp_type sample);
	//void set(smp_type f = -1, smp_type q = -1, smp_type g = -1);
	virtual void mod(smp_type v);
	virtual void reset();
	virtual smp_type query(smp_type);

protected:
	bool on;
	smp_type a0_, a1_, a2_, a3_, a4_;
	smp_type x1_, x2_, y1_, y2_;
	smp_type x1r_, x2r_, y1r_, y2r_;
    smp_type a0, a1, a2, b0, b1, b2;
    smp_type A, omega, sn, cs, alpha, beta;
	//smp_type freq, reso, gain;
	int type;
private:
	void initLPF();
	void initHPF();
	void initBPF();
	void initNCH();
	void initPEQ();
	void initLSH();
	void initHSH();
	void initCoeffs();


inline float lpFreq(float v){ return 220. * pow(2, v * 6.5); } //* pow(100, v); }
inline float hpFreq(float v){ return 32.5 * pow(2, v * 6); }// 1. * pow(1000, 0.5 + 0.6 * v); } //30. + v * 2000; } //5 * pow(500, v);
inline float lpRes (float v){ v *= 2; v += 0.5; return 0.2 + v*v; }
inline float hpRes (float v){ return lpRes(v); }
	
};

class BiQuad2 : public Filter
{
public:
	BiQuad bq1, bq2;
	BiQuad2(int t) : bq1(t), bq2(t) {}
	smp_type filter(smp_type sample){ return bq1.filter(bq2.filter(sample)); }
	smp_type filter_r(smp_type sample){ return bq1.filter_r(bq2.filter_r(sample)); }
	void set(smp_type f=-1, smp_type q=-1, smp_type g=-1)
	{
		bq1.set(f,q,g); bq2.set(f,q,g);
	}
	void mod(smp_type v){ bq1.mod(v); bq2.mod(v); }
	void reset(){ bq1.reset(); bq2.reset(); }
	smp_type query(smp_type s){ return bq1.query(s) + bq2.query(s); }
	
};
