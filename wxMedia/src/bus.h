/*
 *  bus.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-02-06.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */
#pragma once

#include <cmath> 

 
#define OUT_CAST( x ) static_cast<float **>((x))

#ifndef SAMPLE_RATE
#define SAMPLE_RATE 96000
#endif

//#include "delay.h"
//#include "scope.h"
#include "oscillator.h"
//#include "compress.h"
//#include "canvas3d.h"

class Compressor : public Generator
{
public:
	Compressor() :
		env(0.0),
		attack_coef(  exp(log(0.01)/( 0.004 * SAMPLE_RATE ) ) ),
		release_coef( exp(log(0.01)/( 0.400 * SAMPLE_RATE ) ) ),
		ratio(4.0f),
		thresh( powf( 10.f, -12.f / 20.f ) )
	{
	}
	
	fl compress( fl sig )
	{
		return compress(sig, sig);
	}
	
	fl compress( fl sig, fl side )
	{
		return sig;
	}

	float thresh;
	float env;
	float rele;
	float atte;
	float inp;
	float ratio;
	float attack_coef, release_coef;
	bool release;
	float cmp, ds, tdB, cmp1, cmp2, cmp3, cmp4;;

};


class Bus : public Generator {
public:
	Generator *gen[128];
	//Compress *compress;
	//ScopePanel *sc;
	//Canvas3D *sc;
	float *buf[2];
	float *acc[2];

	float atten[128];
	float clip[128];
	int gens;
	float masterVol, gain, masterVolEnv;
	int count;
	int generate GENERATE(inBuf, outBuf, framesPerBuf, timeInfo, status);
	float dist;
	
	//Delay *delay;
	void setDelay(float d=-1,float v=-1);
	
	BiQuad *filtLP, *filtHP;
	float prev_v;
	
	int numSends;
	float send[128][8];
	float *sendacc[8][2];
	void setSend(int,int,float);
	
	float thresh;
	float env;
	float rele;
	float atte;
	float inp;
	float ratio;
	float attack_coef, release_coef;
	bool release;
	float cmp, ds, tdB, cmp1, cmp2, cmp3, cmp4;;
	
	
	Bus();
	~Bus();
	
	fl queryFilter(fl);
	
	void add(Generator *g);
	void setVol(float f);
	void setDist(float f);
	void setClip(int i, float f);
	void setFilter(float v);
	
	wxPaintEvent *evt;
	
	float minmaxv[SAMPLE_RATE];
	int pos, width;

};
