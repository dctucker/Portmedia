#ifndef DCT_GLOBALS
#define DCT_GLOBALS

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
#include "myframe.h"

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


#define NUM_OSCS 8

#endif
