/*
 *  draw_signal.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-07-22.
 *  Copyright 2010. All rights reserved.
 *
 */


void Canvas3D::SetScopeBuffers(float *h, float *l)
{
	scope_maxv = h;
	scope_minv = l;
}

void Canvas3D::drawScope()
{
	glPushMatrix();
	glTranslatef( 0, 0, 0.9 );
	
	float alpha = 0.75;
	float height = 0.5, width = 349.0,
		sleft = 0.1, srigh = sleft + 0.8,
		stop = 0.0, sbot = stop + height,
		center_y = stop + height / 2.0,
		dx = (1.0 / width) * (srigh - sleft);
		;
	
	glColor4f( 0.1, 0.2, 0.1, alpha );
	glBegin(GL_POLYGON);
		glNormal3f( 0, 0, -1 );
		glVertex2f( sleft, stop );
		glVertex2f( sleft, sbot );
		glVertex2f( srigh, sbot );
		glVertex2f( srigh, stop );
	glEnd();
	
	glColor4f(0.0, 1.0 ,0.0, alpha );
	
	int i;
	for(i=0; i < width; i++){
		glBegin(GL_QUADS);
			glNormal3f( 0.0, 0.0, 1.0 );

			glVertex2f(     i * dx + sleft, center_y - height * scope_maxv[i] );
			glVertex2f( (i+1) * dx + sleft, center_y - height * scope_maxv[i+1] );
			glVertex2f( (i+1) * dx + sleft, center_y );
			glVertex2f(     i * dx + sleft, center_y );

			glVertex2f(     i * dx + sleft, center_y - height * scope_minv[i] );
			glVertex2f( (i+1) * dx + sleft, center_y - height * scope_minv[i+1] );
			glVertex2f( (i+1) * dx + sleft, center_y );
			glVertex2f(     i * dx + sleft, center_y );
		glEnd();
	}
	
	glPopMatrix();
}


void Canvas3D::drawLissajous()
{


	float x, y, z, A, B, a, b, t, N, d, dt;
	float red, grn, blu;
	int hue;
	dt= 2.*M_PI*(10./360.);
	A = 1.;
	B = 1.;
	N = 1.;
	
	//d = M_PI * (N - 1.) / (2. * N);
	//d = 0.5 * M_PI; 
	d = lissajous_d;
	
	a = chord_ratio;
	b = 1.;
	t = 0.f;
	
	lissajous_d += dt * 0.1f;
	lissajous_rot += 1;

	glPushMatrix();
	
	//glTranslatef( 0.85, 0.7, 0.0 );
	glScalef(0.2 - (0.1 * pitch), 0.2 + (0.1 * pitch), 0.2 - (0.1 * pitch));
	glRotatef( 15, -1, 0, 0 );
	glRotatef( lissajous_rot*3/4, 0, 0, -1 );
	
	
	glBegin(GL_TRIANGLE_FAN);
	//glBegin(GL_POLYGON);
	
		glVertex3f(0,0,0);

		for(t = 0.0; t < 40. * M_PI; t+= dt)
		{
			x = sinf( a * t + d );
			y = sinf( b * t );
			z = sinf( a * t );

			hue = 180*(t/M_PI) ; //180 * (x+1);
			hue = (hue + 240) % 360;

			if( hue < 60 ) red = 1.0f;
			else if( hue < 120 ) red = 1.0f - ((hue-60) / 60.f);
			else if( hue < 240 ) red = 0.0f;
			else if( hue < 300 ) red = ((hue-240) / 60.f);
			else red = 1.0f;
			
			if( hue < 60 ) grn = (hue / 60.f);
			else if( hue < 180 ) grn = 1.0;
			else if( hue < 240 ) grn = 1.0f - ((hue - 180) / 60.f);
			else grn = 0.f;
			
			if( hue < 120 ) blu = 0.0;
			else if( hue < 180 ) blu = (hue - 120) / 60.f;
			else if( hue < 300 ) blu = 1.0f;
			else blu = 1.0f - (hue - 300) / 60.f;

			glColor4f( red, grn,blu, 0.3f );
			glVertex3f( x, y, z );

		}
	glEnd();
	
	glPopMatrix();

}

