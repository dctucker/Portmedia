#pragma once

#include "globals.h"
#include "myarduino.h"

// KeyStation-88 implementation
class MyKeys : public MidiController, public Set
{ 
public:
	Oscillator *organ, *square, *tri, *noise;
	SubSineOsc *sub;
	Reese *reese;
	Brass *brass;
	Upright *piano;
	Canvas3D *canvas;
	MyArduino *arduino;
	//MyApp *app;
	
	Oscillator *oscs[NUM_OSCS];
	Bus *bus;
	
	float tempo, bpm;
	int inst, tuning;
	int lastLP;
	float mod, pitch, fader, sustain, shelf;
	bool updating;
	bool subOn, subSus;
	bool keyon[128];
	
	int loSplit, hiSplit;
	bool loSet, hiSet;
	int highNote, lowNote;
	
	bool loAssign[NUM_OSCS], hiAssign[NUM_OSCS];
	
	MyKeys();
	void setOscs();
	
	void SetParam( int n, fl v );
	void setString(const char *s);
	void setTemper(int tun);
	
	inline Oscillator * getOsc(int i)
	{
		switch(i)
		{
			case 0: return sub;
			case 1: return piano;
			case 2: return organ;
			case 3: return reese;
			case 4: return tri;
			case 5: return square;
			case 6: return brass;
			case 7: return noise;
		}
		return NULL;
	}

	
	void duckSub();
	void update();
	void updateFilter(int ins);
	void updateEnvelope(int ins);

	void setShelf(float f);
	void setLoRange(int r);
	void setHiRange(int r);

	void noteOn(int ch, int k, int v);
	void noteOff(int ch, int k, int v);
	void cc(int ch, int c, int v);
	void bend(int ch, int b);
	
	void setDelay(float t);
	void modulate( int k );
	
	chord poss[10];
	int pitch_class[12];
	int minor[12], major[12], dimin[12], augme[12];
	
	void inferChord();
};
