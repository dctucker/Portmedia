/*
 *  adsr.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-03-30.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */
#include <math.h>
#include "main.h"

#define ADSR_DIVISOR (1)
#define ADSR_MASK    (0x0)
// 16, 0xF
#define SIZE 524288
#define MASK 0x7FFFF
//#define resolution 524288.
#define resolution 4294967295.L
#define SHIFT (32-19)


class Counter {
protected:
	uint phase;
	uint incr;
	uint bits;
	//static const uint bits = SHIFT;
	
public:
	Counter(uint b = 0, uint incr = 1) :
		phase(0), incr(incr), bits(SHIFT)
	{
	}
	void operator *= (uint i){ // change frequency
		incr = i;
	}
	void operator >>= (uint i){ // change shift amount
		bits = i;
	}
	void operator ++ () { // increment prefix
		phase += incr;
	}
	uint operator ++ (int){ // increment postfix
		uint r = phase;
		phase += incr;
		return r >> bits;
	}
	void operator += (uint i) { // manual increment
		phase += i;
	}
	void operator = (uint i){ // reset phase
		phase = i << bits;
	}
	operator uint () { // returning function
		return phase >> bits;
	}
	uint mod(){ // remainder as integer component
		return ( phase & (( 2 << bits ) - 1) );
	}
	float frac(){ // remainder as a fraction
		return (float)(mod()) / (float)(1 << bits);
	}
	uint getIncr(){
		return incr;
	}
	//*/
};



class ADSR {
public:
	char stage;

	fl A, D, S, R;
	fl dA, dD, dS, dR;
	uint tA, tD, tR, clk;
	fl val;

	ADSR(fl a=0.1, fl d=1.0, fl s=0.75, fl r=0.25) :
		A(a), D(d), S(s), R(r), val(0.0f)
	{
		calcADSR();
	}
	void calcADSR();
	void setADSR (fl a=-1., fl d=-1., fl s=-1., fl r=-1.);
	fl doADSR();
};


typedef struct
{
	int num;
	bool down;
	//int stage;
	ADSR ampEnv, fltEnv;
	Counter phase, phase2, phase3;
	//unsigned int phase,phase2;
	//unsigned int freq;
	//unsigned int time;
	fl amp;
	fl vel;
	fl flt;
}
noteData;

