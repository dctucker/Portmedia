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
	}
	return NULL;
}

Temperament *Temperament::getTemp(int i)
{
	if(temps[i] == NULL)
		Temperament::temps[i] = initTemp(i);
	return Temperament::temps[i];
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
	for(int i=0; i < 128; i++){
		freqTable[i] = 440. * eF[(i+3)%12] * noteOctave(i);
	}*/
	init(eF);
}

HammTemp::HammTemp()
{

	/* Tonewheels
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
/*
	float h[12] = {
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
	};
*/
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

KeplTemp::KeplTemp()
{
	float U = 1.0f ,
		cs  = 135. / 128. ,
		jM  =   9. /   8. ,
		m3  =   6. /   5. ,
		M3  =   5. /   4. ,
		P4  =   4. /   3. ,
		P5  =   3. /   2. ,
		jm6 =   8. /   5. ,
		pM6 =  27. /  16. ,
		jm7 =   9. /   5. ,
		jM7 =  15. /   8. ;

	float kF[12] = { 
		U   , cs,
		jM  , m3,
		M3  ,
		P4  , 45./32.,
		P5  , jm6,
		pM6 , jm7,
		jM7
	};
	init(kF);
}

Just5Temp::Just5Temp()
{
	float jF[12] = { 
		 1.0 , 16./15.,
		9./8.,  6./5. ,
		5./4.,
		4./3.,   0.0,
		3./2.,  8./5.,
		5./3., 16./9.,
		15./8.
	};
	float Gb = 64./45.;
	float Fs = 45./32.;

	initJust( jF, Gb, Fs );
}

PythTemp::PythTemp()
{
	if(note == 0) note = 60;
	// all fifths tuned to 3:2
	//Wikipedia
	//					Ab			Eb			Bb		F		C		G		D	A		E		B		F#		C#			G#
	//float pF[13] = { 1024./729., 256./243., 128./81., 32./27., 16./9., 4./3., 1.0, 3./2., 9./8., 27./16., 81./64., 243./128., 729/512. }

	//				 A		A#			B	C			C#		D	wolf	E		F		F#		G		G#
	float pF[12] = { 1.0, 256./243., 9./8., 32./27., 81./64., 4./3., 0.0, 3./2., 128./81., 27./16., 16./9., 243./128. };
	float Ab = 1024./729.;
	float Gs = 729./512.;

	initJust(pF, Ab, Gs);
}


