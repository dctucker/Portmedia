/*
 *  wxmain.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2009-11-28.
 *  Copyright 2009 UNCW. All rights reserved.
 *
 */

#include "globals.h"
#include "myarduino.h"
#include "mykeys.h"
#include "mybcr.h"
#include "mydrumin.h"

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

	wxMenu *menuFile, *menuMidi, *menuAsio, *menuSeri, *menuTune, *menuPres;
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

	void OnSize(wxSizeEvent &);

public:
	void OnMIDI (double, std::vector<unsigned char> *, void *);
	
	DECLARE_EVENT_TABLE();
};
