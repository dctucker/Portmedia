/*
 *  piano.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-01-13.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */

#ifndef PIANO_H
#define PIANO_H

#include <wx/progdlg.h>
#include <samplerate.h>
#include <iostream>

#include <sndfile.hh>
#include <stdio.h>
#include "oscillator.h"
#include "filter.h"
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


class Upright: public Oscillator, public WaveTable
{
public:
	Upright();
	~Upright();
	int generate GENERATE(inBuf, outBuf, framesPerBuf, timeInfo, status);	
	void setFilter(double);
	void noteOn(int,int);
	void retemper(), bend(float);
	virtual void loadSamples();
protected:
	//FilterIIR *filt;
	//FilterIIR **keyfilt;
	virtual void loadSample(int);
	FilterIIR77 *keyfilt;
	float ***samples;
	bool loaded[128];
	EquaTemp tet;
	
	UprightLoader *load;
};

class Brass: public Upright
{
public:
	Brass();
	~Brass();
	void loadSamples();
protected:
	void loadSample(int);
	void loadSample(int, int, double);
	void loadSample(int, wxString &);
	
};

#endif
