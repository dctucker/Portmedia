/*
 *  reese.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-02-06.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */
#pragma once

#include "oscillator.h"

class Reese : public SawOsc
{
private:
	float detval;
	float detune;
	float atten;
public:
	Reese();
	int generate GENERATE(inBuf, outBuf, framesPerBuf, timeInfo, status);
	void SetParam(int,fl);
	void setDetune(float);
	void retemper();
	WaveTable *wt;
};

class SubSineTbl : public WaveTable
{
public:
	 SubSineTbl();
};

class SubSineOsc : public Oscillator
{
private:
	int curNote, prevNote;
	double time;
public:
	SubSineOsc();
	void noteOn(int,int);
	void noteOff(int);
	void mark(double);
	void duck();
};
