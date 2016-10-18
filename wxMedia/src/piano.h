/*
 *  piano.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-01-13.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */
#pragma once

#include "wx/progdlg.h"
#include <samplerate.h>
#include <iostream>

#include <sndfile.hh>
#include <stdio.h>
#include "filterIIR00.h"
#include "oscillator.h"

#define SAMPLE_LENGTH (1048576)

// note to self: _virtual_ specification allows method overriding

class UprightLoader: public wxThread
{
public:
	UprightLoader(float ***, bool *);
	virtual void *Entry();
	virtual void OnExit();
	void loadSample(int);
	
	float ***samples;
	bool *loaded;
};


class Upright: public Oscillator //, public WaveTable
{
public:
	Upright();
	~Upright();
	int generate GENERATE(inBuf, outBuf, framesPerBuf, timeInfo, status);	
	//void setFilter(double);
	void noteOn(int,int);
	void retemper(), bend(float), trigger(noteData&);
	virtual void loadSamples();
	void setMix(float);
protected:
	//FilterIIR *filt;
	//FilterIIR **keyfilt;
	//virtual void loadSample(int);
	
	FilterIIR77 *keyfilt;
	float ***samples;
	bool loaded[128];
	float mix, mix0, mix1, mix2;
	EquaTemp tet;
	
	UprightLoader *load;
public:
	virtual void SetParam( int f, fl v );

};

////////////////////////////////////////////////////////////////

class BrassLoader: public wxThread
{
public:
	BrassLoader(float ***, bool *);
	virtual void *Entry();
	virtual void OnExit();
	void loadSample(int, int, double);
	void loadSample(int, wxString &);
	
	float ***samples;
	bool *loaded;
};

class Brass: public Upright
{
public:
	Brass();
	~Brass();
	void loadSamples();
protected:
	BrassLoader *load;
	
};
