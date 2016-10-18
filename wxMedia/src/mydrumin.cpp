#include "mydrumin.h"

void MyDrumIn::kick(){
	std::cout << "kick";
	canvas->hitDrum(0);
}
void MyDrumIn::snare(){
	std::cout << "snare";
	canvas->hitDrum(1);
}
void MyDrumIn::tom(){
	std::cout << "tom";
	canvas->hitDrum(2);
}
void MyDrumIn::hat(){
	std::cout << "hat";
	canvas->hitDrum(3);
}
void MyDrumIn::ride(){
	std::cout << "ride";
	canvas->hitDrum(4);
}
void MyDrumIn::crash(){
	std::cout << "crash";
	canvas->hitDrum(5);
}

void MyDrumIn::begin()
{
	// called at the beginning of a new tempo
}

void MyDrumIn::listen()
{
	// called to sync with continuing tempo
}



void MyDrumIn::noteOn(int c, int n, int v){
	//if( c != 0x9 ) return;
	
	note = n;
	vel = v;
	
	std::cout << time << "\t";
	
	switch(n)
	{
		case 35: // acoustic
		case 36: // kick
			kick();
			break;
			
		case 37: // side stick
		case 38: // snare
		case 39: // clap
		case 40: // e-snare
			snare();
			break;
			
		case 41: // lo floor tom
		case 43: // hi floor tom
		case 45: // low tom
		case 47: // mid tom
		case 48: // hi-mid tom
		case 50: // hi tom
			tom();
			break;
			
		case 49: // crash 1
		case 52: // chinese
		case 55: // splash
		case 57: // crash 2
		case 58: // vibraslap
			crash();
			break;
		
		case 42: // closed hat
		case 44: // pedal hat
		case 46: // open hat
		case 80: // mute triangle
		case 81: // open triangle
			hat();
			break;
			
		case 51: // ride 1
		case 59: // ride 2
		case 53: // ride bell
		case 54: // tambourine
		case 56: // cowbell
			ride();
			break;
	}
	
	std::cout << " drum on " << n << ":" << v << "\n";
}


