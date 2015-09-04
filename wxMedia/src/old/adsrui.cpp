/*
 *  scope.cpp
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-02-07.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */

#include "adsrui.h"

BEGIN_EVENT_TABLE(ADSRPanel, wxWindow)
    EVT_PAINT( ADSRPanel::OnPaint )
	EVT_SIZE( ADSRPanel::OnSize )
	EVT_IDLE( ADSRPanel::OnIdle )
END_EVENT_TABLE()

ADSRPanel::ADSRPanel(wxWindow *parent) : 
	wxWindow(parent, wxID_ANY),
	pos(0), a(30), d(30), s(50), r(30)
{
}

void ADSRPanel::set(float a1, float d1, float s1, float r1)
{
	a = a1;
	d = d1;
	s = s1;
	r = r1;
	//Update();
}

void ADSRPanel::OnIdle(wxIdleEvent & WXUNUSED(event)){
	Refresh();
}

void ADSRPanel::OnSize(wxSizeEvent& e){
	wxSize s = e.GetSize();
	height = s.GetHeight();
	width = s.GetWidth();
	
	//m_scrollbar->SetSize( 0, height - 16, width - 16, 16 );
}
void ADSRPanel::OnPaint(wxPaintEvent &WXUNUSED(event)){
	//painting = false;
	//if(!painting){
		//painting = true;
		GETDC;
				
		wxSize sz = GetClientSize();
		width = sz.GetWidth();
		height = sz.GetHeight() - 1;// - scroll_s.GetHeight();

		//int center_y = height / 2;
		int w = width / 2,
			a1 = 0  + a * w,
			d1 = a1 + d * w,
			s1 = height - s * (height - 1),
			r1 = d1 + r * w
			;

		dc.SetBackground(wxBrush(wxColor(0x00,0x00,0x11)));
		dc.Clear();
		
		dc.SetPen(wxPen(wxColor(0x00,0xff,0x00)));
		
		wxPoint pA[3] = { 
			wxPoint(1, height),
			wxPoint(a1, height),
			wxPoint(a1, 1)
		};
		wxPoint pD[4] = {
			wxPoint(a1, height),
			wxPoint(a1, 1),
			wxPoint(d1, s1),
			wxPoint(d1, height)
		};
		wxPoint pR[3] = {
			wxPoint(d1,height),
			wxPoint(d1, s1),
			wxPoint(r1, height),
		};

		dc.SetBrush(wxBrush(wxColor( 0xff, 0xff, 0x00 )));
		dc.SetPen  (wxPen  (wxColor( 0xff, 0xff, 0x00 )));
		dc.DrawPolygon(3, pA);

		dc.SetBrush(wxBrush(wxColor( 0xff, 0x88, 0x33 )));
		dc.SetPen  (wxPen  (wxColor( 0xff, 0x88, 0x33 )));
		dc.DrawPolygon(4, pD);

		dc.SetBrush(wxBrush(wxColor( 0xff, 0x22, 0x11 )));
		dc.SetPen  (wxPen  (wxColor( 0xff, 0x22, 0x11 )));
		dc.DrawPolygon(3, pR);

}



BEGIN_EVENT_TABLE( FilterPanel, wxWindow)
    EVT_PAINT( FilterPanel::OnPaint )
	EVT_SIZE( FilterPanel::OnSize )
	EVT_IDLE( FilterPanel::OnIdle )
END_EVENT_TABLE()

FilterPanel::FilterPanel(wxWindow *parent) : 
	wxWindow(parent, wxID_ANY),
	f1(30), q1(30), f2(30), q2(30)
{
}

void FilterPanel::set(float f10, float q10, float f20, float q20)
{
	f1 = f10;
	q1 = q10;

	f2 = f20;
	q2 = q20;
	//Update();
}

void FilterPanel::OnIdle(wxIdleEvent & WXUNUSED(event)){
	Refresh();
}

void FilterPanel::OnSize(wxSizeEvent& e){
	wxSize s = e.GetSize();
	height = s.GetHeight();
	width = s.GetWidth();
	
	//m_scrollbar->SetSize( 0, height - 16, width - 16, 16 );
}
void FilterPanel::OnPaint(wxPaintEvent &WXUNUSED(event)){
	//painting = false;
	//if(!painting){
		//painting = true;

		GETDC;
				
		wxSize sz = GetClientSize();
		width = sz.GetWidth();
		height = sz.GetHeight() - 1;// - scroll_s.GetHeight();

		//int center_y = height / 2;
		//int w = width / 2;

		dc.SetBackground(wxBrush(wxColor(0x00,0x00,0x11)));
		dc.Clear();
		
		dc.SetPen(wxPen(wxColor(0x00,0xff,0x00)));
		
		float g1 = sqrtf(f1/20000.), g2 = sqrtf(f2/20000.);
		float r1 = ( 1 - q1 ), r2 = ( 1 - q2 );
		
		int x1 = width * g1 - 10,
			x2 = width * g2;
		
		
		if( x1 + 12 < x2 ){		
			wxPoint p[6] = {
				wxPoint( x1 - 8 * r1 - 2, height ),
				wxPoint( x1             , height * r1 ),
				wxPoint( x1 + 8 * r1 + 2, height/2 ),
				//wxPoint(  width/2, height/2),
				
				wxPoint( x2 - 8 * r2 - 2, height/2 ),
				wxPoint( x2             , height * r2 ),
				wxPoint( x2 + 8 * r2 + 2, height )
			
			};
			dc.DrawSpline( 6, p );
		} else {

			wxPoint p[4] = {
				wxPoint( x1 - 10, height ),
				wxPoint( x1     , height * (1-q1) ),
				wxPoint( x2     , height * (1-q2)),
				wxPoint( x2 + 10, height )
			
			};
			dc.DrawSpline( 4, p );
		
		}
		
		dc.SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTWEIGHT_NORMAL, 0));
		dc.SetTextForeground(wxColor(0xee,0xcc,0x00));
		wxString str;
		str.Printf(_T("%4.0f"), f1); dc.DrawText(str, 0, 0);
		str.Printf(_T("%4.0f"), f2); dc.DrawText(str, width - 48, height - 12);
		//str.Printf(_T("%1.1f"), q1); dc.DrawText(str, 0, height - 12);
		//str.Printf(_T("%1.1f"), q2); dc.DrawText(str, width - 48, height - 12);

}
