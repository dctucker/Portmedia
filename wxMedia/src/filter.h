/*
 *  filter.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-07-08.
 *  Copyright 2010. All rights reserved.
 *
 */
#pragma once

#ifndef M_LN2
#define M_LN2	   0.69314718055994530942
#endif

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

#include <cmath>
#include "param.h"

typedef float smp_type;

class Filter : public Set
{
public:
	smp_type freq,reso,gain;
	Filter()
	{
		AddParam(&freq,"Freq");
		AddParam(&reso,"Reso");
		AddParam(&gain,"Gain");
		freq = 2000;
		reso = 0.707;
		gain = 0;
	}
	virtual void setup(){}
	virtual smp_type filter(smp_type x){ return x; }
	virtual smp_type filter_r(smp_type x){ return x; }
	virtual void mod(smp_type){}
	virtual void reset(){}
	virtual smp_type query(smp_type f){ return f; }
	virtual void set(smp_type f=-1,smp_type q=-1,smp_type g=-1)
	{
		if(f != -1)   freq = f;
		if(q != -1)   reso = q;
		if(g != -1)   gain = g;
		
		setup();
	}
	virtual void SetParam(int n, fl v)
	{
		switch(n)
		{
			case P_Freq:
				freq = v;
				set(v);
				break;
			case P_Reso:
				reso = v;
				set(-1,v);
				break;
			case P_Gain:
				gain = v;
				set(-1,-1,v);
				break;
		}
		setup();
	}

inline static fl lpFreq(fl v){ return 220. * pow(2, v * 6.5); } //* pow(100, v); }
inline static fl hpFreq(fl v){ return 32.5 * pow(2, v * 6.); }// 1. * pow(1000, 0.5 + 0.6 * v); } //30. + v * 2000; } //5 * pow(500, v);
inline static fl lpRes (fl v){ v *= 2; v += 0.5; return 0.2 + v*v; }
inline static fl hpRes (fl v){ return lpRes(v); }
	
inline static fl lpFreq1(fl y){ return log2(y / 220.) / 6.5; }
inline static fl hpFreq1(fl y){ return log2(y / 32.5) / 6.; }
inline static fl lpRes1 (fl y){ return ( sqrt(y - 0.2) - 0.5) / 2.; }
inline static fl hpRes1 (fl y){ return lpRes1(y); }

};
