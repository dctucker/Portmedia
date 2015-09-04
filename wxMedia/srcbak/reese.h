/*
 *  reese.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-02-06.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */

#ifndef REESE_H
#define REESE_H

#include "oscillator.h"

class Reese : public SawOsc
{
private:
	float detune;
	float atten;
public:
	Reese();
	int generate GENERATE(inBuf, outBuf, framesPerBuf, timeInfo, status);
	void setDetune(float);
	void retemper();
};

class SubSineTbl : public WaveTable
{
public:
	 SubSineTbl();
};

class SubSineOsc : public Oscillator
{
private:
	int curNote;
public:
	SubSineOsc();
	void noteOn(int,int);
	void noteOff(int);
};


#endif
