/*
 *  scope.cpp
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-02-07.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */

#include "scope.h"

BEGIN_EVENT_TABLE(ScopePanel, wxWindow)
    EVT_PAINT( ScopePanel::OnPaint )
	EVT_SIZE( ScopePanel::OnSize )
	EVT_IDLE( ScopePanel::OnIdle )
	EVT_TIMER( 0, ScopePanel::OnTimer )
END_EVENT_TABLE()

ScopePanel::ScopePanel(wxWindow *parent) : 
	wxWindow(parent, wxID_ANY)
	//pos(0)
{
}
/*
void ScopePanel::append(float l, float h){
	maxv[pos] = h;
	minv[pos] = l;
	++pos;
	if(pos > width) pos = 0;
}
*/
void ScopePanel::SetBuffers(float *mx, float *mn)
{
	maxv = mx;
	minv = mn;
}

void ScopePanel::OnTimer(wxTimerEvent & WXUNUSED(event)){
	Refresh();
}
void ScopePanel::OnIdle(wxIdleEvent & WXUNUSED(event)){
	Refresh();
}

void ScopePanel::OnSize(wxSizeEvent& e){
	wxSize s = e.GetSize();
	height = s.GetHeight();
	width = s.GetWidth();
	//m_scrollbar->SetSize( 0, height - 16, width - 16, 16 );
}

void ScopePanel::OnPaint(wxPaintEvent &WXUNUSED(event)){
	//painting = false;
	//if(!painting){
		//painting = true;
		GETDC;
				
		wxSize s = GetClientSize();
		width = s.GetWidth();
		height = s.GetHeight() - 1;// - scroll_s.GetHeight();

		int center_y = height / 2;

		dc.SetBackground(wxBrush(wxColor(0x00,0x22,0x11)));
		dc.Clear();
		
		dc.SetPen(wxPen(wxColor(0x00,0xff,0x00)));

		for(int i=0; i < width; i++){
			dc.DrawLine(i, center_y - height * maxv[i], i, center_y - height * minv[i] );
		}
}
