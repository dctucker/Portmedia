/*
 *  adsr.cpp
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-03-30.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */

#include "adsr.h"

void ADSR::setADSR (float a, float d, float s, float r)
{
	if( a >= 0.0f )  A = 15.f * powf(a, 6.0f) + 0.01f;
	if( d >= 0.0f )  D =  5.f * powf(d, 2.0f) + 0.01f;
	if( s >= 0.0f )  S = s;
	if( r >= 0.0f )  R = 15.f * powf(r, 6.0f) + 0.01f;
	calcADSR();
}

void ADSR::calcADSR()
{
	static float const k = ((float)SAMPLE_RATE) / (float)ADSR_DIVISOR;
	/*
	tA = A * k; //* SAMPLE_RATE / ADSR_DIVISOR;
	tD = D * k; //* SAMPLE_RATE / ADSR_DIVISOR;
	tR = R * k; //* SAMPLE_RATE / ADSR_DIVISOR;

	dA = 1.f / tA;
	dD = 1.f / tD;
	dR = 1.f / tR;
	*/
	dA = 1.f / (A * k);
	dD = 1.f / (D * k);
	dR = 1.f / (R * k);
}


fl ADSR::doADSR()
{	
	asm("# S");
	if( clk | ADSR_MASK != 0 and stage == 'S' ) return val;

	//modvec = true;
	switch(stage)
	{
	case 'A': asm("# A");
		val += dA;
		if( val >= 1.0f )
		{
			stage = 'D';
			val   = 1.0f;
		}
		break;
	case 'D': asm("# D");
		val -= dD;
		if( val <= 0.0f ) val = 0.0f;

		if( val < S ) //or notes[n].time >= tD )
		{
			stage = 'S';
			val   = S;
		}
		break;
	case 'R': asm("# R");
		val -= dR;
		if( val <= 0.0f )
		{
			val   = 0.0f;
			stage = '_';
		} 
		break;
	case '_':
		//bend(note.num);
		//note.time   = 0;
		break;
	}
	//if(notes[n].amp > notes[n].vel) notes[n].amp = notes[n].vel;
	//modvec = false;
	//std::cout << notes[n].amp << "\n";
	return val;
}
