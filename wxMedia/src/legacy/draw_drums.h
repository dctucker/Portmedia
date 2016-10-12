/*
 *  draw_drums.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-07-22.
 *  Copyright 2010. All rights reserved.
 *
 */

void Canvas3D::drawBPM()
{
	glPushMatrix();
	glTranslatef( 0.9, 0.0, 0.2f );
	glScalef( 0.015, 0.03, 1.0 );
	
	char bpmstr[256];
	sprintf(bpmstr, "%05.2f", bpm);

	drawLED( bpmstr, 0.f,0.f,1.f, 1.0 );
	glPopMatrix();	
}



void Canvas3D::hitDrum(int i)
{
	drums[ i % 7 ] = 1.0;
}

void Canvas3D::drawDrums()
{
	// metronome code
	int c1 = *count % (int)(SAMPLE_RATE * 60.f/(bpm));
	if( c1 < count1 )
		drums[6] = 1.0;
	
	glPushMatrix();

	
	#define DRAWHEPT glBegin(GL_POLYGON);          \
		glVertex2f(  0.000000000f,  1.000000000f ); \
		glVertex2f(  0.781831482f,  0.623489802f ); \
		glVertex2f(  0.974927912f, -0.222520934f ); \
		glVertex2f(  0.433883739f, -0.900968868f ); \
		glVertex2f( -0.433883739f, -0.900968868f ); \
		glVertex2f( -0.974927912f, -0.222520934f ); \
		glVertex2f( -0.781831482f,  0.623489802f ); \
	glEnd();
	
	float o = 0.3;
	float l = 0.7;

	glTranslatef( 0.93, 0.65, 0.0 );
	glScalef( 0.03, 0.05, 0.1 ); 

	glColor4f( l,o,o, drums[0] );	
	DRAWHEPT; // kick

	glColor4f( l,l,l, drums[1] );	
	glTranslatef( -3, 2, 0 );
	DRAWHEPT; // snare

	glColor4f( o,l,o, drums[4] );
	glTranslatef(  4, 0, 0 );
	DRAWHEPT; // ride

	glColor4f( o,l,l, drums[5] );
	glTranslatef(  -1, 2, 0 );
	DRAWHEPT; // crash

	glColor4f( l,l,o, drums[3] );
	glTranslatef(  -2, 0, 0 );
	DRAWHEPT; // hat

	glColor4f( l,o,l, drums[2] );
	glTranslatef(  1, -2, 0 );
	DRAWHEPT; // tom

	glColor4f( o,o,l, drums[6] );
	glTranslatef(  -1, -2, 0 );
	DRAWHEPT; // metronome
		
	glPopMatrix();
	
	//drums[6] -= 0.1;
	for(int i=0; i < 7; i++)
	{
		if( drums[i] > 0.2 ) drums[i] -= 0.2;
		else drums[i] = 0.1;
	}
	count1 = c1;
}

