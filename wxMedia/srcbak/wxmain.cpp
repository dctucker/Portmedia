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
EVT_COMMAND_RANGE(LOADID, LOADID+99, wxEVT_COMMAND_MENU_SELECTED , MyApp::OnLoad )
EVT_COMMAND_RANGE(ADEVID, ADEVID+99, wxEVT_COMMAND_MENU_SELECTED , MyApp::OnMnuAudio )
EVT_COMMAND_RANGE(MDEVID, MDEVID+99, wxEVT_COMMAND_MENU_SELECTED , MyApp::OnMnuMIDI )
EVT_COMMAND_RANGE(TUNEID, TUNEID+99, wxEVT_COMMAND_MENU_SELECTED , MyApp::OnMnuTune )
END_EVENT_TABLE()

#define DEL(X) if(X != NULL) delete X
IMPLEMENT_APP(MyApp)

void MyApp::OnAbout(wxCommandEvent &event){
	wxMessageBox(_T("D. Casey Tucker © 2009"), _T("dnbMedia"));
}

void MyApp::OnOpen(wxCommandEvent &event){
	wxFileDialog fd( NULL, _T("Open") , _T("") , _T(""),
					_T("*.wav") , wxFD_OPEN );
	int r = fd.ShowModal();
	if(r == wxID_OK){
		wxString filename = fd.GetPath();
		SndfileHandle *sf = new SndfileHandle(filename.fn_str(), SFM_READ);
		sf->read(wavePanel->GetSamples()[0], wavePanel->num() );
		delete sf;
		return;
	}
}


// KILL SECTION

bool killFrame(wxFrame *&f){
	if(f != NULL and f->IsActive()){
		f->Close();
		delete f;
		f = NULL;
		return true;
	}
	return false;
}

void MyApp::OnClose(wxCommandEvent &event){
	killFrame(frame) or killFrame(keyFrame) or killFrame(envFrame);
}

void MyApp::OnQuit(wxCommandEvent &event){
	::wxExit();
}

int MyApp::OnExit(){

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

	
	//delete piano;

	return 0;
}

void MyApp::CloseMIDI(int id){
	mi[id]->closePort();
	mi[id]->cancelCallback();
	mi.erase(id);
	
}

void MyApp::killMIDI(){
	for(std::map<int, RtMidiIn *>::iterator i = mi.begin(); i != mi.end(); ++i){
		CloseMIDI(i->first);
	}
	delete rtmi;
	delete rtmo;
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

void MyApp::OnStore(wxCommandEvent &event){
	ofstream myfile;
	wxTextEntryDialog diag( NULL, _T("Enter a preset name"), _T("Preset store"));
	int r = diag.ShowModal();
	if( r == wxID_OK ){
		wxString fn = _T(PRESET_PATH);
		fn << diag.GetValue();
		myfile.open( fn.mb_str() );
		myfile << *bcr;
		myfile.close();
		menuPres->Append( LOADID + numPresets, diag.GetValue() );
	}
}

void MyApp::OnLoad(wxCommandEvent &event){
	wxString fn, menuname;
	
	menuname = menuPres->GetLabelText( event.GetId() );
	
	fn << _T(PRESET_PATH);
	fn << menuname;
	cout << fn.mb_str() << "\n";
	ifstream myfile( fn.mb_str() );
	string s;
	if( myfile and not myfile.eof() ){
		/*
		getline(myfile, s);
		cout << s << "\n";
		getline(myfile, s);
		cout << s << "\n";
		getline(myfile, s);
		cout << s << "\n";
		getline(myfile, s);
		cout << s << "\n";
		*/
		myfile >> *bcr;
		myfile.close();
		bcr->sendValues();
		bcr->execValues();
	}
}

// AUDIO SECTION

void MyApp::OpenAudio(int id){
	try { 
		Device &dev = asys->deviceByIndex(id);
		
		// is this an input or an output device?
		if( dev.maxInputChannels() > 0 ){
			if( stream_in == NULL) {
			
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
				
				wavePanel->SetSamples(rec->GetSamples());

				stream_in->stop();
				stream_in->close();
				delete stream_in;
				stream_in = NULL;
			}

		} else if ( dev.maxOutputChannels() > 0 ) {
		
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
		
		if( stream ) stream->start();
		if( stream_in ) stream_in->start();
		std::cout << "Ready.\n";

	} catch(PaException err){
		wxMessageBox( wxString(err.what(), wxConvUTF8));
	}
}

void MyApp::OnMnuAudio(wxCommandEvent &event){
	PaDeviceIndex adevid = event.GetId() - ADEVID;
	OpenAudio(adevid);
}


// MIDI SECTION

void OnRtMidi(double dt, MIDIMSG message, void *data){
	app->OnMIDI(dt, message, data);
}

void MyApp::OnMIDI(double dt, MIDIMSG message, void *data){
	//bcr->callback(dt, message, data);
	//std::cout << (int)data;
	MidiController *mc = (MidiController *)data;
	mc->callback(dt, message, data);
}

void MyApp::OpenMIDI(int mdevid){
	MidiController *receiver = bcr; // default
	
	mi[mdevid] = new RtMidiIn();
	
	if( mdevid >= rtmi->getPortCount() )
		return;
		
	wxString str(mi[mdevid]->getPortName(mdevid).c_str(), wxConvUTF8);
	
	if(str.Contains(_T("BCR2000"))){
		rtmo = new RtMidiOut();
		//for(int i=0; i < 8; i++){
		//	std::cout << rtmo->getPortName(i) << "\n";
		//}
		rtmo->openPort(mdevid);
		bcr->mo = rtmo;
		receiver = bcr;
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
	mi[mdevid]->openPort( mdevid );
	mdevHash[mdevid] = mi.size() - 1;
	//std::cout << std::string( event.GetMenu().mb_str()) << "\n";
	//std::cout << mi.back()->getPortName(mdevid) << "\n";
}

void MyApp::OnMnuMIDI(wxCommandEvent &event){
	int mdevid = event.GetId() - MDEVID;
	if( event.IsChecked() ){
		OpenMIDI(mdevid);
	} else {
		CloseMIDI(mdevid);
	}
}

void MyApp::OnMnuTune(wxCommandEvent &event){
	int tuning = event.GetId() - TUNEID;
	key->setTemper( Temperament::getTemp(tuning) );
}


// INIT SECTION

// initialize the MIDI menu, populated by RtMIDI
void MyApp::initMidi(wxMenu *menuDevs){ 

	TRYMIDI( rtmi = new RtMidiIn(); )
	wxMenuItem *title = new wxMenuItem(menuDevs, wxID_ANY, _T("MIDI:"), _T("MIDI"));
	title->Enable(false);
	menuDevs->Append(title);
	int nPorts = rtmi->getPortCount();
	for(int i=0; i < nPorts; i++){
		wxString dname( rtmi->getPortName(i).c_str(), wxConvUTF8 );
		wxString str(_T("    "));
		str << i << _T(": ") << dname;
		menuDevs->AppendCheckItem(MDEVID + i, str);
	}
	

	//mi->setCallback( &(MidiController::callback) ); // can't do this unless static
}

// Initialize the audio menu, populated by PortAudio
void MyApp::initAudio(wxMenu *menuDevs){ 
	System::initialize();
	asys = &( System::instance() );
	
	wxMenuItem *title = new wxMenuItem(menuDevs, wxID_ANY, _T("Audio:"), _T("Audio"));
	title->Enable(false);
	menuDevs->Append(title);
	
	//int nDevs = sys.deviceCount();
	for (System::DeviceIterator i = asys->devicesBegin(); i != asys->devicesEnd(); ++i) {
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

// initialize the tunings menu
void MyApp::initTemps(wxMenu *mnu){
	mnu->AppendRadioItem(TUNEID+0, _T("Equal temperament\tCtrl+1"));
	mnu->AppendRadioItem(TUNEID+1, _T("Just 5-limit intonation\tCtrl+2"));
	mnu->AppendRadioItem(TUNEID+2, _T("Kepler Tuning\tCtrl+3"));
	mnu->AppendRadioItem(TUNEID+3, _T("Meantone temperament\tCtrl+4"));
	mnu->AppendRadioItem(TUNEID+4, _T("Pythagorean tuning\tCtrl+5"));	
	mnu->AppendRadioItem(TUNEID+5, _T("Hammond tonewheels\tCtrl+6"));	
}

void MyApp::initPres(wxMenu *mnu)
{
	mnu->Append(wxID_PRINT, _T("&Store\tCtrl+P")  );
	mnu->AppendSeparator();
	// put menu items in from file list
	numPresets = 0;
	wxDir dir( _T("/Users/dctucker/.dnb" ));
	wxString fn;
	bool c = dir.GetFirst(&fn, wxEmptyString, wxDIR_DEFAULT & compl wxDIR_DIRS);
	while( c ){
		++numPresets;
		mnu->Append(LOADID + numPresets, fn);
		c = dir.GetNext(&fn);
	}
}

void MyApp::initMenubar(){
	wxMenuBar *menuBar = new wxMenuBar(wxMB_DOCKABLE);

	menuFile = new wxMenu;
	menuFile->Append(wxID_NEW          , _T("&New\tCtrl+N"));
	menuFile->Append(wxID_OPEN         , _T("&Open\tCtrl+O"));
	menuFile->Append(wxID_CLOSE        , _T("&Close\tCtrl+W"));
	menuFile->Append(wxID_PREFERENCES  , _T("Preferences...\tCtrl+."));
	menuFile->Append(wxID_EXIT         , _T("E&xit\tCtrl+Q"));

	menuDevs = new wxMenu;
	initAudio(menuDevs);

	menuDevs->AppendSeparator();
	initMidi(menuDevs);
	
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

	wxApp::SetExitOnFrameDelete(false);
	wxMenuBar::MacSetCommonMenuBar(menuBar);
}

#include <xmmintrin.h>

bool MyApp::OnInit()
{

	_mm_setcsr( _mm_getcsr() | 0x8040 ); // set DAZ and FZ bits

	INIT_PD;
	pd.Pulse(_T("Initializing"));
	
	app = this;
	bcr = NULL; keyFrame = frame = NULL; stream = NULL; stream_in = NULL;

	
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
	
	initMenubar();
	

	pd.Pulse(_T("Initializing GUI"));

	//setup frame for BCR-2000
	bcr = new MyBCR();
    frame = new wxFrame( NULL, -1,  wxT("BCR-2000"),
						wxPoint(50,350), wxSize(400,300), 
						( wxDEFAULT_FRAME_STYLE /* | wxFRAME_TOOL_WINDOW */ )
						& compl wxMAXIMIZE_BOX
				);
	bcrPanel = new BCRPanel(frame, bcr);
	bcr->panel = bcrPanel;
	//bcr->osc = osc;

	//setup frame for Keystation 88es
	keyFrame = new wxFrame( NULL, -1, _T("Keystation 88es"),
						wxPoint(50,700), wxSize(800, 100),
						( wxDEFAULT_FRAME_STYLE /* | wxFRAME_TOOL_WINDOW */ )
						& compl wxMAXIMIZE_BOX & compl wxRESIZE_BORDER );

	key = new MyKeys();
	keyPanel = new KeyboardPanel(keyFrame, key);
	keyPanel->SetMinSize(wxSize(400, 40));
	keyFrame->SetMinSize(wxSize(400, 40));
	keyFrame->SetMaxSize(wxSize(2000, 200));
	key->panel = keyPanel;
	key->sub   = sine;
	key->organ = osc;
	key->piano = piano;
	key->square= square;
	key->reese = reese;
	key->tri   = tri;
	key->brass = brass;
	key->noise = noise;
	key->bus   = bus;
	
	
	bcr->myKeys = key;
	
	bcr->setLev(&(bus->level));
	
	key->setOscs();

	/*
	// setup frame for the waveform editor
	waveFrame = new wxFrame( NULL, -1, _T("Waveform editor"),
						wxPoint(100,50), wxSize(400,200),
						( wxDEFAULT_FRAME_STYLE )
						& compl wxMAXIMIZE_BOX ); // & wxRESIZE_BORDER );
	
	wavePanel = new WavePanel(waveFrame);
	wavePanel->SetMinSize(wxSize(30,30));
	waveFrame->SetMinSize(wxSize(30,30));
	wavePanel->SetSamples(tri->GetTable());
	waveFrame->Show();
	*/
	
/*
	// setup the computer keyboard frame
	compkeyFrame = new wxFrame( NULL, -1, _T("Keyboard"), 
							wxPoint(100,100), wxSize(100,100),
							( wxDEFAULT_FRAME_STYLE )
							& ~wxMAXIMIZE_BOX
							& ~wxRESIZE_BORDER );
	
	compkeyPanel = new ComputerKeyboardPanel(compkeyFrame);
	compkeyPanel->SetMinSize(wxSize(100,100));
*/
	
	//bcr->wave = wavePanel;
	
	
	// Envelopes frame
	envFrame = new wxFrame( NULL, -1, _T("Envelopes"),
							wxPoint( 500, 50 ), wxSize( 200, 210 ),
							wxDEFAULT_FRAME_STYLE & compl wxMAXIMIZE_BOX );
	envFrame->SetOwnBackgroundColour(wxColor(0,0,0));
	envFrame->SetBackgroundStyle(wxBG_STYLE_COLOUR);
	
	wxStaticBitmap *instIcon[8];
	wxString path = Oscillator::GetSamplePath() << _T("../Bitmaps/");
	instIcon[0] = new wxStaticBitmap(envFrame, wxID_ANY, wxBitmap( path + _T("sine.bmp"),   wxBITMAP_TYPE_BMP ));
	instIcon[1] = new wxStaticBitmap(envFrame, wxID_ANY, wxBitmap( path + _T("piano.bmp"),  wxBITMAP_TYPE_BMP ));
	instIcon[2] = new wxStaticBitmap(envFrame, wxID_ANY, wxBitmap( path + _T("organ.bmp"),  wxBITMAP_TYPE_BMP ));
	instIcon[3] = new wxStaticBitmap(envFrame, wxID_ANY, wxBitmap( path + _T("saw.bmp"),    wxBITMAP_TYPE_BMP ));
	instIcon[4] = new wxStaticBitmap(envFrame, wxID_ANY, wxBitmap( path + _T("tri.bmp"),    wxBITMAP_TYPE_BMP ));
	instIcon[5] = new wxStaticBitmap(envFrame, wxID_ANY, wxBitmap( path + _T("square.bmp"), wxBITMAP_TYPE_BMP ));
	instIcon[6] = new wxStaticBitmap(envFrame, wxID_ANY, wxBitmap( path + _T("brass.bmp"),  wxBITMAP_TYPE_BMP ));
	instIcon[7] = new wxStaticBitmap(envFrame, wxID_ANY, wxBitmap( path + _T("noise.bmp"),  wxBITMAP_TYPE_BMP ));

	wxFlexGridSizer *envSize = new wxFlexGridSizer(8,3,0,0);//8, 2, 0, 0);
	//wxBoxSizer *envSize = new wxBoxSizer(wxVERTICAL);
	for(int i=0; i < 8; ++i){
		bcr->envPanel[i] = new ADSRPanel(envFrame);
		envSize->Add( instIcon[i], 0, wxALIGN_CENTER_VERTICAL );
		envSize->Add(bcr->envPanel[i], 1, wxEXPAND);
		bcr->envPanel[i]->SetMinSize(wxSize(100,20));
		
		bcr->filtPanel[i] = new FilterPanel(envFrame);
		envSize->Add( bcr->filtPanel[i], 1, wxEXPAND);
		bcr->filtPanel[i]->SetMinSize(wxSize(100,20));

		envSize->AddGrowableRow(i, 1);
	}
	envSize->AddGrowableCol(1, 1);

	envFrame->SetSizer(envSize);
	envFrame->Show();
	

	
	// Oscilloscope frame
	scopeFrame = new wxFrame( NULL, -1, _T("Oscilloscope"),
							wxPoint(100,50), wxSize(349,200),
							wxDEFAULT_FRAME_STYLE & compl wxMAXIMIZE_BOX );
	scopePanel = new ScopePanel(scopeFrame);
	timer = new wxTimer( scopePanel, 0 );
	scopePanel->SetMinSize(wxSize(30,30));
	scopeFrame->SetMinSize(wxSize(30,30));
	scopeFrame->Show();
	
	bus->sc = scopePanel;
	
	//done! show the frames
	frame->Show();
	keyFrame->Show();
	
	//compkeyFrame->Show();
	pd.Update(100, _T("Done."));
	
	// open my devices:
	OpenAudio(2);
	OpenMIDI(1);
	OpenMIDI(3);
	
	
	key->setTemper(Temperament::getTemp(0));

	bcr->sendValues();
	bcr->execValues();

	timer->Start(60);
    return true;
}
