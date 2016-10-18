#include "globals.h"
#include "mykeys.h"

class MyDrumIn : public MidiController {
private:
	int note, vel;
	int e16, e2, a16, a2;
	
public:
	MyKeys *myKeys;	
	Canvas3D *canvas;

	void kick();
	void snare();
	void tom();
	void hat();
	void ride();
	void crash();
	
	void begin();
	void listen();
	void noteOn(int c, int n, int v);
};


