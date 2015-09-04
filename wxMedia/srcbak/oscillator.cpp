
#include "oscillator.h"

//inline int max(int a, int b){ return a > b ? a : b; }
//inline int min(int a, int b){ return a < b ? a : b; }
//inline float max(float a, float b){ return a < b ? a : b; }
//inline float min(float a, float b){ return a > b ? a : b; }

void ADSR::setADSR (float a, float d, float s, float r){
	if( a >= 0.0f )  A = 15.f * powf(a, 6.0f) + 0.01f;
	if( d >= 0.0f )  D =  5.f * powf(d, 2.0f) + 0.01f;
	if( s >= 0.0f )  S = s;
	if( r >= 0.0f )  R = 15.f * powf(r, 6.0f) + 0.01f;
	calcADSR();
}

void ADSR::calcADSR(){
	static float const k = ((float)SAMPLE_RATE) / (float)ADSR_DIVISOR;
	/*
	tA = A * k; //* SAMPLE_RATE / ADSR_DIVISOR;
	tD = D * k; //* SAMPLE_RATE / ADSR_DIVISOR;
	tR = R * k; //* SAMPLE_RATE / ADSR_DIVISOR;

	dA = 1.f / tA;
	dD = 1.f / tD;
	dR = 1.f / tR;
	*/
	dA = 1.f / (A * k);
	dD = 1.f / (D * k);
	dR = 1.f / (R * k);
}


fl ADSR::doADSR(){
	
	asm("# S");
	if( clk | ADSR_MASK != 0 and stage == 'S' ) return val;

	//modvec = true;
	switch(stage)
	{
	case 'A': asm("# A");
		val += dA;
		if( val >= 1.0f )
		{
			stage = 'D';
			//note.time  = 0;
			val   = 1.0f;
		}
		break;
	case 'D': asm("# D");
		val -= dD;
		if( val <= 0.0f ) val = 0.0f;

		if( val < S ) //or notes[n].time >= tD )
		{
			stage = 'S';
			//note.time  = 0;
			val   = S;
		}
		break;
	case 'R': asm("# R");
		val -= dR;
		if( val <= 0.0f )
		{
			val   = 0.0f;
			stage = '_';
		} 
		break;
	case '_':
		//bend(note.num);
		//note.time   = 0;
		break;
	}
	//if(notes[n].amp > notes[n].vel) notes[n].amp = notes[n].vel;
	//modvec = false;
	//std::cout << notes[n].amp << "\n";
	return val;
}

///////////////////////////////////////////////////////////////////////////

Oscillator::Oscillator() : 
	modvec(true), sus(0), dist(0.0),
	amp(0.25), poly(0), 
	pitchbend(1.0), bendInt(12.0), highNote(0), lowNote(127), clk(0),
	lfo2lp(0), lfo2hp(0), lfo2amp(0.0)
{

	//calcADSR();
	
	temper = new EquaTemp();
	//wt->setupTable();
	//notes = wt->notes;

	notes = new noteData[maxPoly];
	for(int n=0; n < maxPoly; n++) // all notes off
	{
		noteData note;
		note.num    = 0;
		note.freq   = 440.0;
		note.phase  = 0;
		note.phase2 = 0;
		note.vel    = 0.0;
		note.down   = false;
		note.amp    = 0.0;
		note.ampEnv.stage  = '_';
		//note.time = 0; 
		notes[n]    = note;
	}


	
	active = false;
	
	//filtLP = new FilterIIR();
	//filtHP = new butterworth(200.);
	filtLP = new BiQuad(LPF, 2000.);
	filtHP = new BiQuad(HPF, 200.);
	modvec = false;
	
	delay = new Delay();
	lfo = new SineTbl();


	lfoNote.num = 1;
	lfoNote.freq = calcFreq(7.0f);
	lfoNote.phase *= calcFreq(7.0f);
	//lfoNote.phase >>= 0;
	lfoNote.vel = 0.5;
	lfoNote.amp = 1.0; // use this parameter to control assignment
}

void Oscillator::setLFO2LP(fl v){ lfo2lp = v; }
void Oscillator::setLFO2HP(fl v){ lfo2hp = v; }
void Oscillator::setLFO2Amp(fl v){lfo2amp= v; }

void Oscillator::setLFO( float a, float f ){
	//if( a != -1 ) lfoNote.amp = a;
	if( f != -1 ) {
		lfoNote.freq = calcFreq(f);
		lfoNote.phase *= calcFreq(f);
	}
}

void Oscillator::bend(float b){
	pitchbend = pow(2, bendInt * b / 12. );
	retemper();
}

void Oscillator::retemper() {
	for(int n=0; n < 128; n++){
		//if(notes[n].num != 0){
			notes[n].freq = calcFreq(n);
			notes[n].phase *= notes[n].freq;
		//}
	}
}

void Oscillator::retune(float A){
	temper->retune(A);
	retemper();
}

void Oscillator::modulate(int m){
	temper->modulate(m);
	retemper();
}



int Oscillator::calcFreq(float f)
{
	static const double dsr = resolution / (double)SAMPLE_RATE;
	return f * pitchbend * dsr;
}
int Oscillator::calcFreq(int n)
{
	return calcFreq((*temper)(n)); //* pitchbend * resolution / (double)SAMPLE_RATE;
}



void Oscillator::setDist(float f)
{
	dist = f;
}

void Oscillator::setDelay(float t, float f){
	delay->set(t, f);
}


void Oscillator::setLowpass(fl fc, fl q){
	filtLP->set(fc,q);
	if( fc != -1) fLP = fc;
	if( q  != -1) qLP = q;
}

void Oscillator::setHighpass(fl fc, fl q){
	filtHP->set(fc,q);
	if( fc != -1) fHP = fc;
	if( q  != -1) qHP = q;
}


void Oscillator::sustain(int v){
	sus = v;

	if( v < 64 ){	// initiate release phase for all notes not currently pressed (do this)
		for( int n = 0; n < 128; ++n ){
			if(notes[n].ampEnv.stage != '_' and not notes[n].down){
				//notes[n].stage = 'R';
				//notes[n].time = 0;
				noteOff(n);
			}
		}
	}
}

void Oscillator::noteOn(int n, int v){ // needs mode for retriggering in sustain mode
	//if(modvec) return;
	modvec = true;

	//notes[n].freq  = calcFreq(n); // this should already be precomputed.
	//notes[n].phase = 0; // let the piano class do this itself.	
	//notes[n].time  = 0;
	notes[n].num   = n;
	notes[n].ampEnv.stage ='A';
	notes[n].down  = true;
	if( v >= 0 ){
		notes[n].vel   = v / 127.f;
	}

	if(poly < maxPoly - 1) ++poly;

	if(highNote < n) highNote = n;
	if(lowNote  > n) lowNote  = n;

	modvec = false;
	active = true;
}
void Oscillator::noteOff(int n){
	//if(modvec) return;
	modvec = true;
	noteData &note = notes[n];
	note.down = false;
	if( sus < 64 )
	{
		if(note.ampEnv.stage != 'R'){
			note.ampEnv.stage = 'R';
			//note.time = 0;
		}
		//notes[n].phase = 0;
	}
	/* else {
		//notes[n].stage = 'S';
		//notes[n].time = 0;
	}
	*/
	modvec = false;
	if(highNote == n)
		while( highNote >  0  and notes[highNote].num == 0 )
			highNote--;
	if(lowNote  == n)
		while( lowNote  < 127 and notes[lowNote ].num == 0 )
			lowNote++;
	if(lowNote == 0 and highNote == 127) active = false;
}

void Oscillator::setADSR(fl a, fl d, fl s, fl r){
	for(int i=0; i < 128; ++i){
		notes[i].ampEnv.setADSR(a,d,s,r);
		notes[i].fltEnv.setADSR(a,d,s,r);
	}
}


void Oscillator::doADSR(int n){
	noteData &note = notes[n];
	note.amp = note.ampEnv.doADSR();
	if(note.ampEnv.stage == '_'){
		note.num    = 0;
		note.phase  = 0;
		note.phase2 = 0;
	}
}


int Oscillator::generate GENERATE(inBuf, outBuf, framesPerBuf, timeInfo, status) {
	assert(outBuf != NULL);
	float **out = static_cast<float **>(outBuf);

	fl o, sL;//, sR;

	//int frameClk;
	//if(active){

	for(int i=0; i < framesPerBuf; i++){
		
		sL = 0.0; //sR = 0.0;

		for(int n = lowNote; n <= highNote; n++){
			noteData &note = notes[n];
			if(note.num != 0)
				doADSR(n);
			if(note.num != 0)
				sL += (*wt)(note);
		}
		++clk;
		
		o = applyEffects(sL);
		
		sL = o * amp;
		//sR = o;
		
		out[0][i] = sL;
		out[1][i] = sL;
	}
	return paContinue;
}

float Oscillator::applyEffects(float o){
	fl ao, s, l;
	
	// LFO section
	l = (*lfo)(lfoNote);
	o *= 1.0 - (lfo2amp * (0.5f + l));
	
	filtLP->mod(lfo2lp * (0.5f + l));
	//filtHP->mod(lfo2hp * l);
	
	// distortion section
	ao = fabs(o);
	if( o < 0.0f ) s = -1.0f; else s = 1.0f;
	o = s * powf(ao, dist) * fmaxf(dist, 0.3f); //* fmin(0.8, 1.0 - dist); //* max(0.75f, fabs(dist - 0.5));

	// delay section
	o = delay->process(o);

	///* // filter section
	if( fLP < 20000.f )
		o = filtLP->filter(o);
	//if( fHP > 50 )
		o = filtHP->filter(o);

	return o;
}

///////////////////////////////////////////////////////////////////

WaveTable::WaveTable() : tableSize(SIZE) 
{
	setupTable();
}

float WaveTable::operator() (noteData &note)
{
	
	//note.time++;
	//p = note.phase;// >> SHIFT;
	//f = note.freq;
	return table[note.phase++] * note.amp * note.vel;
	//note.phase += f;
	//++note.phase;
}

///////////////////////////////////////////////////////

SineTbl::SineTbl(){
	double f = 2. * M_PI / (double)tableSize;
	for( int t = 0; t < tableSize; t++ ){
		table[t] = (float) sin( t * f );
	}
}
/*
SineOsc::SineOsc()
{
	wt = new SineTbl();
}*/

TriangleTbl::TriangleTbl(){
	int t = 0;
	for(; t < tableSize / 4; ++t){
		table[t] = ((float)t) / ((float)tableSize/4.);
	}
	for(; t < 3 * tableSize / 4; ++t){
		table[t] = 2.0 - ((float)t) / ((float)tableSize/4.);
	}
	for(; t < tableSize; ++t){
		table[t] = -4.0 + ((float)t / ((float)tableSize/4.));
	}
}
SquareTbl::SquareTbl(){
	float cycle = 0.5;
	int t = 0;
	int duty = tableSize * cycle;

	table[t] = 0.0f;	
	for(int t=1; t < duty; t++){
		table[t] = 1.0f;
	}
	table[t] = 0.0f;
	for(int t = duty+1; t < tableSize; t++){
		table[t] = -1.0;
	}
}
SawTbl::SawTbl(){
	double f =  2.0 / (double)tableSize;
	int t;
	for(t=0; t < tableSize / 2; t++){
		double tf = t * f;
		table[t] = tf;
	}
	for(; t < tableSize; t++){
		double tf = t * f;
		table[t] = (float)(tf - 2.0);
	}
}

NoiseTbl::NoiseTbl()
{
	int t;
	for(t=0; t < tableSize; t++){
		table[t] = (double)(rand() / (double)RAND_MAX ) - 0.5;
	}
}

/*
void Triangle_analog(){
	double f = M_PI / (double)tableSize;
	for( int t = 0; t < tableSize; t++ ){
		table[t] = 0;
		for(int k = 1; k < 999; k++){
			table[t] += (float) sin( k * M_PI / 2. ) * 
				( (float) sin( 2 * M_PI * k * f * (double)(t) )
					/ (double)( k*k ) );
		}
		table[t] *= 8. / M_PI;
	}
}
*/
/*
void SquareOsc_analog(){
	float f = M_PI  / (double)SAMPLE_RATE;
	for( int t = 0; t < SAMPLE_RATE; t++ ){
		table[t] = 0;
		for(int k = 1; k < 999; k++){
			table[t] += (float) sin( (2 * k - 1) * 2 * f * (double)(t) )
					/ (double)(2.*k - 1);
		}
		table[t] *= 4. / M_PI;
	}
}
*/
/*
void SawOsc_differential(){
	float dx = 0.5 / SAMPLE_RATE;
	float x = 0.;
	for( int t = 0; t < SAMPLE_RATE; t++ ){
		table[t] = x;
		x += dx;
		if( x > 1. ) x = -1.0;
	}
}
*/
/*
void SawOsc::analog(){
	float f = 2. * M_PI / (double)SAMPLE_RATE;
	for( int t = 0; t < SAMPLE_RATE; t++ ){
		table[t] = 0;
		for(int k = 1; k < 999; k++){
			table[t] += (float) sin( f * (double)(k * t) ) / (double)k;
		}
		table[t] *= -2. / M_PI;
	}
}
*/


#include <wx/app.h>
#ifdef __WXMAC__
# include "wx/filename.h"
#endif

wxString Oscillator::GetExecutablePath()
{
    static bool found = false;
    static wxString path;

    if (found)
        return path;
    else
    {
        wxString argv0 = wxTheApp->argv[0];

        if (wxIsAbsolutePath(argv0))
            path = argv0;
        else
        {
            wxPathList pathlist;
            pathlist.AddEnvList(wxT("PATH"));
            path = pathlist.FindAbsoluteValidPath(argv0);
        }

        wxFileName filename(path);
        filename.Normalize();
        path = filename.GetFullPath();

        found = true;
        return path;
    }
}

wxString Oscillator::GetSamplePath() // this is a hack
{
	static wxString path = GetExecutablePath();
#ifdef __DARWIN__
	path = path.Left(4 + path.Find(_T(".app")));
	path << _T("/Contents/Resources/Samples/");
#endif
	return path;
}
