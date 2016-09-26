/*
 *  joystick.cpp
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-04-22.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */
/*
#include "joystick.h"
#include <iostream>

BEGIN_EVENT_TABLE(DualAction, wxPanel)
    EVT_JOYSTICK_EVENTS(DualAction::OnJoystickEvent)
END_EVENT_TABLE()

DualAction::DualAction(wxWindow *parent)
	: wxPanel(parent)
{
	m_stick = NULL;
	m_stick = new wxJoystick(wxJOYSTICK1);
	if( not m_stick->IsOk() )
		return;
	
	std::cout << m_stick;
	nButtons = m_stick->GetNumberButtons();
	m_stick->SetCapture(this, 10);	
	for(int i=0; i < 32; i++){
		a[i] = 0;
		b[i] = 0;
	}
}

DualAction::~DualAction()
{
	m_stick->ReleaseCapture();
	delete m_stick;
}

void DualAction::OnJoystickEvent(wxJoystickEvent& event)
{
	wxPoint xy = event.GetPosition();
	int z = event.GetZPosition();
	int btn = event.GetButtonState();
	int s;
	

	//std::cout << "Joy"
	//	<< xy.x << "," << xy.y << " "
	//	<< z  << " "
	//	<< btn
	//	<< "\n";
	
		
	if( a[0] != xy.x )
	{
		a[0] = xy.x;
		axis(0, a[0] / 127.);
	}
	if( a[1] != xy.y )
	{
		a[1] = xy.y;
		axis(1, a[1] / 127.);
	}
	if( a[2] != z )
	{
		a[2] = z;
		axis(2, a[2] / 127.);
	}
	for(int i=0; i < 32; i++)
	{
		s = (btn & ( 1 << i )) >> i;
		if( b[i] != s ){
			b[i] = s;
			button(i, b[i]);
		}
	}
}
*/