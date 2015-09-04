/*
 *  ladder.cpp
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-07-08.
 *  Copyright 2010 Moog. All rights reserved.
 *
 */

#include "ladder.h"

Ladder::Ladder()
{
	Fc = 4000, r = 0.5; //, g = 1.0;
	reset();
	setup();
}

void Ladder::setup()
{
	g = 1. - expf( -2. * M_PI * (Fc / Fs) );
	Vt2 = 2. * Vt;
	IctlCFs = g * Vt2; //ICtl / (C * Fs);
}

smp_type Ladder::filter(smp_type x)
{
	Wd = tanhf( (x - 4. * r * yd1) / Vt2 );
	ya = ya1 + IctlCFs * ( Wd - Wa1 ); Wa = tanhf( ya / Vt2 );
	yb = yb1 + IctlCFs * ( Wa - Wb1 ); Wb = tanhf( yb / Vt2 );
	yc = yc1 + IctlCFs * ( Wb - Wc1 ); Wc = tanhf( yc / Vt2 );
	yd = yd1 + IctlCFs * ( Wc - tanhf( yd1 / Vt2 ) );

	ya1 = ya;
	yb1 = yb;
	yc1 = yc;
	yd1 = yd;
	Wa1 = Wa;
	Wb1 = Wb;
	Wc1 = Wc;

	y = yd;
	return y;
}

smp_type Ladder::filter_r(smp_type x)
{
	return y;
}

void Ladder::mod(smp_type m)
{

}

void Ladder::reset()
{
	ya  =0, yb  =0, yc  =0, yd  =0, y =0,
	ya1 =0, yb1 =0, yc1 =0, yd1 =0,
	Wa  =0, Wb  =0, Wc  =0,
	Wa1 =0, Wb1 =0, Wc1 =0;

}

smp_type Ladder::query(smp_type f)
{
	return (f < Fc) ? 0. : -600.;
}

void Ladder::set(smp_type f,smp_type q,smp_type gn)
{
	if( f != -1 ) Fc = f;
	if( q != -1 and q < 1.0 )  r = q;
	setup();
}
