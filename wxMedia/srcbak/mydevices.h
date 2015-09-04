#define NUM_OSCS 8

class MyKeys : public MidiController { // KeyStation-88 implementation         
public:
	Oscillator *organ, *square, *tri, *noise;
	SubSineOsc *sub;
	Reese *reese;
	Brass *brass;
	Upright *piano;
	KeyboardPanel *panel;
	
	Oscillator *oscs[NUM_OSCS];
	Bus *bus;
	
	int lastLP;
	float mod, pitch, fader, sustain;
	bool updating;
	bool subOn, subSus;
	bool keyon[128];
	
	int loSplit, hiSplit;
	bool loSet, hiSet;
	
	bool loAssign[NUM_OSCS], hiAssign[NUM_OSCS];
	
	MyKeys() : 
		loSplit(48), hiSplit(84),
		loSet(false), hiSet(false),
		lastLP(1), subOn(true), subSus(false)
	{
		for(int i = 0; i < NUM_OSCS; ++i ){
			hiAssign[i] = false;
			loAssign[i] = false;
		}
		
		//loAssign[2] = true;
		//hiAssign[3] = true;
	
	}
	
	void setOscs(){
		for(int i=0; i < NUM_OSCS; i++){
			oscs[i] = getOsc(i);
		}
	}
	

	void setTemper(Temperament *t){
		Oscillator *osc;
		for(int i=0; i < NUM_OSCS; ++i){
			osc = oscs[i];
			if( osc != NULL ){
				osc->setTemper(t);
			}
		}
	}
	
	inline Oscillator *getOsc(int i)
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
	
	void update(){
		
		if(!updating){
			updating = true;
			panel->Refresh();
			wxMouseEvent eve(wxEVT_MOTION);
			::wxPostEvent(panel, eve);
			updating = false;
		}
		
	}
	void noteOn(int ch, int k, int v){
		if(k <= 20) // modulation keys
		{
			int m = (k + 3) % 12;
			//std::cout << "Modulate: " << m << "\n";
			organ->modulate( m );
		}

		if( k < loSplit ) // sub sine
		{
			if(subOn) sub->noteOn(k, v);
			if( loAssign[1] and k >= 21)
				piano->noteOn(k, v);
		}
		else if( k < hiSplit )
		{
			//osc->noteOn(k, v);
			//piano->noteOn(k,v);
			for(int i=0; i < NUM_OSCS; ++i){
				if(loAssign[i]){
					oscs[i]->noteOn(k,v);
				}
			}
		}
		else
		{
			for(int i=0; i < NUM_OSCS; ++i){
				if(hiAssign[i])
					if(i == 1)
						piano->noteOn(k,v);
					else
						oscs[(i)]->noteOn(k - 24,v);
			}
		}

		panel->keyOn(k);
		update();
		//std::cout << k;
	}
	void noteOff(int ch, int k, int v){
		panel->keyon[k] = false;
		panel->keyOff(k);

		if( k < loSplit )
		{
			sub->noteOff(k);
			piano->noteOff(k);
		}
		else if( k < hiSplit )
		{
			for(int i=0; i < NUM_OSCS; ++i){
				//if(loAssign[i])
					oscs[(i)]->noteOff(k);
			}
		}
		else
		{
			for(int i=0; i < NUM_OSCS; ++i){
				//if(hiAssign[i])
					if(i == 1)
						piano->noteOff(k);
					else
						oscs[i]->noteOff(k - 24);
			}
		}

		
		if(loSet){
			loSplit = k;
		} else if(hiSet){
			hiSplit = k;
		}
		
		//osc->noteOff(k);
		//piano->noteOff(k);

		update();
	}
	void cc(int ch, int c, int v)
	{
		switch(c)
		{
			
			case 1: // mod wheel
			
				mod = v / 127.; panel->SetMod(mod);
				
				
				for(int i=0; i < 8; ++i){
					if(loAssign[i] or hiAssign[i]){
						oscs[i]->setLFO(mod * mod);
					}
				}
				//oscs[lastLP]->setLowpass( 200 * pow(100, ((float)v/127.)));
			
			
			break;
			
			case 7: // volume fader
			
				fader = v / 128.; panel->SetFader(fader);
				
				//piano->setADSR( -1, -1, -1, fader );
				//piano->setCutoff(fader);
				bus->setVol( fader * fader );
			
			break;
			
			case 64: // sustain pedal
			
				sustain = v / 128.; panel->SetSustain(sustain);
				
				//osc->sustain(v);
				if(subSus) sub->sustain(v);
				for(int i=0; i < NUM_OSCS; ++i)
					if(loAssign[i] or hiAssign[i]){
						oscs[i]->sustain(v);
					}
				//sub->setADSR( -1, -1, -1, 0.25 + 2. * sustain );
				
			break;
			
			default:
			
				std::cout << c;
			break;
		}
		update();
		//osc->setCutoff((v+1) / 129.);
	}
	void bend(int ch, int b){
		pitch = (b - 8192) / 8192.;
		panel->SetPitch(pitch);
		oscs[0]->bend(pitch);
		for(int i = 0; i < NUM_OSCS; ++i){
			if(loAssign[i] or hiAssign[i])
				oscs[i]->bend(pitch);
		}
		//organ->bend(pitch);
		//std::cout << "Bend: " << b << "\n";
		
		update();
	}
	
	void setDelay(float f){
		for(int i=0; i < NUM_OSCS; i++){
			oscs[i]->setDelay(f);
		} 
	}
	

};

using namespace std;

class MyBCR : public BCR2000 { // BCR-2000 implementation
public:
	//Oscillator *sub, *piano, *organ;
	MyKeys *myKeys;
	BCRPanel *panel;
	WavePanel *wave;
	ADSRPanel *envPanel[8];
	FilterPanel *filtPanel[8];
	RtMidiOut *mo;
	std::vector<unsigned char> msg;

	
	int curPage;
	float pages[5][128];
	
	void setLev(float *l){
		panel->setLev(l);
	}
	
	MyBCR() : 
		mo(NULL),
		myKeys(NULL),
		curPage(1)
	{
		
		for(int i=0; i < 128; i++){
			for(int j=0; j < 5; j++){
				pages[j][i] = 0.0;
			}
			button[i] == 0.0;
		}

		// default instrument assignments
		button[93] = 1.0;
		button[84] = 1.0;
		
		// LP freq
		pages[1][11] = 0.265;
		pages[1][12] = 0.406;
		pages[1][13] = 0.422;
		pages[1][14] = 0.305;
		pages[1][15] = 0.367;
		pages[1][16] = 0.328;
		pages[1][17] = 0.453;
		pages[1][18] = 0.422;

		// HP freq
		pages[1][21] = 0.547;
		pages[1][22] = 0.539;
		pages[1][23] = 0.500;
		pages[1][24] = 0.906;
		pages[1][25] = 0.359;
		pages[1][26] = 1.0;
		pages[1][27] = 0.173;
		pages[1][28] = 0.656;
		
		// Volume
		pages[1][31] = 0.312;
		pages[1][32] = 0.500;
		pages[1][33] = 0.234;
		pages[1][34] = 0.305;
		pages[1][35] = 0.305;
		pages[1][36] = 0.359;
		pages[1][37] = 0.297;
		pages[1][38] = 0.500;



		// A                    // D
		pages[1][41] = 0.450 ;  pages[1][51] = 0.500 ;
		pages[1][42] = 0.508 ;  pages[1][52] = 0.267 ;
		pages[1][43] = 0.250 ;  pages[1][53] = 0.500 ;
		pages[1][44] = 0.300 ;  pages[1][54] = 0.383 ;
		pages[1][45] = 0.726 ;  pages[1][55] = 0.000 ;
		pages[1][46] = 0.617 ;  pages[1][56] = 0.000 ;
		pages[1][47] = 0.250 ;  pages[1][57] = 0.000 ;
		pages[1][48] = 0.242 ;  pages[1][58] = 0.430 ;

		// S                    // R
		pages[1][61] = 0.633 ;  pages[1][71] = 0.430 ;
		pages[1][62] = 1.000 ;  pages[1][72] = 0.555 ;
		pages[1][63] = 0.289 ;  pages[1][73] = 0.000 ;
		pages[1][64] = 0.898 ;  pages[1][74] = 0.633 ;
		pages[1][65] = 0.000 ;  pages[1][75] = 0.000 ;
		pages[1][66] = 0.000 ;  pages[1][76] = 0.555 ;
		pages[1][67] = 1.000 ;  pages[1][77] = 0.400 ;
		pages[1][68] = 0.547 ;  pages[1][78] = 0.438 ;




		// LP res
		pages[2][11] = 0.2;
		pages[2][12] = 0.2;
		pages[2][13] = 0.052;
		pages[2][14] = 0.114;
		pages[2][15] = 0.098;
		pages[2][16] = 0.000;
		pages[2][17] = 0.4;
		pages[2][18] = 0.367;

		// HP res
		pages[2][21] = 0.2;
		pages[2][22] = 0.2;
		pages[2][23] = 0.091;
		pages[2][24] = 0.005;
		pages[2][25] = 0.114;
		pages[2][26] = 0.231;
		pages[2][27] = 0.2;
		pages[2][28] = 0.398;

		
		pages[3][31] = 1.0;
		pages[3][32] = 1.0;
		pages[3][33] = 1.0;
		pages[3][34] = 1.0;
		pages[3][35] = 1.0;
		pages[3][36] = 1.0;
		pages[3][37] = 1.0;
		pages[3][38] = 1.0;
		
		pages[4][41] = 0.461;

		msg.push_back(0x00);
		msg.push_back(0x00);
		msg.push_back(0x00);
		
	}
	
	void send(unsigned char a, unsigned char b, unsigned char c){
		msg[0] = a;
		msg[1] = b;
		msg[2] = c;
		mo->sendMessage(&msg);		
	}
	

	void sendValues(){
		if( mo ){
			for(int i=40; i < 128; ++i){
				send(0xBF, i, 127 * knob[i]);
			}
			for(int i=0; i <= 98; ++i){
				send(0x9F, i, button[i] ? 0x7F : 0x00);
			}
		}
	}
	
	void execValues(){
		for(int p=0;  p <=4; p++){
			for(int i=0; i < 128; i++){
				turnPage(p, i, pages[p][i]);
			}
		}
		/*
		for(int i=40; i < 128; ++i){
			turn(i, knob[i]);
		}*/
		
		for(int i=0; i <= 98; ++i){
			press(i, button[i]);
		}
	}

	friend ostream &operator<<(ostream &stream, MyBCR &bcr){
		for(int p=0; p <= 4; ++p){
			for(int i=0; i < 128; ++i){
				if( i % 10 == 1 )
					stream << "\n";
				else
					stream << "\t";
				stream << bcr.pages[p][i];
			}
		}
		for(int i=0; i <= 98; ++i){
			if( i % 10 == 1 )
				stream << "\n";
			else
				stream << "\t";
			stream << bcr.button[i];
		}
		stream << "\n" << bcr.myKeys->loSplit << "\t" << bcr.myKeys->hiSplit;
		stream << "\n";
		return stream;
	}
	
	friend istream &operator>>(istream &stream, MyBCR &bcr){
		for(int p=0; p <=4; ++p){
			for(int i=0; i < 128; ++i){
				stream >> bcr.pages[p][i];
			}
		}
		for(int i=0; i <= 98; ++i){
			stream >> bcr.button[i];
		}
		stream >> bcr.myKeys->loSplit;
		stream >> bcr.myKeys->hiSplit;
		return stream;
	}
	


	
	void update(){
		if(panel) panel->Refresh();
		wxMouseEvent eve(wxEVT_MOTION);
		::wxPostEvent(panel, eve);
	}
	
	float lpFreq(float v){
		return 200. * pow(100, v);
	}
	float hpFreq(float v){
		return 30.0 + v * 2000; //5 * pow(500, v);
	}
	
	void turnPage( int p, int k, float v){
		float f;
		int inst = (k - 1) % 10;
		pages[p][k] = v;

		if( p == 1 ) // volume, lowpass and highpass frequency
		{
		
			switch( k )
			{
			case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 18: 
					
				f = lpFreq(v);
				myKeys->oscs[inst]->setLowpass( f );
				//myKeys->lastLP = inst;
				break;
				
			case 21: case 22: case 23: case 24: case 25: case 26: case 27: case 28: 
				
				f = hpFreq(v);
				myKeys->oscs[inst]->setHighpass( f );
				break;

			case 31: case 32: case 33: case 34: case 35: case 36: case 37: case 38:
				myKeys->oscs[inst]->setAmp(v * v);
				break;
			
			
			// ADSR
			case 41: case 42: case 43: case 44: case 45: case 46:case 47:case 48:
				myKeys->oscs[inst]->setADSR(v);
				break;

			case 51: case 52: case 53: case 54: case 55: case 56:case 57:case 58:
				myKeys->oscs[inst]->setADSR(-1,v);
				break;
				
			case 61: case 62: case 63: case 64: case 65: case 66:case 67:case 68:
				myKeys->oscs[inst]->setADSR(-1,-1,v);
				break;
				
			case 71: case 72: case 73: case 74: case 75: case 76:case 77:case 78:
				myKeys->oscs[inst]->setADSR(-1,-1,-1,v);
				break;
			}
			
			if( inst >= 0 and inst < 8 ){
				if( k > 40 and k < 79 )
					envPanel[inst]->set(pages[1][41+inst], pages[1][51+inst], pages[1][61+inst], pages[1][71+inst]);
				else if( k > 10 and k < 40 )
					filtPanel[inst]->set(
						hpFreq(pages[1][21+inst]),
						pages[2][21+inst], 
						lpFreq(pages[1][11+inst]),
						pages[2][11+inst]
					);
			}
		}
		else if( p == 2 )
		{
			
		// compression, lowpass and highpass resonance
		
			switch( k ){
			case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 18: 
			
				f = 1 - v * 0.95;
				myKeys->oscs[inst]->setLowpass( -1, f);
				break;

			case 21: case 22: case 23: case 24: case 25: case 26: case 27: case 28: 

				f = 1 - v * 0.95;
				myKeys->oscs[inst]->setHighpass( -1, f );
				break;

			case 31: case 32: case 33: case 34: case 35: case 36: case 37: case 38: 

				myKeys->bus->setClip(inst, v * v);
				break;
			}
			
			if( inst >= 0 and inst < 8 ){
				if( k > 10 and k < 30 ){
					filtPanel[inst]->set(
						hpFreq(pages[1][21+inst]),
						pages[2][21+inst], 
						lpFreq(pages[1][11+inst]),
						pages[2][11+inst]
					);
				}
			}


		}
		else if( p == 3 ) // LFO (for now)
		{
			switch(k)
			{
			case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 18: 
				if( v < 1.0f )
					myKeys->oscs[inst]->setLFO2LP(v);
				break;
				
			case 21: case 22: case 23: case 24: case 25: case 26: case 27: case 28: 
				
				myKeys->oscs[inst]->setLFO( -1, 1.0f + v * 20.0f );
				break;
			
			case 31: case 32: case 33: case 34: case 35: case 36: case 37: case 38: 

				if( v < 1.0f )
					myKeys->oscs[inst]->setLFO2Amp(v);
				break;
				
			}
		}
		else if( p == 4 ) // effects
		{
		
			switch(k)
			{
			case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 18: 

				//myKeys->bus->setSend( inst, 0, v*v ); // delay send
				myKeys->oscs[inst]->setDelay( -1,  v * v );
				break;
			
			case 31: case 32: case 33: case 34: case 35: case 36: case 37: case 38: 
				
				myKeys->oscs[inst]->setDist(1.0 - v * 0.9); // low as 0.1
				break;
				
			case 24:
				myKeys->reese->setDetune(v);
				break;
			
			case 25:			
				break;
			
			case 26:
				break;
				
			}
		}
	}
	
	void turn(int k, float v)
	{
		turnPage(curPage, k, v);
		update();
	}
	
	void press(int b, bool v)
	{
		switch(b)
		{
			case 81: // set hiSplit
				myKeys->hiSet = v;
				break;

			case 91: // set loSplit
				myKeys->loSet = v;
				break;
			
			case 82: case 83: case 84: case 85: case 86: case 87: case 88:
				myKeys->hiAssign[(b - 1) % 10] = v;
				break;
				
			case 92: case 93: case 94: case 95: case 96: case 97: case 98:
				myKeys->loAssign[(b - 1) % 10] = v;
				break;
				
			case 42:
				if(v == 1.0) myKeys->piano->loadSamples();
				break;
				
			case 47:
				if(v == 1.0) myKeys->brass->loadSamples();
				break;
				
				
			// mode selection
			case 104: // effects mode
			
				// tap tempo
				if( curPage == 4 and time < 2.0 ){
					myKeys->setDelay(time);
				}
				//continue;
				
			case 101: // amplitude mode
			case 102: // frequency mode
			case 103: // modulation mode
				curPage = b - 100;
				
				//activate the proper light
				for(int i = 1; i <= 4; i++){
					if(i == curPage){
						send(0x9F, b, 0x7F);
					} else {
						send(0x9F, 100 + i, 0x00);
					}
				}
				
				// set and send knob assignments for page
				for(int i = 0; i < 128; ++i){
					float v = pages[curPage][i];
					knob[i] = v;
					send(0xBF, i, v * 127);
				}
				
				break;
			
			case 111:
				myKeys->subSus = (v > 0.5 );
				break;
			case 112:
				myKeys->subOn = ( v > 0.5 );
				break;
		}
		//std::cout << b << "\n";
		//osc->noteOn(b, 100);
		update();
	}
};
