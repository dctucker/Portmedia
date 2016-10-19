/*
 *  pan.cpp
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2016-09-25.
 *  Copyright 2016. All rights reserved.
 *
 */

#include "pan.h"
#include "math.h"

void Pan::setPan(fl bal)
{
	pan = bal;
	calcPan();
}

void Pan::calcPan()
{
	switch( law ){
		case 0:
			calcLinearPan();
			break;
		case 1:
			calcSinPan();
			break;
		case 2:
			calcPowerPan();
			break;
		case 3:
			calcSqrtPan();
			break;
		case 4:
			calcSubtractPan();
			break;
	}
}

void Pan::calcLinearPan()
{
	balance = pan;

	ampL = 1. - balance;
	ampR = balance;
}

void Pan::calcSubtractPan()
{
	balance = 2. * pan - 1.;

	ampL = 1. - balance;
	if( ampL > 1. )
		ampL = 1.;
	ampR = 1. + balance;
	if( ampR > 1. )
		ampR = 1.;
}

void Pan::calcSinPan()
{
	balance = HALFPI * pan;

	ampL = cosf( balance );
	ampR = sinf( balance );
}

void Pan::calcPowerPan()
{
	balance = RAD45 * ( 2. * pan - 1. );

	fl sinth = sinf( balance );
	fl costh = cosf( balance );
	
	ampL = HALFSQRT2 * ( costh - sinth );
	ampR = HALFSQRT2 * ( costh + sinth );
}

void Pan::calcSqrtPan()
{
	balance = pan;
	ampL = sqrt( 1 - balance );
	ampR = sqrt( balance );
}

void Pan::SetParam( int n, fl v )
{
	switch( n )
	{
		case P_PanB:
			setPan( v );
			break;
	}
}
