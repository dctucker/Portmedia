#define NUM_OSCS 8
#include "chord.h"

// lights controller
class MyArduino : public Arduino
{
private:
	bool _sustain;
	int down;
public:
	MyArduino() : Arduino(), _sustain(false), down(0)
	{

	}
	void Init(wxString &str)
	{
		Arduino::Init(str.mb_str(), 115200);
	}

	void root(int n)
	{
		down++;
		WriteByte( 1 + (n % 12) );
	}

	void off()
	{
		down--;
		if( !_sustain && down <= 0 )
			WriteByte(0);
		else
			WriteByte('_');
	}

	void bend(float pitch)
	{
		float x = 360 + pitch * 360.;
		int pit = (int)ceil(x);
		wxString p = wxString::Format(_T("%iB"), pit);
		//cout << p;
		Write(p.mb_str());
	}

	void filter(float pos)
	{
		float x = pos * 2.;
		int po = (int)x;
		wxString p = wxString::Format(_T("%iW"), po);
		//cout << p;
		Write(p.mb_str());
	}

	void sustain(int sus)
	{
		_sustain = sus != 0;
	}
};

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

	
	MyKeys() : 
		loSplit(48), hiSplit(84), shelf(0.5),
		loSet(false), hiSet(false), inst(0),
		lastLP(1), subOn(true), subSus(false),
		canvas(NULL)
	{
		for(int i = 0; i < NUM_OSCS; ++i ){
			hiAssign[i] = false;
			loAssign[i] = false;
		}
		//canvas->active[0] = subOn;

		//loAssign[2] = true;
		//hiAssign[3] = true;
		
	
	}
	void setOscs()
	{
		for(int i=0; i < NUM_OSCS; i++)
		{
			oscs[i] = getOsc(i);
		}
		AddSet( sub,    "Sub" );
		AddSet( piano,  "Pian" );
		AddSet( organ,  "Hamm" );
		AddSet( reese,  "Rees" );
		AddSet( tri,    "Tria" );
		AddSet( square, "Squa" );
		AddSet( brass,  "Bras" );
		AddSet( noise,  "Nois" );

		AddParam( &loSplit, "LoSp" );
		AddParam( &hiSplit, "HiSp" );
		AddParam( &tuning,  "Tune" );
		AddParam( &shelf,   "Shlf" );
	}
	
	// note, for efficiency, these values are coded as longs instead of four-byte strings.
	// Endianness is not preserved here.
	void SetParam( int n, fl v ) 
	{
		switch( n )
		{
			case 0x70536F4C:  // LoSp
				setLoRange( v );
				break;
			case 0x70536948:  // HiSp
				setHiRange( v );
				break;
			case 0x666C6853:  // Shlf
				setShelf( v );
				break;
		}
	}
	
	void setString(const char *s)
	{
		canvas->keyOn(1);
	}

	void setTemper(int tun)
	{
		Oscillator *osc = NULL;
		Temperament *t = Temperament::getTemp(tun);
		tuning = tun;
		for(int i=0; i < NUM_OSCS; ++i)
		{
			if( oscs[i] != NULL )
			{
				oscs[i]->setTemper(t);

				if( oscs[i]->loAssign or oscs[i]->hiAssign )
				{
					osc = oscs[i];
					/* // unstable
					if( osc->chord_ratio != 1.0 )
					{
						if( osc != NULL and canvas != NULL )
						{
							osc->calc_chord_ratio();
							canvas->chord_ratio = osc->chord_ratio;
						}
						
					}
					*/
				}
						
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
	
	void duckSub()
	{
		sub->duck();
	}
	
	void update()
	{
		if(!updating)
		{
			updating = true;
			//panel->Refresh();
			//wxMouseEvent eve(wxEVT_MOTION);
			//::wxPostEvent(panel, eve);
			updating = false;
		}
		
	}
	
		
	void updateFilter(int ins)
	{
		if( ins >= 0 )
		{
			Oscillator *osc = getOsc(ins);
			int i = 0;
			for(float f = 15.625f; f < 18000; f *= 2 )
			{
				for( int g = 0; g < 12 and i < 128; g++ ){
					float ef = f * powf(2., g / 12.f);
					float y = osc->queryFilter(ef);
					y += bus->queryFilter(ef);
					if( y < -36 ) y = -36;
					canvas->updateFilter(ins, i, y);
					//std::cout << i << " " << ef << "\t" << y << "\n";
					++i;
				}
			}
			for( ; i < 128; i++){
				canvas->updateFilter(ins, i, -36);
			}
			//std::cout << "\n";
		} 
		else 
		{
			for(int i=7; i >= 0; i--)
			{
				if( i != inst )
					updateFilter(i);
			}
			updateFilter(inst);
		}
	}
	
	void updateEnvelope(int ins)
	{
		//Oscillator *osc = getOsc(ins);
		/*
		canvas->updateEnvelope( ins, 
			osc->notes[0]->ampEnv->A,
			osc->notes[0]->ampEnv->D,
			osc->notes[0]->ampEnv->S,
			osc->notes[0]->ampEnv->R
		);
		*/
		
	}

	void setShelf(float f){
		shelf = f;
	}
	
	void setLoRange(int r)
	{
		loSplit = r;
		canvas->loSplit = r;
	}
	void setHiRange(int r)
	{
		hiSplit = r;
		canvas->hiSplit = r;
	}
	

	void noteOn(int ch, int k, int v)
	{
		Oscillator *osc = NULL;
		if(k <= 20) // modulation keys
		{
			modulate(k);
		}		
		else if( k < loSplit ) // sub sine
		{
			if(subOn)
			{
				sub->mark(time);
				sub->noteOn(k, v);
			}
			
			if( oscs[1]->loAssign and k >= 21)
				piano->noteOn(k, v);
				
			if( hiSet )
			{
				modulate(k);
			}

			// activate other oscillators with shelf
			for(int i=0; i < NUM_OSCS; ++i)
			{
				if(oscs[i]->loAssign)
				{
					osc = oscs[i];
					osc->noteOn(k, v * shelf);
				}
			}

			// hook for lighting via serial port goes here
			arduino->root(k);
		}
		else if( k < hiSplit )
		{
			//osc->noteOn(k, v);
			//piano->noteOn(k,v);
			for(int i=0; i < NUM_OSCS; ++i)
			{
				if(oscs[i]->loAssign){
					osc = oscs[i];
					osc->noteOn(k,v);
				}
			}
		}
		else
		{
			for(int i=0; i < NUM_OSCS; ++i)
			{
				if( oscs[i]->hiAssign )
				{
					if(i == 1)
					{
						osc = piano;
						piano->noteOn(k,v);
					}
					else
					{
						osc = oscs[i];
						osc->noteOn(k - 24,v);
					}
				}
			}
		}

		canvas->keyOn(k);

		if(highNote < k) highNote = k;
		if(lowNote  > k) lowNote  = k;

		/* // unstable
		if( osc != NULL )
		{
			osc->calc_chord_ratio();
			canvas->chord_ratio = osc->chord_ratio;
			
		}
		//std::cout << "on " << osc->chord_ratio << "\n";
		//*/
		
		//inferChord();
		//update();
		
		//std::cout << k;
	}
	void noteOff(int ch, int k, int v)
	{
		Oscillator *osc = NULL;
		canvas->keyon[k] = false;
		canvas->keyOff(k);

		if( k < loSplit ) // lower range - sub
		{
			sub->mark(time);
			sub->noteOff(k);
			for(int i=0; i < NUM_OSCS; ++i){
				if( oscs[i]->loAssign )
					osc = oscs[i];
				oscs[i]->noteOff(k);
			}
			arduino->off();
		}
		else if( k < hiSplit ) // mid range
		{
			
			for(int i=0; i < NUM_OSCS; ++i)
			{
				if( oscs[i]->loAssign )
				{
					osc = oscs[i];
					oscs[i]->noteOff(k);
				}
			}
		}
		else // higher range
		{
			for(int i=0; i < NUM_OSCS; ++i)
			{
				if( oscs[i]->hiAssign )
				{
					if(i == 1)
					{
						osc = oscs[i];
						piano->noteOff(k);
					}
					else
					{
						osc = oscs[i];
						osc->noteOff(k - 24);
					}
				}
			}
		}

		if( loSet )
			setLoRange(k);
		
		else if( hiSet and k > loSplit )
			setHiRange(k);

		//osc->noteOff(k);
		//piano->noteOff(k);
		
		if(highNote == k)
			while( highNote >  0  and keyon[highNote] == 0 )
				highNote--;
		if(lowNote  == k)
			while( lowNote  < 127 and keyon[lowNote ] == 0 )
				lowNote++;

		
		///* // unstable
		if( osc != NULL ){
			osc->calc_chord_ratio();
			canvas->chord_ratio = osc->chord_ratio;
		}
		//*/
		
		//std::cout << "off " << osc->chord_ratio << "\n";

		update();
	}
	void cc(int ch, int c, int v)
	{
		switch(c)
		{
			
			case 1: // mod wheel
			
				mod = (127. - v) / 127.;
				canvas->SetMod(mod);				
				bus->setFilter(mod);
				arduino->filter(127 - v);
				updateFilter(-1);
							
			break;
			
			case 7: // volume fader
			
				fader = v / 127.;
				canvas->SetFader(fader);				
				bus->setVol( fader );
			
			break;

			case 11: // expression controller

				//reese->setDetune( max(16,v) / 127. );

			break;
			
			case 64: // sustain pedal
			
				sustain = v / 128.;
				canvas->SetSustain(sustain);
				
				if(subSus) sub->sustain(v);
				for(int i=0; i < NUM_OSCS; ++i)
				{
					if( oscs[i]->loAssign or oscs[i]->hiAssign ){
						oscs[i]->sustain(v);
					}
				}
				arduino->sustain(v);
				
			break;
		}
		update();
		//osc->setCutoff((v+1) / 129.);
	}
	void bend(int ch, int b)
	{
		pitch = (b - 8192) / 8192.;
		canvas->SetPitch(pitch);
		arduino->bend(pitch);
		
		oscs[0]->bend(pitch);
		for(int i = 0; i < NUM_OSCS; ++i){
			if( oscs[i]->loAssign or oscs[i]->hiAssign )
				oscs[i]->bend(pitch);
		}
		//organ->bend(pitch);
		//std::cout << "Bend: " << b << "\n";
		
		update();
	}
	
	void setDelay(float t)
	{
		tempo = t;
		bpm = 60.f / tempo;

		for(int i=0; i < NUM_OSCS; i++)
		{
			//oscs[i]->setDelay(t);
			oscs[i]->delay->SetParam( PN("Time"), t * SAMPLE_RATE );
		}
		
		canvas->bpm = bpm;
	}
	
	void modulate( int k )
	{
		int m = (k + 3) % 12;
		std::cout << "Modulating to " << m;
		oscs[0]->modulate(m);
		for(int i=1; i < NUM_OSCS; ++i){
			if( oscs[i] != NULL and (oscs[i]->loAssign or oscs[i]->hiAssign) )
				oscs[i]->modulate( m );
		}
	}
	
	chord poss[10];
	int pitch_class[12];
	int minor[12], major[12], dimin[12], augme[12];
	
	void inferChord() // needs to be heavily optimized
	{
		int i;

		int bass_note = -1;
		int note_count = 0;
		

		for(i=0; i < 12; i++) pitch_class[i] = 0;
		// find bass note and collect pitch class frequency values
		for(i = lowNote; i <= highNote; i++)
		{
			if( canvas->keyon[i] )
			{
				pitch_class[i % 12] += canvas->keyon[i] ? 1 : 0;
				if( bass_note == -1 )
				{
					bass_note = i % 12;
				}
				++note_count;
			}
		}
		
		if( note_count < 3 ) return;
		
		// determine major, minor, diminished, augmented
		for(i=0; i < 12; i++)
		{
			int m = (i + 3) % 12;
			int M = (i + 4) % 12;
			int T = (i + 6) % 12;
			int P = (i + 7) % 12;
			int A = (i + 8) % 12;
			
			minor[i] = pitch_class[i] * pitch_class[m] * pitch_class[P];
			major[i] = pitch_class[i] * pitch_class[M] * pitch_class[P];
			dimin[i] = pitch_class[i] * pitch_class[m] * pitch_class[T];
			augme[i] = pitch_class[i] * pitch_class[M] * pitch_class[A];
		}
		
		
		int pos = 0;
		for(i=0; pos < 10 && i < 12; i++)
		{
			if( major[i] > 0 )
			{
				poss[pos].type = 0;
				poss[pos].root = i;
				poss[pos].add1 = poss[pos].add2 = 0;
				poss[pos].weight = major[i];
				if( i == bass_note ) poss[pos].weight *= 2;
				++pos;
			}
			if( minor[i] > 0 )
			{
				poss[pos].type = 1;
				poss[pos].root = i;
				poss[pos].add1 = poss[pos].add2 = 0;
				poss[pos].weight = minor[i];
				if( i == bass_note ) poss[pos].weight *= 2;
				++pos;
			}
			if( dimin[i] > 0 )
			{
				poss[pos].type = 2;
				poss[pos].root = i;
				poss[pos].add1 = poss[pos].add2 = 0;
				poss[pos].weight = dimin[i];
				if( i == bass_note ) poss[pos].weight *= 2;
				++pos;
			}
			if( augme[i] > 0 )
			{
				poss[pos].type = 3;
				poss[pos].root = i;
				poss[pos].add1 = poss[pos].add2 = 0;
				poss[pos].weight = augme[i];
				if( i == bass_note ) poss[pos].weight *= 2;
				++pos;
			}
		}
		
		int maxi = 0, maxw = 0;
		for(i = 0; i < pos; i++)
		{
			if( poss[i].weight > maxw )
			{
				maxw = poss[i].weight;
				maxi = i;
				
			}
		}
		
		i = maxi;

		int r = poss[i].root;
		int
			a9  = pitch_class[( r +  2 ) % 12],
			sus = pitch_class[( r +  5 ) % 12],
			m6  = pitch_class[( r +  8 ) % 12],
			M6  = pitch_class[( r +  9 ) % 12],
			m7  = pitch_class[( r + 10 ) % 12],
			M7  = pitch_class[( r + 11 ) % 12];

		if( a9 > 0 )
			poss[i].add1 = 9;
		
		else if( sus > 0 )
			poss[pos].add1 = 4;
		
		if( ( m6 > 0 or M6 > 0 ) and ( poss[i].type != 3 ) )
			poss[i].add2 = 6;
		
		else if( m7 > 0 or M7 > 0 )
			poss[i].add2 = 7;

		poss[i].bass = bass_note;
		canvas->curChord = poss[i];
	}	
};



class MyDrumIn : public MidiController {
private:
	int note, vel;
	int e16, e2, a16, a2;
	
public:
	MyKeys *myKeys;	
	Canvas3D *canvas;

	void kick(){
		std::cout << "kick";
		canvas->hitDrum(0);
	}
	void snare(){
		std::cout << "snare";
		canvas->hitDrum(1);
	}
	void tom(){
		std::cout << "tom";
		canvas->hitDrum(2);
	}
	void hat(){
		std::cout << "hat";
		canvas->hitDrum(3);
	}
	void ride(){
		std::cout << "ride";
		canvas->hitDrum(4);
	}
	void crash(){
		std::cout << "crash";
		canvas->hitDrum(5);
	}
	
	void begin()
	{
		// called at the beginning of a new tempo
	}
	
	void listen()
	{
		// called to sync with continuing tempo
	}
	
	
	
	void noteOn(int c, int n, int v){
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
};


class MyBCR : public BCR2000 { // BCR-2000 implementation
public:
	//Oscillator *sub, *piano, *organ;
	MyKeys *myKeys;
	RtMidiOut *mo;
	std::vector<unsigned char> msg;
	bool loading;
	int inst;

	
	int curPage;
	float pages[5][128];
	
	Param *param[5][128];
	Set *set[5][128];
 
	MyBCR() : 
		mo(NULL),
		myKeys(NULL),
		curPage(1),
		loading(false)
	{
		
		for(int i=0; i < 128; i++)
		{
			for(int j=0; j < 5; j++)
			{
				pages[j][i] = 0.0;
				param[j][i] = NULL;
			}
			button[i] = 0.0;
		}

		msg.push_back(0x00);
		msg.push_back(0x00);
		msg.push_back(0x00);
		
	}
	
	#define SETCTRL( pg, kn, obj, prm ) \
		set[pg][kn] = obj; param[pg][kn] = obj-> FindParam( PN(prm) )
	
	void setControls()
	{
		SETCTRL( 4, 21, myKeys, "Shlf"  );
		SETCTRL( 4, 22, myKeys->piano, "Mix"  );
		SETCTRL( 4, 24, myKeys->reese, "Detu" );
		SETCTRL( 4, 27, myKeys->brass, "Mix"  );

		for( int i=0 ; i < NUM_OSCS ; i++ )
		{
			SETCTRL(0, 81+i, myKeys->oscs[i], "HiAs" );
			SETCTRL(0, 91+i, myKeys->oscs[i], "LoAs" );
			
			SETCTRL(1, 11+i, myKeys->oscs[i]->filtLP1, "Freq" );
			SETCTRL(1, 21+i, myKeys->oscs[i]->filtHP1, "Freq" );
			SETCTRL(1, 31+i, myKeys->oscs[i], "AmpV" );
			
			SETCTRL(2, 11+i, myKeys->oscs[i]->filtLP1, "Reso" );
			SETCTRL(2, 21+i, myKeys->oscs[i]->filtHP1, "Reso" );
			SETCTRL(2, 31+i, myKeys->oscs[i], "EnLP" );
			
			SETCTRL(3, 11+i, myKeys->oscs[i], "LFLP" );
			SETCTRL(3, 21+i, myKeys->oscs[i], "LFOF" );
			SETCTRL(3, 31+i, myKeys->oscs[i], "LFOA" );
			
			SETCTRL(4, 11+i, myKeys->oscs[i]->delay, "Fdbk" );
			//TCTRL(4, 21+i, myKeys->oscs[i], "Mix" );
			SETCTRL(4, 31+i, myKeys->oscs[i], "Dist" );
			
			SETCTRL(1, 41+i, myKeys->oscs[i], "AmpA" );
			SETCTRL(1, 51+i, myKeys->oscs[i], "AmpD" );
			SETCTRL(1, 61+i, myKeys->oscs[i], "AmpS" );
			SETCTRL(1, 71+i, myKeys->oscs[i], "AmpR" );
			
			SETCTRL(2, 41+i, myKeys->oscs[i], "FltA" );
			SETCTRL(2, 51+i, myKeys->oscs[i], "FltD" );
			SETCTRL(2, 61+i, myKeys->oscs[i], "FltS" );
			SETCTRL(2, 71+i, myKeys->oscs[i], "FltR" );
			
			#define PARAMFUNC( obj, prm, eff, eff1 ) \
				obj->FindParam( PN(prm) )->func = eff; \
				obj->FindParam( PN(prm) )->func1 = eff1
				
			PARAMFUNC( myKeys->oscs[i], "AmpV", Delay::square, Delay::root );
			PARAMFUNC( myKeys->oscs[i], "Dist", distParam, distParam1 );
			PARAMFUNC( myKeys->oscs[i]->delay, "Fdbk", Delay::square, Delay::root);
			PARAMFUNC( myKeys->oscs[i]->filtLP1, "Freq", Filter::lpFreq, Filter::lpFreq1 );
			PARAMFUNC( myKeys->oscs[i]->filtHP1, "Freq", Filter::hpFreq, Filter::hpFreq1 );
			PARAMFUNC( myKeys->oscs[i]->filtLP1, "Reso", Filter::lpRes , Filter::lpRes1 );
			PARAMFUNC( myKeys->oscs[i]->filtHP1, "Reso", Filter::hpRes , Filter::hpRes1 );
		}
	}
	
	static fl distParam (fl v){ return  1.0f - v * 0.9f; }
	static fl distParam1(fl y){ return (1.0f - y )/0.9f;  }
	
	friend ostream &operator<<(ostream &stream, MyBCR &bcr)
	{
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
	friend istream &operator>>(istream &stream, MyBCR &bcr)
	{
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
		bcr.myKeys->setLoRange( bcr.myKeys->loSplit );
		bcr.myKeys->setHiRange( bcr.myKeys->hiSplit );
		return stream;
	}
	
	
	void send(unsigned char a, unsigned char b, unsigned char c)
	{
		msg[0] = a;
		msg[1] = b;
		msg[2] = c;
		if( mo != NULL ) mo->sendMessage(&msg);		
	}
	
	void sendValues()
	{
		if( mo )
		{
			for(int i=40; i < 128; ++i)
			{
				send(0xBF, i, 127 * knob[i]);
			}
			for(int i=0; i <= 98; ++i)
			{
				send(0x9F, i, button[i] ? 0x7F : 0x00);
			}
		}
	}
	
	void execValues()
	{
		loading = true;
		
		for(int p=0;  p <=4; p++)
		{
			for(int i=0; i < 128; i++)
			{
				turnPage(p, i, pages[p][i]);
			}
		}
 
		
		for(int i=0; i <= 98; ++i){
			press(i, button[i]);
		}
		
		loading = false;
	}

	void update()
	{
		//if(panel) panel->Refresh();
		//wxMouseEvent eve(wxEVT_MOTION);
		//::wxPostEvent(panel, eve);
	}
	
	void updatePage(int b)
	{
		curPage = b - 100;

		//myKeys->canvas->doMessage( pageNames[curPage] );
		
		//activate the proper light
		for(int i = 1; i <= 4; i++)
		{
			if(i == curPage)
			{
				send( 0x9F, b, 0x7F );
			}
			else
			{
				send( 0x9F, 100 + i, 0x00 );
			}
		}
		
		// set and send knob assignments for page
		Param *pm;
		float v;
		for(int i = 0; i < 128; ++i)
		{
			pm = param[curPage][i];
			if( pm != NULL )
			{
				v = pm->GetValue();
				if( pm->func != NULL )
					v = pm->func1(v);
				pages[curPage][i] = v;
			}
			else
			{
				v = pages[curPage][i];
			}
			knob[i] = v;
			send( 0xBF, i, 0x7F * v );
		}
		for(int i=81; i < 99; i++)
		{
			pm = param[0][i];
			if( pm != NULL )
			{
				v = pm->GetValue();
				button[i] = v;
				send( 0x9F, i, 0x7F * v );
			}
		}
	}
	
	void press(int b, bool v)
	{
		/*
		char *pageNames[] =
		{
			"MAIN",
			"RES",
			"LFO",
			"FX"
		};
		*/
		
		int inst = ( b - 1 ) % 10;
		switch(b)
		{
			// preset selection
			case 51: case 52: case 53: case 54: case 55: case 56: case 57: case 58:
				if( v > 0.5)
				{
					wxCommandEvent menue( wxEVT_COMMAND_MENU_SELECTED, 1 + LOADID + inst );
					::wxPostEvent( wxTheApp, menue );
				}
				break;
				
			// tuning activation
			case 61: case 62: case 63: case 64: case 65: case 66: case 67: case 68:
				if(v > 0.5)
				{
					wxCommandEvent menue( wxEVT_COMMAND_MENU_SELECTED, TUNEID + b - 61 );
					::wxPostEvent( wxTheApp, menue );
				}
				break;
			
			case 42:
				if(v == 1.0) myKeys->piano->loadSamples();
				break;
				
			case 47:
				if(v == 1.0) myKeys->brass->loadSamples();
				break;
			
			// manual assignments
			case 81: // set hiSplit
				myKeys->hiSet = v;
				//myKeys->SetParam( PN("HiSp"), v );
				break;
			
			case 91: // set loSplit
				myKeys->loSet = v;
				//myKeys->SetParam( PN("LoSp"), v );
				break;
			
			case 82: case 83: case 84: case 85: case 86: case 87: case 88:
				inst = (b - 1) % 10;
				myKeys->hiAssign[inst] = v;
				myKeys->oscs[inst]->hiAssign = v;
				myKeys->canvas->active[inst] = 
					myKeys->loAssign[inst] | 
					myKeys->hiAssign[inst] ;
				myKeys->canvas->selinst = inst;
				break;
				
			case 92: case 93: case 94: case 95: case 96: case 97: case 98:
				inst = (b - 1) % 10;
				myKeys->loAssign[inst] = v;
				myKeys->oscs[inst]->loAssign = v;
				myKeys->canvas->active[inst] = 
					myKeys->loAssign[inst] | myKeys->hiAssign[inst] ;
				myKeys->canvas->selinst = inst;
				break;
				
			// mode selection
			case 104: // effects mode
			
				// tap tempo
				if( curPage == 4 and time < 2.0 )
				{
					myKeys->setDelay(time);
				}
				updatePage(b);
				break;
				
			case 101: // amplitude mode
				updatePage(b);
				//updateFilter(0);
				break;
				
			case 102: // frequency mode
			case 103: // LFO mode
				updatePage(b);
				break;

			// pedals
			case 111:
				myKeys->subSus = v;//(v > 0.5 );
				break;
			case 112:
				myKeys->subOn = v;//( v > 0.5 );
				myKeys->canvas->active[0] = myKeys->subOn;
				break;
		}
		//std::cout << b << "\n";
		//osc->noteOn(b, 100);
		update();
	}
	
	
	void turnPage( int p, int k, float v)
	{
	
		char *instNames[] =
		{
			"BASS ",
			"PIANO",
			"ORGAN",
			"REESE",
			"ANGLE",
			"PULSE",
			"BRASS",
			"NOISE"
		};
	
		const char *names[][7] = {
		{
			"","","",
			"","","",""
		},{
			"LPF",
			"HPF",
			"VOL",
			"A-A","A-D","A-S","A-R"
		},{
			"LPQ",
			"HPQ",
			"ENV-LPF",
			"F-A","F-D","F-S","F-R"
		},{
			"LFO-LPF",
			"LFO RATE",
			"LFO-AMP",
			"","","",""
		},{
			"DELAY",
			"MOD",
			"DIST",
			"","","",""
		}};
		
		//float f;
		inst = (k - 1) % 10;
		myKeys->inst = inst;
		pages[p][k] = v;
		
		int ks = (k / 10) - 1;

		Param *pm = param[p][k];
		if( pm != NULL )
		{
			if( pm->func != NULL )
				v = pm->func(v);
			set[p][k]->SetParam( pm->name.v, v );
		}
		
		if( ( p == 1 or p == 2 ) and k > 10 and k < 29 and inst < 8 )
			myKeys->updateFilter( inst );
		
		
		if( not loading and ks >= 0 and ks < 7 and inst >=0 and inst < 8)
		{
			wxString str(instNames[inst], wxConvUTF8);
			wxString str1(names[p][ks], wxConvUTF8);
			str << _T(" ");
			str << str1;
			str << _T(": ");
			str << v;
		
			myKeys->canvas->doMessage( str.mb_str(wxConvUTF8) );
		}

	}
	
	void turn(int k, float v)
	{
		turnPage(curPage, k, v);
		update();
	}
	
};

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

