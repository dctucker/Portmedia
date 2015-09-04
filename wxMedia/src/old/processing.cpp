#include "wx/event.h"
#include "wx/scrolwin.h"
#include "wx/wx.h"

#include "processing.h"
#include <iostream>


class Ctrl: public wxPanel {
public:
	wxFrame *m_owner;
	//MidiController *device;
	wxFont *f;
	
	wxString &nf(float n, int k);
	
	int posX, posY, posW, posH;
	Ctrl(wxFrame *parent) : wxPanel(parent), posW(32){
		f = new wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

	}
	void setPos(int x, int y){
		posX = x; posY = y;
	}

  void OnPaint(wxPaintEvent& WXUNUSED(event)){
	wxPaintDC pdc(this);

    wxGCDC gdc( pdc ) ;
	wxDC &d = gdc;
	maindc = &d;
	
	PrepareDC(*maindc);
	m_owner->PrepareDC(*maindc);
		std::cout << "paint\n";


	maindc->SetBackground(wxColor(0,0,0));
	maindc->Clear();

	display();
  }

  virtual void display(){};
  DECLARE_EVENT_TABLE();

};

BEGIN_EVENT_TABLE(Ctrl, wxPanel)
	EVT_PAINT(Ctrl::OnPaint)
END_EVENT_TABLE()

class Button: public Ctrl {
public:
  int note;
  int vel;
  Button (wxFrame *w, int n) : Ctrl(w) {
    note = n;
  }
  void press(int v){
    vel = v;
  }
  void display (){
     fill(0, vel, vel);
     rect(posX - posW/2, posY - 5, posW, 10);
     fill(0xcccccc);
     textFont(f);
     textAlign(wxCENTER, wxCENTER);
     //text(_T("") + note, posX, posY);
  }
  DECLARE_EVENT_TABLE();
};

BEGIN_EVENT_TABLE(Button, Ctrl)
	EVT_PAINT(Ctrl::OnPaint)
END_EVENT_TABLE()



class Knob: public Ctrl {
public:
  int cc;
  //int val;
  float minv, maxv, val;
  float step;
  
  boolean relative;
  public:
  Knob(wxFrame *w, int _cc) : Ctrl(w) {
    cc = _cc;
    step = 0.002;
    minv = 0;
    maxv = 1.0;
  }
  void setRange(float v1, float v2){
    minv = v1;
    maxv = v2;
  }
  void setRange(float v1, float v2, float s){
    minv = v1;
    maxv = v2;
    step = s;
  }
  void setStep(float s){
    step = s;
  }
  void change(int v){
    if(relative){
      if(v >= 64){
        val = constrain( val - step * (128 - v), minv, maxv );
      } else {
        val = constrain( val + step * v, minv, maxv);
      }
    } else {
      val = v;
    }
  }
  void display(){
    textFont(f);
    textAlign(CENTER, CENTER);
    noStroke();
    strokeWeight(1);
    fill(0x000000);
    rect(posX - posW/2, posY - 5, posW, 10);
    fill(0xff0000);
    rect(posX - posW/2, posY - 5, map(val, minv,maxv, 0,posW) , 10);
    fill(0x0099ff);
    //text( _T("") + cc , posX, posY);
  }
  void setRelative(boolean tf){
    relative = tf;
  }
  void setValue(float v){
    val = v;
  }
};

/*

static int row[13] = { 70,85,100, 10,10,10,10, 20,20,20,20, 40,50 };

class BCRPanel: public wxScrolledWindow {
	DECLARE_DYNAMIC_CLASS()
public:
  Knob   *knob[128];
  Button *button[128];
  int posX, posY;
  int lastMovedRow;
	wxFrame *m_owner;
  
public:
	BCRPanel(){}
	BCRPanel(wxFrame *parent) //: wxScrolledWindow(parent),
		: wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                           wxHSCROLL | wxVSCROLL | wxNO_FULL_REPAINT_ON_RESIZE)
		,
		lastMovedRow(4) {
		m_owner = parent;
		setup(0,0);
		//std::cout << " bcrPanel\n";
	}
	void setup(int x, int y){

    posX = x;
    posY = y;
    //knob = new Knob[127];
    //button = new Button[127];    
    
    for(int j=1; j <= 7; j++){
      int i1 = 1 + (10 * j);
      for(int i=i1; i < i1+8; i++){
        knob[i] = new Knob(m_owner, i);
        knob[i]->setPos(posX + (*knob[i]).posW * (i - i1), row[j - 1] + posY);
        knob[i]->setRelative(true);
      }
    }
    for(int j=4; j <= 9; j++){
      int i1 = 1 + 10 * j;
      for(int i=i1; i < i1+8; i++){
        button[i] = new Button(m_owner, i);
        button[i]->setPos(posX + button[i]->posW * (i - i1), row[j + 3]  + posY);
      }
    }
    button[101] = new Button(m_owner, 101);
    button[102] = new Button(m_owner, 102);
    button[101]->setPos(int(posX + button[101]->posW * 8.5), row[1] + posY);
    button[102]->setPos(int(posX + button[102]->posW * 9.5), row[1] + posY);
    button[103] = new Button(m_owner, 103);
    button[104] = new Button(m_owner, 104);
    button[103]->setPos(int(posX + button[103]->posW * 8.5), row[2] + posY);
    button[104]->setPos(int(posX + button[104]->posW * 9.5), row[2] + posY);
    
  }
  void OnPaint(wxPaintEvent&);
  float change(int c, int v){
    knob[c]->change(v);
    if(c > 40 and c < 80) lastMovedRow = c / 10;
    return knob[c]->val;
  }
  void press(int n, int v){
    button[n]->press(v);
    if(n > 40 and n < 80) lastMovedRow = n / 10;
  }
  void setRange(int c, float minv, float maxv, float step){
    knob[c]->setRange(minv,maxv, step);
  }
  void setKnob(int n, float v){
    knob[n]->setValue(v);
  }
  DECLARE_EVENT_TABLE();
};

BEGIN_EVENT_TABLE(BCRPanel, wxScrolledWindow)
	EVT_PAINT(BCRPanel::OnPaint)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(BCRPanel, wxScrolledWindow)

void BCRPanel::OnPaint(wxPaintEvent& WXUNUSED(event)){
	std::cout << "paint\n";
  	wxPaintDC pdc(this);

    wxGCDC gdc( pdc ) ;
	wxDC &d = gdc;
	maindc = &d;
	
	PrepareDC(*maindc);
	m_owner->PrepareDC(*maindc);


	maindc->SetBackground(wxColor(0,0,0));
	maindc->Clear();
  
    strokeWeight(1);
	std::cout << "bcr paint";
    int i1 = lastMovedRow * 10 + 1;
    fill(0x111133);
    rect(posX - knob[41]->posW, posY, knob[41]->posW * 11.5, 120);
    for(int i=i1; i < i1 + 8; i++){
      knob  [i]->display();
      button[i]->display();
    }
    for(int i=0; i < 40; i++){
      if(knob[i] != NULL){
        knob[i]->display();
		}
	}
    for(int i=80; i < 128; i++){
      if(button[i] != NULL){
        button[i]->display();
      }
    }
    fill(0x330000);
    rect(posX + knob[41]->posW * 8, row[4] + posY, knob[41]->posW * 2, 20 );
    fill(0xff0000);
    switch(lastMovedRow){
      case 4: rect(posX + knob[41]->posW * 8, row[4] + posY, knob[41]->posW, 10);
      break;
      case 5: rect(posX + knob[41]->posW * 9, row[4] + posY, knob[41]->posW, 10);
      break;
      case 6: rect(posX + knob[41]->posW * 8, row[8] + posY, knob[41]->posW, 10);
      break;
      case 7: rect(posX + knob[41]->posW * 9, row[8] + posY, knob[41]->posW, 10);
      break;
    }
}
*/
