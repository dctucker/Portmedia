#include <iostream>

#ifndef TEMPERAMENT_H
#define TEMPERAMENT_H

class Temperament 
{
protected:
	int note;

	//float *freqTable, *fundTable, *intervals;
	float freqTable[128], fundTable[128], intervals[15];
	float freqA, loTT, hiTT;
	Temperament();

public:
	virtual float operator()(int n);
	void  retune(float);
	void  initFund();
	void  initIntervals(float jF[]);
	void  init( float jF[] );
	float noteOctave(int i);
	void  modulate(int);

	void initJust(float jF[], float Gb, float Fs);

	int static tempCount;	
	static Temperament *temps[10];
	static Temperament *getTemp(int i);
	static Temperament *initTemp(int i);
	static void nullify();
};

#define SUBTEMPER(X)				\
class X : public Temperament {		\
public:								\
	X ();
#define ENDSUB };

SUBTEMPER( EquaTemp  ) ENDSUB
SUBTEMPER( MeanTemp  ) ENDSUB
SUBTEMPER( Just5Temp ) ENDSUB
SUBTEMPER( KeplTemp  ) ENDSUB
SUBTEMPER( PythTemp  )
	PythTemp(int);
ENDSUB
SUBTEMPER( HammTemp  ) ENDSUB
SUBTEMPER( ChinTemp  ) ENDSUB
SUBTEMPER( PtolTemp  ) ENDSUB
SUBTEMPER( Dowland   ) ENDSUB
SUBTEMPER( Kirnberger) ENDSUB
// form of equation:  B1 ^ ( N ^ B2 ) / D   or  (N ^ B1) / (D / B2)
/*

	2 ^ (1 / 1200)  : 1
	(3 ^ 53) / (2 ^ 84)

*/

using namespace std;


#endif


