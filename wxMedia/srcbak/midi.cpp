#include "midi.h"

void MidiController::callback(double dt, MIDIMSG message, void * d){
	time = dt;
	int t=-1, c=-1, v=-1, mt, ch;
	int nBytes = message->size();
	if(nBytes > 0) t = message->at(0);
	if(nBytes > 1) c = message->at(1);
	if(nBytes > 2) v = message->at(2);
	data = d;
	mt = t & 0xF0;
	ch = t & 0x0F;

	switch(mt){
		case 0x80:	noteOff	(ch, c, v);	break;
		case 0x90: if(v != 0)
					noteOn	(ch, c, v);	else
					noteOff	(ch, c, v);	break;
		case 0xA0:	aft		(ch, c, v);	break;
		case 0xB0:	cc		(ch, c, v);	break;
		case 0xC0:	patch	(ch, c);	break;
		case 0xD0:	pressure(ch, c);	break;
		case 0xE0:	bend	(ch, v * 0x80 + c); break; // 8192 is center
		case 0xF0:	sysex	(message);	break;
	}
}

BCR2000::BCR2000 (){
	knob	= new float[128];
	maxKnob	= new float[128];
	minKnob	= new float[128];
	button	= new float[128];
	step	= new float[128];
	for(int i=0; i < 128; i++){
		knob    [i] = 0.0;
		button	[i] = 0.0;
		maxKnob	[i] = 1.0;
		minKnob	[i] = 0.0;
		button	[i] = 0.0;
		step	[i] = 1./128.;
	}
}
BCR2000::~BCR2000(){
	delete knob, maxKnob, minKnob, button;
}

float BCR2000::getButton(int k) const { return button[k]; }

float BCR2000::getKnob(int k) const { return knob[k]; }

void BCR2000::setKnob(int k, float v){
	if(k < 128){
		if(v < minKnob[k]) v = minKnob[k];
		if(v > maxKnob[k]) v = maxKnob[k];
		knob[k] = v;
	}
}

void BCR2000::cc(int ch, int c, int v){
	if(v < 64) {
		knob[c] += step[c] * v;
	} else {
		knob[c] -= step[c] * (128 - v);
	}
	if(knob[c] > maxKnob[c])
		knob[c] = maxKnob[c];
	else if(knob[c] < minKnob[c])
		knob[c] = minKnob[c];

	turn(c, knob[c]);
}
void BCR2000::noteOn(int ch, int n, int v){
	button[n] = step[n] * v;
	//on(n);
	press(n, true);
}
void BCR2000::noteOff(int ch, int n, int v){
	button[n] = step[n] * v;
	press(n, false);
}

void BCR2000::turn(int k, float v){
	//std::cout << "Turn " << k << " = " << v << "\n";
}

void BCR2000::press(int b, bool v){
	//std::cout << "On  " << b << "\n";
}
/*
void BCR2000::off(int b){
	std::cout << "Off " << b << "\n";
}
*/