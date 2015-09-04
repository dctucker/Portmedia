#include "midigui.h"
#include "wx/dcbuffer.h"
#include "wx/dcmemory.h"
#include "wx/dc.h"
#include "wx/wx.h"
#include "wx/event.h"

//IMPLEMENT_DYNAMIC_CLASS(KeyboardPanel, wxScrolledWindow)

//#define GETDC wxBufferedPaintDC pdc(this, *bmp, wxBUFFER_CLIENT_AREA); /*wxGCDC gdc( pdc ); */ wxDC &dc = (wxDC&) pdc; //PrepareDC(dc); //m_owner->PrepareDC(dc);
#define GETDC wxPaintDC pdc(this); /*wxGCDC gdc( pdc ); */ wxDC &dc = (wxDC&) pdc; //PrepareDC(dc); //m_owner->PrepareDC(dc);

BEGIN_EVENT_TABLE(ComputerKeyboardPanel, wxWindow)
	EVT_KEY_DOWN( ComputerKeyboardPanel::OnKeyDown )
	EVT_KEY_UP( ComputerKeyboardPanel::OnKeyUp )
END_EVENT_TABLE()


BEGIN_EVENT_TABLE(KeyboardPanel, wxWindow)
    EVT_PAINT( KeyboardPanel::OnPaint )
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(BCRPanel, wxWindow)
EVT_PAINT( BCRPanel::OnPaint )
//EVT_IDLE( BCRPanel::OnIdle )
//EVT_COMMAND( wxID_ANY, wxEVT_COMMAND_SLIDER_UPDATED, BCRPanel::OnMIDIUpdate )

END_EVENT_TABLE()

ComputerKeyboardPanel::ComputerKeyboardPanel(wxWindow *parent) : 
	wxWindow(parent, wxID_ANY)//, width(100), height(100)
{
	m_owner = parent;
}

void ComputerKeyboardPanel::OnKeyDown(wxKeyEvent& event){
	std::cout << event.GetKeyCode();
	std::cout << std::endl;
}
void ComputerKeyboardPanel::OnKeyUp(wxKeyEvent& event){
	//std::cout << event.GetKeyCode();
	//std::cout << std::endl;
	wxMessageBox(_("KeyUp"));
}



BCRPanel::BCRPanel(wxWindow *parent, BCR2000 *b) :
	wxWindow(parent, wxID_ANY), width(300), height(200)
{
	m_owner = parent;
	bcr = b;
	sp = 3;
	//bmp = new wxBitmap(width, height);
	updating = false;

}

void BCRPanel::OnIdle(wxIdleEvent & WXUNUSED(event)){
	if(needUpdate){
		//Refresh();
		Paint();
		Update();
	}
}


void BCRPanel::OnMIDIUpdate(wxCommandEvent &event){
	Refresh();
	//Update();
}

void BCRPanel::setLev(float *l){
	level = l;
}

void BCRPanel::DrawButton(wxDC& dc, int btn){
	int i = btn / 10;
	int j = btn % 10;
	int row; // = bHeight * i;
	
	int col = bWidth * (j - 1);
	
	switch(i){
		case 4: case 5: case 6: case 7:
			row = 1;
		break;
		case 8:
			row = 4;
		break;
		case 9:
			row = 5;
		break;
		case 10:
			if( j == 1 or j == 2 ){
				row = bHeight * 9;
				col = bWidth * (8 + j);
			} else {
				row = bHeight * 10;
				col = bWidth * (6 + j);
			}
		break;
	}

	float val = bcr->getButton(btn);

	if(val > 0.0){
		dc.SetPen  (wxColor( 0x00, 0x00, 0x33 ));
		dc.SetBrush(wxColor( 0xFF, 0x00, 0x00 ));
	} else {
		dc.SetPen  (wxColor( 0x00, 0x00, 0x33 ));
		dc.SetBrush(wxColor( 0x33, 0x00, 0x00 ));
	}
	
	int x = sp + bLeft + col;
	int y = bTop + bHeight * row + sp;
	int w = bWidth - 2 * sp;
	int h = bHeight - 2 * sp;
	dc.DrawRectangle( x, y, w, h );

	wxString text;
	text << btn;
	int tw, th;
	dc.GetTextExtent(text, &tw, &th);
	dc.DrawText( text, x+w/2 - tw/2, y+h/2 - th/2 );

}

void BCRPanel::DrawKnob(wxDC& dc, int kn){
	int i = kn / 10;
	int j = kn % 10;
	int row, col;
	
	col = bWidth * (j - 1);
	switch(i){
		case 1: case 2: case 3:
			row = (4 + 2 * i);
		break;
		case 4: 
			row = 0;
			break;
		
		case 5:
			row = 1;
			break;
	
		case 6:
			row = 2;
			break;
		case 7:
			row = 3;
			break;
		break;
	}
	
	int x = sp + bLeft + col;
	int y = bTop + bHeight * row + sp;
	int w = bWidth - 2 * sp;
	int h = bHeight - 2 * sp;

	float val = bcr->getKnob(kn);
	if( kn == 38 )
		val = *level;

	dc.SetPen  (wxColor( 0x00, 0x00, 0x33 ));
	dc.SetBrush(wxColor( 0x00, 0x00, 0x00 ));
	dc.DrawRectangle( x, y, w, h );

	dc.SetBrush(wxColor( 0xAA, 0x00, 0x00 ));
	dc.DrawRectangle( x, y, w * val, h );
	
	wxString text;
	text << wxString::Format(_T("%.3f"), val);
	int tw, th;
	dc.GetTextExtent(text, &tw, &th);
	dc.DrawText( text, x+w/2 - tw/2, y+h/2 - th/2 );

}

void BCRPanel::Paint(){
/*
	wxMemoryDC dc(*bmp);
	
	wxSize s = GetClientSize();
	width = s.GetWidth();
	height = s.GetHeight();
...		
	wxClientDC cdc(this);
	cdc.Blit(0, 0, 300, 200, &dc, 0, 0);
*/
}


void BCRPanel::OnPaint(wxPaintEvent &WXUNUSED(event)){
	if(not updating){
		updating = true;

		GETDC;
		
		wxSize s = GetClientSize();
		width = s.GetWidth();
		height = s.GetHeight();
		bLeft = 0; bTop = 0;
		bWidth = (width / 8); bHeight = height / 11;

		
		dc.SetBackground(wxBrush(wxColor(0x22,0x22,0x44)));
		dc.Clear();
		
		dc.SetFont(wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTWEIGHT_NORMAL, 0));
		dc.SetTextForeground(wxColor(0xee,0xee,0xee));

		for( int i = 0; i < 8; i++ ){
			DrawButton(dc, 41 + i);
			DrawButton(dc, 81 + i);
			DrawButton(dc, 91 + i);
		}
		DrawButton(dc, 101);
		DrawButton(dc, 102);
		DrawButton(dc, 103);
		DrawButton(dc, 104);

		for(int i=0; i < 8; i++){
			DrawKnob(dc, 11 + i);
			DrawKnob(dc, 21 + i);
			DrawKnob(dc, 31 + i);
			DrawKnob(dc, 41 + i);
			DrawKnob(dc, 51 + i);
			DrawKnob(dc, 61 + i);
			DrawKnob(dc, 71 + i);
		}	

		//	DrawKnob(dc, 11);

		updating = false;
	}

}

void BCRPanel::callback(double dt, MIDIMSG msg, void *data){
	bcr->callback(dt, msg, data);
	needUpdate = true;
}


//////////////////////////////////////////////////////////////////////////////



KeyboardPanel::KeyboardPanel(wxWindow *parent, MidiController *mc1) :
	wxWindow(parent, wxID_ANY),
	nOctaves(7), height(20), width(800), keysX(50),
	pitch(0.0), mod(0.5), fader(0.5)
{
	//width = keyspacing * nOctaves * 14;
	//keyspacing = width / 88;
	m_owner = parent;
	mc = mc1;
	//parent->SetSizeHints(wxSize(400, 80), wxSize(2000, 200));
	for(int i=0; i<128; i++){
		keyon[i] = false;
	}
	
}

void KeyboardPanel::OnPaint(wxPaintEvent &WXUNUSED(event)){
	int w, h;
	GetClientSize(&w, &h);
	width = w; height = h;
	keyspacing = 12. * (width - keysX) / (89. * 14.) ;


	GETDC;
		
	wxColor c1( 0x22, 0x22, 0x44);
	wxBrush b1(c1, wxSOLID);
	
	dc.SetBackground(b1);
	dc.Clear();

	dc.SetPen(wxColor(0xaa,0xaa,0xaa));
	dc.SetBrush(wxColor( 0xbb, 0xbb, 0xbb));
	dc.DrawRectangle( 0, 0, keysX, height);
	
	drawPanel(dc);
	drawLowKeys(dc);
	
	for(int o = 0; o < nOctaves; o++){	
		drawOctave(dc, o);
	}

	
	float noteval[12] = { 0, 1.5, 1, 3.5, 2, 3, 7.5, 4, 9.5, 5, 11.5, 6 };
	dc.SetPen(  wxColor(0xff, 0x00, 0x00));
	dc.SetBrush(wxColor(0xff, 0x00, 0x00));
	for(int i=0; i<128; i++){
		if(keyon[i]){
			int n = i % 12;
			int o = i / 12;
			//int ox = 4 * keyspacing + 14.0 * keyspacing * o;

			if( n==1 or n==3 or n==6 or n==8 or n==10){ // black keys
				dc.DrawRectangle( 
					keyspacing * 4 + keysX + keyspacing / 2 + ((o-2) * 14 * keyspacing) + noteval[n] * keyspacing - keyspacing / 2,
					height / 2 - keyspacing,
					keyspacing - 1, keyspacing );			
			} else {// white key
				dc.DrawRectangle( 
					keyspacing * 4 + keysX + keyspacing / 2 + ((o-2) * 14 * keyspacing) + noteval[n] * keyspacing * 2,
					height - keyspacing,
					keyspacing, keyspacing );			
			}
		}
	}
	
}

void KeyboardPanel::drawPanel(wxDC &dc){
	dc.SetPen(wxColor(0x00,0x00,0x00));
	dc.SetBrush(wxColor(0x33,0x33,0x33));
	
	//draw pitch wheel
	dc.DrawRectangle(     keysX / 5,  0.6 * height,  keysX / 5,  0.3 * height );
	
	//draw mod wheel
	dc.DrawRectangle( 3 * keysX / 5,  0.6 * height,  keysX / 5,  0.3 * height );
	
	//draw fader
	dc.DrawRectangle(     keysX / 5,  0.1 * height,  keysX /10,  0.3 * height );
	
	//draw pedal
	dc.DrawRectangle(     keysX / 2,  0.1 * height,  keysX / 5,  0.3 * height );
	dc.SetPen(wxColor(0x00,0x00,0x00));
	dc.SetBrush(wxColor(0x66,0x66,0x66));
	dc.DrawRectangle(     keysX / 2 + keysX/20,  0.1 * height,  1+keysX /10,  0.2 * height );
	dc.DrawRectangle(     keysX / 2 + keysX/40,  0.2 * height,  1+keysX /5 - keysX/40,  0.1 * height * sustain  );
	
	dc.SetBrush(wxColor(0x00,0x00,0x00));
	dc.SetPen(wxColor(0x66,0x66,0x66));

	//draw value of pitch bend wheel9
	dc.DrawRectangle( 0.2 * keysX, 0.6 * height + ( 0.5 * ( 1.0 - pitch) * 0.3 * height) - 0.1 * keysX,
					0.2 * keysX, 0.2 * keysX);
	
	//draw value of mod wheel
	dc.DrawRectangle( 0.6 * keysX, 0.6 * height + ((1.0 - mod) * 0.3 * height) - 0.1 * keysX,
					0.2 * keysX, 0.2 * keysX);

	//draw value of fader
	dc.DrawRectangle( 0.15 * keysX, 0.1 * height + ((1.0-fader) * 0.3 * height) - 0.05 * keysX,
					0.2 * keysX, 0.1 * keysX);
					

	//keyboard area
	dc.SetPen(wxColor(0xaa,0xaa,0xaa));
	dc.SetBrush(wxColor( 0xff, 0xff, 0xff));
	dc.DrawRectangle( keysX, 0, width, height);
	
}

void KeyboardPanel::keyOn(int k){
	keyon[k] = true;
}

void KeyboardPanel::keyOff(int k){
	keyon[k] = false;
}


#define vLine( col )  dc.DrawLine( keysX + ox + keyspacing * col, 0, keysX + ox + keyspacing * col, height - 1);
#define bkRect( ksm ) dc.DrawRectangle( keysX + ox + ksm * keyspacing, 0, keyspacing, height / 2 );

void KeyboardPanel::drawLowKeys(wxDC &dc){
	int ox = 0;

	dc.SetPen   ( wxColor( 0xaa, 0xaa, 0xaa ) );
	dc.SetBrush ( wxColor( 0xff, 0xff, 0xff ) );
	vLine( 2 );
	vLine( 4 );
	
	dc.SetPen(wxColor(0x33, 0x33, 0x33));
	dc.SetBrush(wxColor(0,0,0));
	bkRect( 1.5 );
}

void KeyboardPanel::drawOctave(wxDC &dc, int o){
	int ox = 4 * keyspacing + 14.0 * keyspacing * o;

	//white key outlines
	dc.SetPen   ( wxColor( 0xaa, 0xaa, 0xaa ) );
	dc.SetBrush ( wxColor( 0xff, 0xff, 0xff ) );
	for(int i = 2; i <= 14; i += 2){ // 2,4,6,8,10,12,14
		vLine( i );
	}

	//black keys
	dc.SetPen(wxColor(0x33, 0x33, 0x33));
	dc.SetBrush(wxColor(0,0,0));
	bkRect(  1.5 );
	bkRect(  3.5 );
	bkRect(  7.5 );
	bkRect(  9.5 );
	bkRect( 11.5 );
}




/* Novation X-something

JOYSTICK		phantom		
 x = pitch		gain		
 y = mod		pan				
																		  |		  |		   |	   |		|	   |		|	   |	   |
				octave ±	lineout	monitor	phones	<<	>>	[]	>	O	attack	decay	sustain	release	|attack	decay	sustain	release	velocity	trigger
XY							level	in/out	vol																										.
PAD							
				49 keys		


*/
