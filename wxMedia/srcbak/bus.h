/*
 *  bus.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-02-06.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */
#ifndef BUS_H
#define BUS_H

#include <cmath> 
#include "delay.h"

 
#define OUT_CAST( x ) static_cast<float **>((x))

#ifndef SAMPLE_RATE
#define SAMPLE_RATE 96000
#endif

#include "scope.h"
#include "oscillator.h"


class Bus : public Generator {
public:
	Generator *gen[128];
	ScopePanel *sc;
	float *buf[2];
	float *acc[2];

	float atten[128];
	float clip[128];
	int gens;
	float masterVol;
	int generate GENERATE(inBuf, outBuf, framesPerBuf, timeInfo, status);
	float level, dist;
	
	Delay *delay;
	void setDelay(float d=-1,float v=-1);
	
	int numSends;
	float send[128][8];
	float *sendacc[8][2];
	void setSend(int,int,float);
	
	Bus();
	
	void add(Generator *g);
	void setVol(float f);
	void setDist(float f);
	void setClip(int i, float f);
	
	wxPaintEvent *evt;
};


#endif
