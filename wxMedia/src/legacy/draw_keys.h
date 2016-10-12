/*
 *  draw_keys.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-07-22.
 *  Copyright 2010. All rights reserved.
 *
 */


void Canvas3D::SetPitch(float f)
{
	pitch = f;
}
void Canvas3D::SetFader(float f)
{
	vol = f;
}
void Canvas3D::SetMod(float f)
{
	mod = f;
}
void Canvas3D::SetSustain(float f)
{
	// nop
}

void Canvas3D::keyOn(int k)
{
	keyon[k] = true;
}

void Canvas3D::keyOff(int k)
{
}

void Canvas3D::drawPiano()
{
	
	float phei = 0.12;
	float alpha = 1.0;
	float
		aleft = 0.0, arigh = 0.07,
		atop = 0.0, abot = 0.9,
		ktop = 0.3, kmid = 0.6, kbot = 1.0,
		kleft = arigh, krigh = 1.0;
	float n, n1,
		az  =  0.1,
		bz  =  0.0,
		wz  = -0.1,
		kz0 = -0.1,
		kz1 =  0.0,
		nz  = -0.2,
		cz  = -0.3,
		spz = -0.3,
		
		nw = (krigh-kleft) / 78.0;


	glPushMatrix();
	
	//glOrtho( 0.1, 0.9, 1.0, 0.0, -1.0, 1.0 );
	
	glTranslatef( 0.0, 1.0 - phei, 0.0 );
	//glRotatef( -rot, 1, 1, 0 );
	
	glScalef( 1.0, phei, phei );
	

	glBegin(GL_QUADS);
		glNormal3f( 0, 0, -1 );
		
		// back panel
		glColor4f( 0.6, 0.6, 0.6, alpha );
		glVertex3f( aleft, atop, bz );
		glVertex3f( aleft, kbot, bz );
		glVertex3f( krigh, kbot, bz );
		glVertex3f( krigh, atop, bz );

		glVertex3f( aleft, atop, az );
		glVertex3f( aleft, kbot, az );
		glVertex3f( krigh, kbot, az );
		glVertex3f( krigh, atop, az );

		glVertex3f( aleft, atop, nz );
		glVertex3f( aleft, abot, nz );
		glVertex3f( aleft, kbot, az );
		glVertex3f( aleft, atop, az );

		glVertex3f( arigh, atop, bz );
		glVertex3f( arigh, kbot, bz );
		glVertex3f( arigh, kbot, az );
		glVertex3f( arigh, atop, az );

		glVertex3f( aleft, atop, bz );
		glVertex3f( aleft, atop, az );
		glVertex3f( krigh, atop, az );
		glVertex3f( krigh, atop, bz );

		glVertex3f( aleft, kbot, bz );
		glVertex3f( aleft, kbot, az );
		glVertex3f( krigh, kbot, az );
		glVertex3f( krigh, kbot, bz );

		
		glColor4f( 0.5, 0.5, 0.5, alpha );
		glVertex3f( aleft, atop, bz );
		glVertex3f( aleft, atop, cz );
		glVertex3f( krigh, atop, cz );
		glVertex3f( krigh, atop, bz );

		glVertex3f( aleft, atop, cz );
		glVertex3f( aleft, ktop, cz );
		glVertex3f( krigh, ktop, cz );
		glVertex3f( krigh, atop, cz );

		glVertex3f( kleft, ktop, bz );
		glVertex3f( kleft, ktop, cz );
		glVertex3f( krigh, ktop, cz );
		glVertex3f( krigh, ktop, bz );

		glVertex3f( aleft, kbot, az );
		glVertex3f( aleft, abot, nz );
		glVertex3f( arigh, abot, nz );
		glVertex3f( arigh, kbot, az );
		
		
		// split points
		glColor4f( 1.0, 0.0, 0.0, alpha );
		glVertex3f( kleft + ((0)/88.0) * (krigh - kleft), ktop - nw,      spz );
		glVertex3f( kleft + ((0)/88.0) * (krigh - kleft), ktop - nw - nw, spz );
		glVertex3f( kleft + ((loSplit-21)/88.0) * (krigh - kleft), ktop - nw - nw, spz );
		glVertex3f( kleft + ((loSplit-21)/88.0) * (krigh - kleft), ktop - nw,      spz );

		glColor4f( 0.0, 1.0, 0.0, alpha );
		glVertex3f( kleft + ((loSplit-21)/88.0) * (krigh - kleft), ktop - nw,      spz );
		glVertex3f( kleft + ((loSplit-21)/88.0) * (krigh - kleft), ktop - nw - nw, spz );
		glVertex3f( kleft + ((hiSplit-21)/88.0) * (krigh - kleft), ktop - nw - nw, spz );
		glVertex3f( kleft + ((hiSplit-21)/88.0) * (krigh - kleft), ktop - nw,      spz );
		
		glColor4f( 0.0, 0.0, 1.0, alpha );
		glVertex3f( kleft + ((hiSplit-21)/88.0) * (krigh - kleft), ktop - nw,      spz );
		glVertex3f( kleft + ((hiSplit-21)/88.0) * (krigh - kleft), ktop - nw - nw, spz );
		glVertex3f( kleft + ((87.0)/88.0) * (krigh - kleft), ktop - nw - nw, spz );
		glVertex3f( kleft + ((87.0)/88.0) * (krigh - kleft), ktop - nw,      spz );

		/*
		// white key background
		glColor4f( 1.0, 1.0, 1.0, alpha );
		glVertex3f( kleft, ktop, wz );
		glVertex3f( kleft, kbot, wz );
		glVertex3f( krigh, kbot, wz );
		glVertex3f( krigh, ktop, wz );
		*/
	
	//glEnd();
		
	// coordinates for pitch, mod, volume
	float 
		pw = nw, ph = pw / phei,
		pleft = aleft + pw, prigh = pleft + pw,
		ptop = kmid, pbot = abot - ph,
		ppos = ptop + (pbot - ptop) * (1-pitch)/2 - ph / 2
		;
	float
		mw = pw, mh = mw / phei,
		mleft = prigh + mw, mrigh = mleft + mw,
		mtop = ptop, mbot = pbot,
		mpos = mtop + (mbot - mtop) * (1-mod) - mh / 2
		;
	float
		vw = mw / 2, vh = vw / phei,
		vtop = ptop - (mbot - mtop) - mh, vbot = vtop + (pbot - ptop),
		vleft = pleft + vw / 2, vrigh = prigh - vw / 2,
		vpos = vtop + (vbot - vtop) * (1-vol) - ph / 2
		;
		
	//float ch = vh;
		

	//pitch wheel
	//glBegin(GL_QUADS);
		glNormal3f( 0, 0, -1 );

		glColor4f( 0.3, 0.3, 0.3, alpha);
		glVertex3f( pleft, ptop, nz );
		glVertex3f( pleft, pbot, nz );
		glVertex3f( prigh, pbot, nz );
		glVertex3f( prigh, ptop, nz );

		glColor4f( 0.0, 0.0, 0.0, alpha );
		glVertex3f( pleft, ppos, nz );
		glVertex3f( pleft, ppos + ph, nz );
		glVertex3f( prigh, ppos + ph, nz );
		glVertex3f( prigh, ppos, nz );

		//mod wheel
		glColor4f( 0.3, 0.3, 0.3, alpha);
		glVertex3f( mleft, mtop, nz );
		glVertex3f( mleft, mbot, nz );
		glVertex3f( mrigh, mbot, nz );
		glVertex3f( mrigh, mtop, nz );

		glColor4f( 0.0, 0.0, 0.0, alpha );
		glVertex3f( mleft, mpos, nz );
		glVertex3f( mleft, mpos + mh, nz );
		glVertex3f( mrigh, mpos + mh, nz );
		glVertex3f( mrigh, mpos, nz );

		//volume fader
		glColor4f( 0.3, 0.3, 0.3, alpha);
		glVertex3f( vleft, vtop, nz );
		glVertex3f( vleft, vbot, nz );
		glVertex3f( vrigh, vbot, nz );
		glVertex3f( vrigh, vtop, nz );

		glColor4f( 0.0, 0.0, 0.0, alpha );
		glVertex3f( pleft, vpos, nz );
		glVertex3f( pleft, vpos + vh, nz );
		glVertex3f( prigh, vpos + vh, nz );
		glVertex3f( prigh, vpos, nz );


	//glEnd();

	float kloc[13] = { 0, 1.00, 2.00, 2.50, 3.50, 4.50, 5.50, 6.50, 7.00, 8.00, 8.50, 9.50, 10.50 };
	float wloc[13] = { 0, 1.45, 1.55, 2.95, 3.05, 4.50, 5.95, 6.05, 7.45, 7.55, 8.95, 9.05, 10.45 };
	float sep = 0.05;
	
	//kmid += pitch;

	#define WHITEKEY(q,p)\
		if( keyon[ 24 + o * 12 + q ] ) \
			glColor4f( 1.0, 0.0, 0.0, alpha ); \
		else \
			glColor4f( 1.0, 1.0, 1.0, alpha ); \
		\
		glNormal3f( 0, 0, -1 );\
		glVertex3f( n + nw * (kloc[q] + sep), ktop, kz0);\
		glVertex3f( n + nw * (kloc[p] - sep), ktop, kz0);\
		glVertex3f( n + nw * (kloc[p] - sep), kmid, kz0);\
		glVertex3f( n + nw * (kloc[q] + sep), kmid, kz0);\
		glNormal3f( 0, 0, -1 );\
		glVertex3f( n + nw * (wloc[q] + sep), kmid, kz0);\
		glVertex3f( n + nw * (wloc[p] - sep), kmid, kz0);\
		glVertex3f( n + nw * (wloc[p] - sep), kbot, kz0);\
		glVertex3f( n + nw * (wloc[q] + sep), kbot, kz0);\
		\
		glNormal3f( 0, 1, 0 );\
		glVertex3f( n + nw * (wloc[p] - sep), kbot, kz1);\
		glVertex3f( n + nw * (wloc[q] + sep), kbot, kz1);\
		glVertex3f( n + nw * (wloc[q] + sep), kbot, kz0);\
		glVertex3f( n + nw * (wloc[p] - sep), kbot, kz0);\
		\
		glNormal3f( -1, 0, 0 );\
		glVertex3f( n + nw * (wloc[p] - sep), ktop, kz0);\
		glVertex3f( n + nw * (wloc[p] - sep), ktop, kz1);\
		glVertex3f( n + nw * (wloc[p] - sep), kbot, kz1);\
		glVertex3f( n + nw * (wloc[p] - sep), kbot, kz0);\
		\
		glNormal3f( 1, 0, 0 );\
		glVertex3f( n + nw * (wloc[q] + sep), ktop, kz0);\
		glVertex3f( n + nw * (wloc[q] + sep), ktop, kz1);\
		glVertex3f( n + nw * (wloc[q] + sep), kbot, kz1);\
		glVertex3f( n + nw * (wloc[q] + sep), kbot, kz0);

	#define BLACKKEY(q,p) \
		if( keyon[ 24 + o * 12 + q ] ) \
			glColor4f( 1.0, 0.0, 0.0, alpha ); \
		else \
			glColor4f( 0.1, 0.1, 0.1, alpha ); \
		\
		glNormal3f( 0, 0, -1 );\
		glVertex3f( n + nw * kloc[q], ktop, nz);\
		glVertex3f( n + nw * kloc[p], ktop, nz);\
		glVertex3f( n + nw * kloc[p], kmid, nz);\
		glVertex3f( n + nw * kloc[q], kmid, nz);\
		\
		glNormal3f( -1, 0, 0 );\
		glVertex3f( n + nw * kloc[q], ktop, nz);\
		glVertex3f( n + nw * kloc[q], ktop, wz);\
		glVertex3f( n + nw * kloc[q], kmid, wz);\
		glVertex3f( n + nw * kloc[q], kmid, nz);\
		\
		glNormal3f( 1, 0, 0 );\
		glVertex3f( n + nw * kloc[p], ktop, nz);\
		glVertex3f( n + nw * kloc[p], ktop, wz);\
		glVertex3f( n + nw * kloc[p], kmid, wz);\
		glVertex3f( n + nw * kloc[p], kmid, nz);\
		\
		glNormal3f( 0, 1, 0 );\
		glVertex3f( n + nw * kloc[q], kmid, nz);\
		glVertex3f( n + nw * kloc[p], kmid, nz);\
		glVertex3f( n + nw * kloc[p], kmid, wz);\
		glVertex3f( n + nw * kloc[q], kmid, wz);


	int o;
	
	o = -1;
	
	//draw first three keys
	n = kleft;
	//glBegin(GL_QUADS);
	
		glColor4f( 1.0, 1.0, 1.0, alpha );
		WHITEKEY(0,1)
		n -= nw * 1.5;
		WHITEKEY(4,5)
		n += nw * 1.5;
	
		glColor4f( 0.1, 0.1, 0.1, alpha );
		BLACKKEY(1,2)
		
	//glEnd();
	
	n += nw * 3.0;
	n1 = n;
	n = n1;

	//glBegin(GL_QUADS);
	//	glNormal3f( 0,0,-1 );

	//int q,p;
	for(o=0; o < 7; o++, n += nw * 10.5){
	
		WHITEKEY(0,1)
		WHITEKEY(2,3)
		WHITEKEY(4,5)
		WHITEKEY(5,6)
		WHITEKEY(7,8)
		WHITEKEY(9,10)
		WHITEKEY(11,12)

		BLACKKEY(1,2)
		BLACKKEY(3,4)
		BLACKKEY(6,7)
		BLACKKEY(8,9)
		BLACKKEY(10,11)

	}
	WHITEKEY(0,1)

	glEnd();
	
	glPopMatrix();
}
