#include "chord.h"





//using namespace std;

/*
	V#						3
					
	b3						V
				k3
	
			k1		k4
	
				k2

	k by itself is tonic (I)
*/

/*
class MyGamepad : public DualAction {
public:
	MyKeys *myKeys;
	MyGamepad(wxWindow *parent)
		: DualAction(parent)
	{
	}
	void OnJoystickEvent(wxJoystickEvent &event){
		std::cout << "Gamepad\n";
	}
	void button(int b, int v){
		std::cout << "b " << b << " = " << v << "\n";
		int notes[] = {
			64, 66, 68, 69, 71, 73, 75, 76, 78, 79, 81, 82, 83, 84, 85, 86, 87, 88 };
		int n = notes[b];
		if( v == 1 )
			myKeys->noteOn(1, n, 100);
		else
			myKeys->noteOff(1, n, 100);
	}
	void axis(int a, float v){
		std::cout << "a " << a << " = " << v << "\n";
		//if( a == 0 ) 
		//	myKeys->bend( 1, v * 8192 );
	}
};
*/

