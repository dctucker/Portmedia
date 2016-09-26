/*	
**	D. Casey Tucker
**	dnbMedia wxWidgets application for audio testing
**	all code within belongs to the author
*/

#include "wxmain.h"

MyApp *app;

BEGIN_EVENT_TABLE(MyApp, wxApp)
EVT_MENU( wxID_ABOUT , MyApp::OnAbout )
EVT_MENU( wxID_OPEN  , MyApp::OnOpen  )
EVT_MENU( wxID_CLOSE , MyApp::OnClose )
EVT_MENU( wxID_EXIT  , MyApp::OnQuit  )
EVT_MENU( wxID_PRINT , MyApp::OnStore )
EVT_MENU( wxID_DELETE, MyApp::OnDelete )
EVT_MENU( wxID_BOLD  , MyApp::OnLoadSamples )
EVT_COMMAND_RANGE(LOADID, LOADID+99, wxEVT_COMMAND_MENU_SELECTED , MyApp::OnLoad )
EVT_COMMAND_RANGE(ADEVID, ADEVID+99, wxEVT_COMMAND_MENU_SELECTED , MyApp::OnMnuAudio )
EVT_COMMAND_RANGE(MDEVID, MDEVID+99, wxEVT_COMMAND_MENU_SELECTED , MyApp::OnMnuMIDI )
EVT_COMMAND_RANGE(TUNEID, TUNEID+99, wxEVT_COMMAND_MENU_SELECTED , MyApp::OnMnuTune )
EVT_COMMAND_RANGE(TUNEID, SERIID+99, wxEVT_COMMAND_MENU_SELECTED , MyApp::OnMnuSeri )
END_EVENT_TABLE()

#define DEL(X) if(X != NULL) delete X
IMPLEMENT_APP(MyApp)

void MyApp::OnAbout(wxCommandEvent &event)
{
	wxMessageBox(_T("D. Casey Tucker © 2009-2016"), _T("dnbMedia"));
}

float loaded_samples[960000];
 
void MyApp::OnOpen(wxCommandEvent &event)
{
	wxFileDialog fd( NULL, _T("Open") , _T("") , _T(""),
					_T("*.wav") , wxFD_OPEN );
	int r = fd.ShowModal();

	if(r == wxID_OK)
	{
		if( waveFrame == NULL )
		{
			// setup frame for the waveform editor
			waveFrame = new wxFrame( NULL, -1, _T("Waveform editor"),
								wxPoint(100,50), wxSize(400,200),
								( wxDEFAULT_FRAME_STYLE )
								& compl wxMAXIMIZE_BOX ); // & wxRESIZE_BORDER );
			
			//wavePanel = new WavePanel(waveFrame);
			//wavePanel->SetMinSize(wxSize(30,30));
			//waveFrame->SetMinSize(wxSize(30,30));
			//wavePanel->SetSamples(loaded_samples);
			//waveFrame->Show();
		}
		wxString filename = fd.GetPath();
		SndfileHandle *sf = new SndfileHandle(filename.mb_str(), SFM_READ);
		sf->read(loaded_samples, 960000 );
		delete sf;
		return;
	}
}


// KILL SECTION

bool killFrame(wxFrame *&f)
{

	if(f != NULL and f->IsActive())
	{
		f->Close();
		delete f;
		f = NULL;
		return true;
	}
	return false;
}

void MyApp::OnClose(wxCommandEvent &event)
{
	//timer->Stop();
	killFrame(frame) or killFrame(envFrame) or killFrame(glFrame);
}

void MyApp::OnQuit(wxCommandEvent &event)
{
	//timer->Stop();
	::wxExit();
}

int MyApp::OnExit()
{
	//timer->Stop();
	//glCanvas->timer->Stop();
	delete glCanvas->context;

	if(mi[0] != NULL){
		killMIDI();
		delete mi[0];
	}

	if(stream !=NULL){
		killAudio();
		//delete stream;
	}

	DEL( bus );

	DEL(  bcr   );
	DEL(  osc   );
	DEL( square );
	DEL(  sine  );
	DEL( piano  );

	DEL( brass  );
	/*
	DEL( tri    );
	DEL( noise  );
	*/
	
	//delete piano;

	return 0;
}

void MyApp::CloseMIDI(int id)
{
	//if( mi[id] != NULL )
	try {
		mi[id]->closePort();
		mi[id]->cancelCallback();
	} catch( RtMidiError *err ) {
		//wxMessageBox( wxString( err->getMessageString(), wxConvUTF8 ) );
		err->printMessage();
	}
	mi.erase(id);
}

void MyApp::killMIDI()
{
    // @TODO fix
	/*
    for( std::map<int, RtMidiIn *>::iterator i = mi.begin(); i != mi.end(); i++)
    // for(unsigned i=0; i < 10; i++)
	{
		CloseMIDI(i->first);
	}
    */
	delete rtmi;
	DEL(rtmo);
}

void MyApp::killAudio(){
	stream->stop();
	stream->close();
	delete stream;

//	stream->stop();
//	stream->close();
	asys->terminate();
	//delete asys;
	stream = NULL;

}


// Presets

void MyApp::OnDelete(wxCommandEvent &event)
{
	wxConfigBase *config = wxConfigBase::Get();
	if( wxMessageBox(_T("Really delete current preset?"), _T("Delete"), wxYES_NO ) == wxYES )
	{
		config->DeleteGroup( _T("/Presets/") + current_preset );
		config->Flush();
	}
}

void MyApp::OnStore(wxCommandEvent &event)
{
	wxConfigBase *config = wxConfigBase::Get();
	
	ofstream myfile;
	wxTextEntryDialog diag( NULL, _T("Enter a preset name"), _T("Preset store") );
	int r = diag.ShowModal();
	if( r == wxID_OK )
	{
		current_preset = diag.GetValue();
		
		wxString fn = _T(PRESET_PATH);
		fn << diag.GetValue();
		myfile.open( fn.mb_str() );
		myfile << *bcr;
		myfile.close();
		numPresets++;
		menuPres->Append( LOADID + numPresets, diag.GetValue() );
		
		wxString ph = _T("/Presets/") + diag.GetValue();
		config->SetPath( ph );
		key->Save();
		config->SetPath( _T("/") );
		config->Flush();
	}
}

//#define FILEPRESETS

void MyApp::OnLoad(wxCommandEvent &event)
{
	wxString fn, menuname;
	
	menuname = menuPres->GetLabelText( event.GetId() );
	current_preset = menuname;
	
	glCanvas->doMessage( menuname.Upper().mb_str(wxConvUTF8) );
	 
#ifdef FILEPRESETS
	fn << _T(PRESET_PATH);
	fn << menuname;
	cout << fn.mb_str() << "\n";
	ifstream myfile( fn.mb_str() );
	string s;
	if( myfile and not myfile.eof() )
	{
		
		//getline(myfile, s);
		//cout << s << "\n";

		myfile >> *bcr;
		myfile.close();
		bcr->sendValues();
		bcr->execValues();
		
	}
#else
	wxConfigBase *config = wxConfigBase::Get();
	config->SetPath( _T("/Presets/") + menuname );
	key->Load();
	config->SetPath( _T("/") );
	for(int i=0; i < NUM_OSCS; i++)
		key->updateFilter(i);
#endif

	bcr->press(101, 1);
}

void MyApp::OnLoadSamples(wxCommandEvent &event)
{
	piano->loadSamples();
	brass->loadSamples();
}

// AUDIO SECTION

void MyApp::OpenAudio(int id)
{
	try
	{ 
		Device &dev = asys->deviceByIndex(id);
		
		// is this an input or an output device?

		if ( dev.maxOutputChannels() > 0 ) {
		
			if( stream == NULL ){
		
				DirectionSpecificStreamParameters
					outParams(dev, dev.maxOutputChannels(), FLOAT32, false,
						//dev.defaultLowOutputLatency(), 
						DESIRED_LATENCY,
						NULL );
				
				StreamParameters
					params(DirectionSpecificStreamParameters::null(), outParams,
						SAMPLE_RATE, FRAMES_PER_BUFFER, 0 );
				stream = new MemFunCallbackStream<Bus> (params, *bus, &Bus::generate);
				//stream = new MemFunCallbackStream<Oscillator> (params, *osc, &Oscillator::generate);
			
			} else {
				stream->stop();
				stream->close();
				delete stream;
				stream = NULL;
			}
		} 
		else
		if( dev.maxInputChannels() > 0 )
		{
			if( stream_in == NULL)
			{
			
				DirectionSpecificStreamParameters
					inParams(dev, dev.maxInputChannels(), FLOAT32, false,
						DESIRED_LATENCY,
						//dev.defaultLowInputLatency(), 
						NULL );
				
				StreamParameters
					params(inParams, DirectionSpecificStreamParameters::null(),
						SAMPLE_RATE, FRAMES_PER_BUFFER, compl paClipOff );
			
				rec->Rewind();
				stream_in = new MemFunCallbackStream<Recorder> (params, *rec, &Recorder::consume);

			} else {
				
				//wavePanel->SetSamples(rec->GetSamples());

				stream_in->stop();
				stream_in->close();
				delete stream_in;
				stream_in = NULL;
			}

		}
		
        if( stream ){
            stream->start();
            std::cout << "Output ready.\n";
        }
        if( stream_in ){
            stream_in->start();
            std::cout << "Input ready.\n";
        }


	} catch(PaException err){
		wxMessageBox( wxString(err.what(), wxConvUTF8));
	}
}

void MyApp::OnMnuAudio(wxCommandEvent &event)
{
	PaDeviceIndex adevid = event.GetId() - ADEVID;
	OpenAudio(adevid);
}


// MIDI SECTION

void OnRtMidi(double dt, MIDIMSG message, void *data)
{
	app->OnMIDI(dt, message, data);
}

void MyApp::OnMIDI(double dt, MIDIMSG message, void *data){
	//bcr->callback(dt, message, data);
	//std::cout << (int)data;
	MidiController *mc = (MidiController *)data;
	mc->callback(dt, message, data);
}

void MyApp::OpenMIDI(int mdevid)
{
	MidiController *receiver = bcr; // default
	
	
	if( mdevid >= rtmi->getPortCount() )
	{
		return;
	}

	mi[mdevid] = new RtMidiIn();
		
	wxString str(mi[mdevid]->getPortName(mdevid).c_str(), wxConvUTF8);
	
	if(str.Contains(_T("BCR2000 Port 1"))){
		rtmo = new RtMidiOut();
		rtmo->openPort(mdevid);
		bcr->mo = rtmo;
		receiver = bcr;
		std::cout << "Found BCR2000\n";
	}
	if(str.Contains(_T("BCR2000 Port 2"))){
		receiver = drums;
		std::cout << "Found BCR secondary input for drums\n";
	}
	if(str.Contains(_T("CP33"))){
		receiver = key;
		std::cout << "Found CP33\n";
	}
	if(str.Contains(_T("Keystation"))){
		receiver = key;
		std::cout << "Found Keystation\n";
	}
	if(str.Contains(_T("Little Phatty"))){
		receiver = key;
		std::cout << "Found Phatty\n";
	}
	if(str.Contains(_T("YAMAHA"))){
		receiver = key;
	}
	
	mi[mdevid]->ignoreTypes( true, true, true );
	mi[mdevid]->setCallback( OnRtMidi, receiver);
	try {
		mi[mdevid]->openPort( mdevid );
	} catch( RtMidiError *err ){
		std::cout << "Couldn't open "<< mdevid;
	}
	mdevHash[mdevid] = mi.size() - 1;
	//std::cout << std::string( event.GetMenu().mb_str()) << "\n";
	//std::cout << mi.back()->getPortName(mdevid) << "\n";
}

void MyApp::OnMnuMIDI(wxCommandEvent &event)
{
	int mdevid = event.GetId() - MDEVID;
	if( event.IsChecked() ){
		OpenMIDI(mdevid);
	} else {
		CloseMIDI(mdevid);
	}
}


void MyApp::OnMnuTune(wxCommandEvent &event)
{
	int tuning = event.GetId() - TUNEID;
	wxString str( menuTune->GetLabelText( event.GetId() ) );
	key->setTemper( tuning );
	
	glCanvas->doMessage( str.Upper().mb_str(wxConvUTF8) );

}


void MyApp::OnMnuSeri(wxCommandEvent &event)
{
	// int seri = event.GetId() - SERIID;
	wxString str( menuDevs->GetLabelText( event.GetId() ) );
	key->arduino->Init( str.Trim(false) );
}
// INIT SECTION

// initialize the MIDI menu, populated by RtMIDI
void MyApp::initMidi(wxMenu *menuDevs)
{
	TRYMIDI( rtmi = new RtMidiIn(); )
	wxMenuItem *title = new wxMenuItem(menuDevs, wxID_ANY, _T("MIDI:"), _T("MIDI"));
	title->Enable(false);
	menuDevs->Append(title);
	int nPorts = rtmi->getPortCount();
	for(unsigned i=0; i < nPorts; i++){
		wxString dname( rtmi->getPortName(i).c_str(), wxConvUTF8 );
		wxString str(_T("    "));
		str << i << _T(": ") << dname;
        std::cout << str << "\n";
		menuDevs->AppendCheckItem(MDEVID + i, str);
	}
	
	//mi->setCallback( &(MidiController::callback) ); // can't do this unless static
}

// Initialize the audio menu, populated by PortAudio
void MyApp::initAudio(wxMenu *menuDevs)
{ 
	System::initialize();
	asys = &( System::instance() );
	
	wxMenuItem *title = new wxMenuItem(menuDevs, wxID_ANY, _T("Audio:"), _T("Audio"));
	title->Enable(false);
	menuDevs->Append(title);
	
	//int nDevs = sys.deviceCount();
	for (System::DeviceIterator i = asys->devicesBegin(); i != asys->devicesEnd(); ++i)
	{
		wxString devname(_T("    "));
		bool defaultDisplayed = false;

		if (i->isSystemDefaultInputDevice()){
			defaultDisplayed = true;
		} else if ((*i).isHostApiDefaultInputDevice()){
			defaultDisplayed = true;
		}

		if (i->isSystemDefaultOutputDevice()) {
			defaultDisplayed = true;
		}
		else if (i->isHostApiDefaultOutputDevice()){
			defaultDisplayed = true;
		}

		wxString n1(i->name(), wxConvUTF8 );
		wxString n2(i->hostApi().name(), wxConvUTF8);
		wxString n3;
		if (defaultDisplayed) n3 << _T("*"); else n3 << _T(" ");
		
		devname << i->index() << _T(":") << n3 << _T("[") << n2 << _T("] ") << n1;
		
		/* wxMenuItem *adev = */
		menuDevs->AppendCheckItem(ADEVID + i->index() , devname);
		//if(defaultDisplayed) adev->Check();
    }
}

void MyApp::initSeri(wxMenu *menuDevs)
{
	int i = 0;
	wxMenuItem *title = new wxMenuItem(menuDevs, wxID_ANY, _T("Serial:"), _T("Serial"));
	title->Enable(false);
	menuDevs->Append(title);

	wxDir dir("/dev");
	wxString fn;
	bool cont = dir.GetFirst(&fn, _T("tty.*"));
	while( cont )
	{
		menuDevs->AppendCheckItem(SERIID + i, _T("    /dev/") + fn);
		i++;
		cont = dir.GetNext(&fn); 
	}

}

// initialize the tunings menu
void MyApp::initTemps(wxMenu *mnu)
{
	mnu->AppendRadioItem(TUNEID+0, _T("Equal temperament\tCtrl+1"));
	mnu->AppendRadioItem(TUNEID+1, _T("Just 5-limit intonation\tCtrl+2"));
	mnu->AppendRadioItem(TUNEID+2, _T("Kepler Tuning\tCtrl+3"));
	mnu->AppendRadioItem(TUNEID+3, _T("Meantone temperament\tCtrl+4"));
	mnu->AppendRadioItem(TUNEID+4, _T("Pythagorean tuning\tCtrl+5"));	
	mnu->AppendRadioItem(TUNEID+5, _T("Hammond tonewheels\tCtrl+6"));	
	mnu->AppendRadioItem(TUNEID+6, _T("Ptolemeic tuning\tCtrl+6"));	
	mnu->AppendRadioItem(TUNEID+7, _T("Chinese gamut\tCtrl+6"));	
}

void MyApp::initPres(wxMenu *mnu)
{
	mnu->Append(wxID_PRINT, _T("&Store\tCtrl+P")  );
	mnu->Append(wxID_DELETE, _T("&Delete\tCtrl+D")  );
	mnu->AppendSeparator();
	// put menu items in from file list
	numPresets = 0;
	
#ifdef FILEPRESETS
	wxDir dir( _T(PRESET_PATH));
	wxString fn;
	bool c = dir.GetFirst(&fn, wxEmptyString, wxDIR_DEFAULT & compl wxDIR_DIRS);
	while( c )
	{
		++numPresets;
		mnu->Append(LOADID + numPresets, fn);
		c = dir.GetNext(&fn);
	}
#else
	int i=0;
	long dummy;
	wxString str;
	wxConfigBase *config = wxConfigBase::Get();
	config->SetPath(_T("/Presets"));
	numPresets = config->GetFirstGroup(str,dummy);
	while( numPresets )
	{
		++i;
		mnu->Append( LOADID+i, str );
		numPresets = config->GetNextGroup(str, dummy);
	}
	numPresets = config->GetNumberOfGroups();
#endif
}

void MyApp::initMenubar()
{
	wxMenuBar *menuBar = new wxMenuBar(wxMB_DOCKABLE);

	menuFile = new wxMenu;
	menuFile->Append(wxID_NEW          , _T("&New\tCtrl+N"));
	menuFile->Append(wxID_OPEN         , _T("&Open\tCtrl+O"));
	menuFile->Append(wxID_BOLD         , _T("&Load samples\tCtrl+L"));
	menuFile->Append(wxID_CLOSE        , _T("&Close\tCtrl+W"));
	menuFile->Append(wxID_PREFERENCES  , _T("Preferences...\tCtrl+."));
	menuFile->Append(wxID_EXIT         , _T("E&xit\tCtrl+Q"));

	menuDevs = new wxMenu;
	initAudio(menuDevs);
	menuDevs->AppendSeparator();
	initMidi(menuDevs);
	menuDevs->AppendSeparator();
	initSeri(menuDevs);
	
	menuTune = new wxMenu;
	initTemps(menuTune);
	
	menuPres = new wxMenu;
	initPres(menuPres);


	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT, _T("&About"));

	menuBar->Append(menuFile, _T("&File"));
	menuBar->Append(menuDevs, _T("&Devices"));
	menuBar->Append(menuTune, _T("&Tuning"));
	menuBar->Append(menuPres, _T("&Preset"));
	menuBar->Append(menuHelp, _T("&Help"));

	//wxApp::SetExitOnFrameDelete(false);
	
#ifndef WIN32
	wxMenuBar::MacSetCommonMenuBar(menuBar);
#else
	frame->SetMenuBar(menuBar);
#endif
}

#include <xmmintrin.h>

//#include "intervals.cpp"

bool MyApp::OnInit()
{
	SetAppName(_T("dnbMedia"));
	SetVendorName(_T("D. Casey Tucker"));
	//_mm_setcsr( _mm_getcsr() | 0x8040 ); // set DAZ and FZ bits
	
	/*
	for(int i=0; i < 100; i++){
		cout << (double)IntervalTable[i].ratio <<"\n";
	}
	*/

	wxConfigBase::Set( new wxFileConfig(_T("dnbMedia")) );
	//wxConfigBase::Get()->

	INIT_PD;
	pd.Pulse(_T("Initializing"));
	
	app = this;
	bcr = NULL; frame = NULL; stream = NULL; stream_in = NULL;
	drums = NULL;
	waveFrame = NULL;
	rtmo = NULL;
	Temperament::nullify();

	
	pd.Pulse(_T("Loading instruments"));
	
	sine   = new SubSineOsc();
	osc    = new Hammond();
	square = new SquareOsc();
	tri    = new TriangleOsc();	
	reese  = new Reese();
	brass  = new Brass();
	noise  = new NoiseOsc();
	
	rec = new Recorder;

	//piano = NULL;
	piano = new Upright;
	
	
	
	bus = new Bus;
	bus->add(sine);
	bus->add(piano);
	bus->add(osc);
	bus->add(reese);
	bus->add(tri);
	bus->add(square);
	bus->add(brass);
	bus->add(noise);
	
	pd.Pulse(_T("Enumerating Audio / MIDI devices"));
	

	pd.Pulse(_T("Initializing GUI"));


    
	frame = new wxFrame( NULL, wxID_ANY, wxT("Musical canvas"),
		wxPoint(250, 100), wxSize(933,700), 
		( wxDEFAULT_FRAME_STYLE /* | wxFRAME_TOOL_WINDOW */ )
	);

	initMenubar();

	
	//bcrPanel = new BCRPanel(frame, bcr);
	
	//bcr->panel = bcrPanel;
	//bcr->osc = osc;

	/*
	//setup frame for Keystation 88es
	keyFrame = new wxFrame( NULL, -1, _T("Keystation 88es"),
						wxPoint(50,700), wxSize(800, 100),
						( wxDEFAULT_FRAME_STYLE ) // | wxFRAME_TOOL_WINDOW )
						& compl wxMAXIMIZE_BOX & compl wxRESIZE_BORDER );

	*/

	key = new MyKeys();
	/*
	keyPanel = new KeyboardPanel(keyFrame, key);
	keyPanel->SetMinSize(wxSize(400, 40));
	keyFrame->SetMinSize(wxSize(400, 40));
	keyFrame->SetMaxSize(wxSize(2000, 200));
	key->panel = keyPanel;
	*/
	key->sub   = sine;
	key->organ = osc;
	key->piano = piano;
	key->square= square;
	key->reese = reese;
	key->tri   = tri;
	key->brass = brass;
	key->noise = noise;
	key->bus   = bus;
	key->setOscs();

	bcr = new MyBCR();	
	bcr->myKeys = key;
	bcr->setControls();
	
	
	//bcr->setLev(&(bus->level));

	drums = new MyDrumIn();


/*
	joyFrame = new wxFrame( NULL, -1, _T("Gamepad"),
		wxPoint(50, 200), wxSize( 100,100),
		wxDEFAULT_FRAME_STYLE & compl wxMAXIMIZE_BOX );
	
	joy = new MyGamepad(joyFrame);
	joy->myKeys = key;
	joyFrame->Show();
*/
	
	glCanvas = new Canvas3D( frame );
	glCanvas->SetScopeBuffers(bus->maxv, bus->minv);
	glCanvas->count = &(bus->count);

	//frame->SetClientSize(400,400);

	for(int i=0; i < 8; i++)
	{
		glCanvas->ampA[i] = &( key->getOsc(i)->notes[0].ampEnv.A );
		glCanvas->ampD[i] = &( key->getOsc(i)->notes[0].ampEnv.D );
		glCanvas->ampS[i] = &( key->getOsc(i)->notes[0].ampEnv.S );
		glCanvas->ampR[i] = &( key->getOsc(i)->notes[0].ampEnv.R );
		glCanvas->ampV[i] = &( key->getOsc(i)->amp );
	}

	
	key->canvas = glCanvas;
	drums->canvas = glCanvas;
	drums->myKeys = key;
	glCanvas->bcr = bcr;

	//done! show the frames
	frame->Show(true);
	//keyFrame->Show();
	
	//compkeyFrame->Show();
	//pd.Update(100, _T("Done."));
	
	// open my devices:
	OpenAudio(0);
	OpenMIDI(0);
	OpenMIDI(2);
	
	
	key->setTemper(0);

	key->arduino = new MyArduino();

	bcr->sendValues();
	bcr->execValues();

	glCanvas->timer->Start(20);
    return true;
}
