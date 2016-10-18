/*
 *  pan.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2016-09-25.
 *  Copyright 2016. All rights reserved.
 *
 */
#pragma once

#include "main.h"
#include "param.h"

#define RAD45 0.785398163397448
#define HALFSQRT2 0.707106781186548
#define HALFPI 1.5707963267949

class Pan : public Set {

public:
	unsigned int law;
	fl pan;
	fl balance;
	fl ampL, ampR;
	void calcPan();
	void calcLinearPan();
	void calcSinPan();
	void calcPowerPan();
	void calcSqrtPan();
	void calcSubtractPan();
	void setPan(fl bal);
	virtual void SetParam( int n, fl v );

	Pan(fl bal=0.5) :
		balance(bal), law(1)
	{
		AddParam( &pan, "PanB" );
		calcPan();
	}
};
