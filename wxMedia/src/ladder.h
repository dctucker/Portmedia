/*
 *  ladder.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-07-08.
 *  Copyright 2010 Moog. All rights reserved.
 *
 */

#ifndef LADDER_H
#define LADDER_H

#include "main.h"
#include "filter.h"
#define fl smp_type

#define Fs  ((smp_type)SAMPLE_RATE)
#define PI  3.14159265358979323846
#define Vt  40000
//25.8520269e-6
//40000
//25.8520269e-6

class Ladder : public Filter
{
public:
	Ladder();
	void setup();
	smp_type filter(smp_type);
	smp_type filter_r(smp_type);
	void mod(smp_type);
	void reset();
	smp_type query(smp_type);
	void set(smp_type=-1,smp_type=-1,smp_type=-1);
protected:
	fl  Fc, r;

	fl	ya  , yb  , yc  , yd  , y ,
		ya1 , yb1 , yc1 , yd1 ,
		Wa  , Wb  , Wc  , Wd  ,
		Wa1 , Wb1 , Wc1 ;
	fl g;
	fl IctlCFs;
	fl Vt2;

inline float lpFreq(float v){ return 220. * pow(2, v * 6.5); } //* pow(100, v); }
inline float hpFreq(float v){ return 32.5 * pow(2, v * 6); }// 1. * pow(1000, 0.5 + 0.6 * v); } //30. + v * 2000; } //5 * pow(500, v);
inline float lpRes (float v){ return v; }
inline float hpRes (float v){ return v; }
	

};

#endif
