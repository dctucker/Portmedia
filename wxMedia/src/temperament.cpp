#ifndef CMATH 
#define CMATH
#include <cmath>
#endif

#include "temperament.h"


int Temperament::tempCount = 0;
Temperament *Temperament::temps[10];

Temperament *Temperament::initTemp(int i)
{
	switch(i){
		case 0: return new EquaTemp();
		case 1: return new MeanTemp();
		case 2: return new Just5Temp();
		case 3: return new KeplTemp();
		case 4: return new PythTemp();
		case 5: return new HammTemp();
		case 6: return new PtolTemp();
		case 7: return new ChinTemp();
		case 8: return new Dowland();
	}
	return temps[0];
}

Temperament *Temperament::getTemp(int i)
{
	if( i < 10 )
	{
		if(temps[i] == NULL)
		{
			Temperament::temps[i] = initTemp(i);
		}
	}
	else
	{
		return temps[0];
	}
			
	return Temperament::temps[i];
}

void Temperament::nullify()
{
	for(int i=0; i < 10; i++){
		temps[i] = NULL;
	}
}

Temperament::Temperament() : 
		//freqTable(new float[128]),
		//fundTable(new float[12]),
		//intervals(new float[12]),
		freqA(440)
{

}

float Temperament::operator() (int n) {
	if(n <  0 ) return 0;
	if(n > 127) return 127;
	//std::cout << freqTable[n] << " Hz\n";
	return freqTable[n];
}

void Temperament::initFund(){
	for(int i=0; i < 12; i++){
		fundTable[i] = freqTable[69 + i];
		//std::cout << "Init Interval " << i << " = " << fundTable[i] << "\n";
	}
}
void Temperament::initIntervals(float jF[]){
	for(int i=0; i < 12; i++){
		intervals[i] = jF[i];
	}
}
void Temperament::init( float jF[] ){
	initIntervals(jF);
	for(int i=0; i < 128; i++){
		freqTable[i] = freqA * intervals[(i+3) % 12] * noteOctave(i);
	}
	initFund();
}
float Temperament::noteOctave(int i){
	return pow(2., ((i+3) / 12) - 6);
}

void Temperament::initJust(float jF[], float Gb, float Fs) {
	initIntervals(jF);
	loTT = Gb;
	hiTT = Fs;
	int j = 3; // oh no, we're tuning relative to A=440
	for(int i=0; i < 128; i++, j++){
		freqTable[i] = freqA * noteOctave(i);
		
		if(intervals[j % 12] == 0){
			if(i < 62)
				freqTable[i] *= loTT;
			else freqTable[i] *= hiTT;
		} else {
			freqTable[i] *= intervals[j % 12];
		}
	}
	initFund();
}



void Temperament::retune(float A){
	freqA = A;
	init(intervals);
}

void Temperament::modulate( int m ){ // m ε [0..11]
	int mN = 69 + m;
	
	// fails on Just5 .. missing tritone [on upper octave]?
	
	float newCenter = fundTable[m];
	std::cout << "New center: " << newCenter << "\n";
	for(int i = 0; i < 12; i++){
		int n = i + mN;
		freqTable[n] = intervals[i] * newCenter;
		//std::cout << "Interval " << i << " = " << freqTable[n] << "\n";

		n -= 12;
		int o = -1;
		while( n >= 0){
			freqTable[n] = intervals[i] * newCenter * pow(2., o);
			n -= 12;
			o--;
		}

		n = i + mN + 12;
		o = 1;
		while(n < 128){
			freqTable[n] = intervals[i] * newCenter * pow(2., o);
			n += 12;
			o++;
		}
		
	}
}


EquaTemp::EquaTemp() {
	float eF[12];
	for(int i=0; i < 12; i++){
		eF[i] = pow(2., i / 12.);
	}

	/*
	for( int i=0; i < 100; i++){
		std::cout << ( (double) IntervalTable[i].ratio ) << "\n";
	}
	//*/

	/*
	for(int i=0; i < 128; i++){
		freqTable[i] = 440. * eF[(i+3)%12] * noteOctave(i);
	}*/
	init(eF);
}

HammTemp::HammTemp()
{	/* Tonewheels
		N	A	B	Ratio A/B
		C	85	104	0.817307692
		C#	71	82	0.865853659
		D	67	73	0.917808219
		D#	105	108	0.972222222
		E	103	100	1.030000000
		F	84	77	1.090909091
		F#	74	64	1.156250000
		G	98	80	1.225000000
		G#	96	74	1.297297297
		A	88	64	1.375000000
		A#	67	46	1.456521739
		B	108	70	1.542857143
		
		M = 20 Hz
		T = teeth
		R = ratio
		Frequency = M*T*R
		
		Teeth = 2, 4, 8, 16, 32, 64, 128, 192
		 final seven keys in upper octave ^^^
	*/
/*	float h[12] = {
		 85./104.  ,
		 71./82.   ,
		 67./73.   ,
		105./108.  ,
		103./100.  ,
		 84./77.   ,
		 74./64.   ,
		 98./80.   ,
		 96./74.   ,
		 88./64.   ,
		 67./46.   ,
		108./70.
	}; */
	
	float h[12] = {
		 88./64.   , // A = 1.375; 20 * 16 * 1.375 = 440 Hz
		 67./46.   ,
		108./70.   ,

		 85./104.  , // C
		 71./82.   ,
		 67./73.   ,
		105./108.  ,
		103./100.  ,
		 84./77.   ,
		 74./64.   ,
		 98./80.   ,
		 96./74.   
	};
	
	for(int i = 0; i < 12; ++i){
		h[i] /= 1.375;
		if(h[i] < 1.0)
			h[i] *= 2.0;
	}

	init(h);
}

MeanTemp::MeanTemp()
{
	float P = pow(5., 1./4.);
	float T = pow(5., 1./2.) / 2.;
	float S = 8. / pow(5., 5./4.);
	float Z = T / S;

	float mF[12] = {
		1.    ,   Z   ,
		T     ,   T*S ,
		T*T   ,
		T*T*S , T*T*T ,
		P     ,   P*Z ,
		P*T   , P*T*S ,
		P*T*T
	};
	init(mF);
}

Just5Temp::Just5Temp()
{
	// only factors 2,3,5 are used
	float jF[12] = { 
		 1.0  , 16./15.,
		 9./8.,  6./5. ,
		 5./4.,
		 4./3., 45./32.,
		 3./2.,  8./5. ,
		 5./3., 16./9. ,
		15./8.
	};
	float Gb = 64./45.;
	float Fs = 45./32.;

	initJust( jF, Gb, Fs );
}

KeplTemp::KeplTemp()
{
	float U = 1.0f ,
		cs  = 135. / 128. , // lemma = 15:16 / 8:9
		jM  =   9. /   8. , // major whole tone
		m3  =   6. /   5. , // minor third
		M3  =   5. /   4. , // major third
		P4  =   4. /   3. , // perfect fourth
		dT  =  45. /  32. , // augmented fourth
		P5  =   3. /   2. , // perfect fifth
		jm6 =   8. /   5. , // minor sixth
		pM6 =  27. /  16. , // pythagorean major sixth
		jm7 =   9. /   5. , // 16:9 or 9:5	
		jM7 =  15. /   8. ; // 

	float kF[12] = { 
		U   , cs,
		jM  , m3,
		M3  ,
		P4  , dT,
		P5  , jm6,
		pM6 , jm7,
		jM7
	};
	init(kF);
}

PtolTemp::PtolTemp()
{
	float tF[12] = {
		1.0,
		16. / 15.,
		 9. /  8.,
		 6. /  5.,
		 5. /  4.,
		 4. /  3.,
		 7. /  5.,
		 3. /  2.,
		 8. /  5.,
		 5. /  3.,
		 7. /  4.,
		15. /  8.
	};
	init(tF);
}

PythTemp::PythTemp()
{
	if(note == 0) note = 60;
	// all fifths tuned to 3:2
	//Wikipedia
	//					Ab			Eb			Bb		F		C		G		D	A		E		B		F#		C#			G#
	//float pF[13] = { 1024./729., 256./243., 128./81., 32./27., 16./9., 4./3., 1.0, 3./2., 9./8., 27./16., 81./64., 243./128., 729/512. }

	float pF[12] = {
		   1.0   , 256./243.,
		  9./8.  ,  32./27. ,
		 81./64. ,
		  4./3.  , 729./512., //1024./729.,
		  3./2.  , 128./81. ,
		 27./16. ,  16./9.  ,
		243./128.
	};
	float Ab = 1024./729.;
	float Gs = 729./512.;

	initJust(pF, Ab, Gs);
	
}

ChinTemp::ChinTemp()
{
	#define CFRAC( x, y ) pow( 3., x##. ) / pow( 2., y##. )
	// 1: 2/3 length of string fret
	// 2: 4/3 length of stopped string
	// 3: repeat step 1
	/*
	 3⁰  / 2⁰   ,  3⁷  / 2¹¹ ,
	 3²  / 2³   ,  3⁹  / 2¹⁴ ,
	 3⁴  / 2⁶   ,
	 3¹¹ / 2¹⁷  ,  3⁶  / 2⁹  ,
	 3¹  / 2¹   ,  3⁸  / 2¹² ,
	 3³  / 2⁴   ,  3¹⁰ / 2¹⁵ ,
	 3⁵  / 2⁷
	
	*/
	float cF[12] = {
		CFRAC( 0, 0), CFRAC( 7,11),
		CFRAC( 2, 3), CFRAC( 9,14),
		CFRAC( 4, 6),
		CFRAC(11,17), CFRAC( 6, 9),
		CFRAC( 1, 1), CFRAC( 8,12),
		CFRAC( 3, 4), CFRAC(10,15),
		CFRAC( 5, 7)
	};
	/*
	float cF[12] = {
		    1.0    , 2187./2048.,
		   9./8.   , 1968./1638.,
		  81./64.  ,
		1771./1311.,  729./512. ,
		   3./2.   , 6561./4096.,
		  27./16.  , 5905./3277.,
		 243./128.
	};*/
	init(cF);
}

#define FRAC(x,y) x##./y##.
#define FRACSTEM(\
	n1, d1,  n2, d2, \
	n3, d3,  n4, d4, \
	n5, d5, \
	n6, d6,  n7, d7, \
	n8, d8,  n9, d9, \
	na, da,  nb, db, \
	nc, dc  \
	) { \
	FRAC( n1, d1 ), FRAC( n2, d2 ), \
	FRAC( n3, d3 ), FRAC( n4, d4 ), \
	FRAC( n5, d5 ), \
	FRAC( n6, d6 ), FRAC( n7, d7 ), \
	FRAC( n8, d8 ), FRAC( n9, d9 ), \
	FRAC( na, da ), FRAC( nb, db ), \
	FRAC( nc, dc ) }
	

Dowland::Dowland() {
	float cF[12] = FRACSTEM(
		   1,   1,   33, 31,
		   9,   8,   33, 28,
		 264, 211,
		   4,   3,   24, 17,
		   3,   2,   99, 62,
		  27,  16,   99, 56,
		 396, 211 
	);
	init( cF );
}


Kirnberger::Kirnberger(){
	float kF[12] = FRACSTEM(
		    1,   1,   256, 243,
		    9,   8,    32,  27,
		    5,   4, 
		    4,   3,    45,  32,
		    3,   2,   128,  81,
		  270, 161,    16,   9,
		   15,   8 		
	);
	init( kF );
}

/*

To be written: Maqam scales
1. Rast	(C)	T	P
2. Bayat	(C)	 T	 P
3. Sigah	(C)	 T	 P
4. Hijaz Diwan	(C)	 T
5. Saba	(C)	 T
6. 'Ajam' Ushayran1	(C)	 T
7. Husayni

*/