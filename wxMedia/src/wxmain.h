/*
 *  wxmain.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2009-11-28.
 *  Copyright 2009 UNCW. All rights reserved.
 *
 */


#include <iostream>
#include <fstream>
#include <map>

#include <wx/textdlg.h>
#include <wx/dir.h>
#include <wx/event.h>
#include <wx/scrolwin.h>
#include <wx/menu.h>
#include <wx/menuitem.h>
#include <wx/wx.h>
#include <wx/panel.h>

#include "main.h"

#include "portaudiocpp/PortAudioCpp.hxx"
#include "RtMidi.h"
#include "sndfile.hh"

#include "midi.h"
#include "midigui.h"
#include "adsrui.h"
#include "joystick.h"
#include "arduino.h"

//#include "wavepanel.h"
//#include "delay.h"
#include "oscillator.h"
#include "bus.h"
#include "piano.h"
#include "hammond.h"
#include "reese.h"
#include "recording.h"
#include "biquad.h"

#include "canvas3d.h"

#define ADEVID 500
#define MDEVID 600
#define TUNEID 700
#define LOADID 800
#define SERIID 900

using namespace portaudio;

#define PRESET_PATH "/Users/casey/.dnb/"


#define INIT_PD 	wxProgressDialog pd(_T("Loading dnbMedia"),         \
					_T("______________________________________"), 88,    \
					NULL, wxPD_CAN_ABORT | wxPD_APP_MODAL)


#include "mydevices.h"

class MyApp: public wxApp {


	virtual bool OnInit();
	virtual int OnExit();

	void initMidi(wxMenu *);
	void initAudio(wxMenu *);
	void initSeri(wxMenu *);
	void initMenubar();
	void killMIDI();
	void killAudio();
	
	void initTemps(wxMenu *);
	void initPres(wxMenu *);
	
	void OpenAudio(int);
	void OpenMIDI(int);
	void CloseMIDI(int);

	wxMenu *menuFile, *menuDevs, *menuTune, *menuPres;
	int numPresets;

	wxString current_preset;
	std::map<int, RtMidiIn *> mi;
	System *asys;
	RtMidiIn *rtmi;
	RtMidiOut *rtmo;
	//MemFunCallbackStream<Oscillator> *stream;
	MemFunCallbackStream<Bus> *stream;
	MemFunCallbackStream<Recorder> *stream_in;

	SubSineOsc *sine;
	Oscillator *osc, *square, *tri, *noise;
	Reese *reese;
	Brass *brass;
	Bus *bus;
	Recorder *rec;
	Upright *piano;

 	MyBCR *bcr;
	MyKeys *key;
	//MyGamepad *joy;
	MyDrumIn *drums;

    wxFrame *frame, *envFrame; //, *compkeyFrame;
	wxFrame *waveFrame, *scopeFrame;
	wxFrame *glFrame;
	//wxFrame *joyFrame;
	
	
	KeyboardPanel *keyPanel;
	BCRPanel *bcrPanel;
	//ComputerKeyboardPanel *compkeyPanel;
	//WavePanel *wavePanel;
	//ScopePanel *scopePanel;
	ADSRPanel *envPanel;
	
	Canvas3D *glCanvas;
	
	wxTimer *timer;
	
	std::map<int, int> mdevHash;
	
	void OnMnuAudio (wxCommandEvent &);
	void OnMnuMIDI  (wxCommandEvent &);
	void OnMnuSeri  (wxCommandEvent &);
	void OnMnuTune  (wxCommandEvent &);
	
	void OnAbout(wxCommandEvent &);
	void OnOpen(wxCommandEvent &);
	void OnClose(wxCommandEvent &);
	void OnStore(wxCommandEvent &);
	void OnDelete(wxCommandEvent &);
	void OnLoad(wxCommandEvent &);
	void OnLoadSamples(wxCommandEvent &);
	void OnQuit(wxCommandEvent &);

public:
	void OnMIDI (double, std::vector<unsigned char> *, void *);
	
	DECLARE_EVENT_TABLE();
};
