#include "wx/wx.h"
#include "midi.h"

class BCRPanel: public wxWindow {
	wxWindow *m_owner;
	BCR2000 *bcr;
	wxBitmap *bmp;
	bool needUpdate, updating;
	float *level;
public:
	void setLev(float *);
	
	BCRPanel(wxWindow *, BCR2000*);
	void OnPaint(wxPaintEvent&);
	void OnIdle(wxIdleEvent &);
	void OnMIDIUpdate(wxCommandEvent&);
	void DrawButton(wxDC&, int);
	void DrawKnob(wxDC&, int);
	
	void Paint();
	
	void callback(double, MIDIMSG, void *);

	int width, height, sp;
	int bLeft, bTop, bWidth, bHeight;

	DECLARE_EVENT_TABLE();
};

class ComputerKeyboardPanel: public wxWindow {
	wxWindow *m_owner;
	wxBitmap *bmp;
public:
	ComputerKeyboardPanel( wxWindow * );
	void OnKeyDown( wxKeyEvent& );
	void OnKeyUp( wxKeyEvent& );
	DECLARE_EVENT_TABLE();
};

class KeyboardPanel: public wxWindow {
//	DECLARE_DYNAMIC_CLASS();
    wxWindow *m_owner;
	wxBitmap *bmp;
	MidiController *mc;
	float mod, pitch, fader, sustain;
public:

	bool keyon[128];
	KeyboardPanel(wxWindow *, MidiController*);
	void OnPaint(wxPaintEvent&);
	void drawOctave(wxDC &, int);
	void drawLowKeys(wxDC &);
	void drawPanel(wxDC &);
	
	void SetMod(float f){ mod = f; }
	void SetPitch(float f){ pitch = f; }
	void SetFader(float f){ fader = f; }
	void SetSustain(float f){ sustain = f; }
	void keyOn(int);
	void keyOff(int);

	int height, width, nOctaves, keysX;
	float keyspacing;
	DECLARE_EVENT_TABLE();
};
