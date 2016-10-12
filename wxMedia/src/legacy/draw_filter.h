/*
 *  draw_filter.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-07-22.
 *  Copyright 2010. All rights reserved.
 *
 */
 
void Canvas3D::instColor(int inst, float alph)
{
	switch( inst )
	{
		case 0: glColor4f(  1,  0,  0, alph ); break;
		case 1: glColor4f( .8, .8, .8, alph ); break;
		case 2: glColor4f(  1,  1,  0, alph ); break;
		case 3: glColor4f(  0,  1,  0, alph ); break;
		case 4: glColor4f( .2, .3,  1, alph ); break;
		case 5: glColor4f(  0,  1,  1, alph ); break;
		case 6: glColor4f(  1,  0,  1, alph ); break;
		case 7: glColor4f( .6, .5, .5, .5   ); break;
	}
}


void Canvas3D::drawEnvelope()
{
	glPushMatrix();
	glTranslatef( -0.6, -0.55, 0.0 );
	glScalef( 0.2, 0.4, 0.2 );
	
	for(int i=0; i < 8; i++)
	{
		if( active[i] )
		{
			float alpha;
			float
				v =  fmaxf( 0.1, ( 40. + 20. * log10( *(ampV[i]) ) ) * 0.025 ) ,
				a =  *(ampA[i]) ,
				d =  *(ampD[i]) + a ,
				s =  fmaxf( 0.0, ( 40. + 20. * log10( *(ampS[i]) * *(ampV[i]) ) ) * 0.025 ) ,
				r =  *(ampR[i]) + a + d ;

			alpha = 0.4;
			if( selinst == i ) alpha = 0.6;
		
			glBegin(GL_TRIANGLES);
				instColor(i, alpha * 1.0 );
				//glColor4f(1,0,0,alpha);
				glVertex2f( 0, 0 );
				glVertex2f( a, 0 );
				glVertex2f( a, v );
				
				instColor(i, alpha * 0.9 );
				//glColor4f(1,1,0,alpha);
				glVertex2f( a, v );
				glVertex2f( d, s );
				glVertex2f( a, 0 );
				
				instColor(i, alpha * 1.0 );
				//glColor4f(0,1,0,alpha);
				glVertex2f( a, 0 );
				glVertex2f( d, 0 );
				glVertex2f( d, s );

				instColor(i, alpha * 0.9 );
				//glColor4f(0,1,1,alpha);		
				glVertex2f( d, s );
				glVertex2f( d, 0 );
				glVertex2f( r, 0 );
			glEnd();
			
			glTranslatef( -0.02, -0.02, 0.0 );
		}
	}
	
	
	glPopMatrix();
}


void Canvas3D::drawFilter()
{
	int mindb = -36, maxdb = 24;

	glPushMatrix();
	glTranslatef( -0.02, -0.35, 0.0 );
	//glRotatef( 50.f, 0.f, 1.f, 0.f );
	//glScalef( 0.5, 0.5, 1.0 );
	glScalef( 1.0f/128.f, 1.f/100.f, 1 );
	

	glTranslatef( 0.0, 0.0, 0.9 );
	glBegin(GL_LINES);
		glColor4f( 0.4 , 0.4 , 0.4 , 0.4 );
		glVertex2f(   0, 0 );
		glVertex2f( 128, 0 );

		glColor4f( 0.4 , 0.4 , 0.4 , 0.8 );
		glVertex2f(  32, maxdb );     // 100
		glVertex2f(  32, mindb );
		glVertex2f(  72, maxdb );     // 1k
		glVertex2f(  72, mindb );
		glVertex2f( 112, maxdb );     // 10k
		glVertex2f( 112, mindb );
		
		glColor4f( 0.3 , 0.3 , 0.3 , 0.4 );
		glVertex2f(  20, maxdb ); // 50
		glVertex2f(  20, mindb );

		glVertex2f(  44, maxdb ); // 200
		glVertex2f(  44, mindb );

		glVertex2f(  57, maxdb ); // 420
		glVertex2f(  57, mindb );

		glColor4f( 0.5 , 0.0 , 0.0 , 0.3 );
		glVertex2f(  67, maxdb ); // 750
		glVertex2f(  67, mindb );

		glVertex2f(  79, maxdb ); // 1500
		glVertex2f(  79, mindb );

		glVertex2f(  91, maxdb ); // 3k
		glVertex2f(  91, mindb );

		glVertex2f( 100, maxdb ); // 5k
		glVertex2f( 100, mindb );

		glVertex2f( 103, maxdb ); // 6k
		glVertex2f( 103, mindb );

		glVertex2f( 106, maxdb ); // 7k
		glVertex2f( 106, mindb );

		glVertex2f( 115, maxdb ); // 12k
		glVertex2f( 115, mindb );

		glVertex2f( 123, maxdb ); // 20k
		glVertex2f( 123, mindb );

		glColor4f( 0.0 , 0.0 , 0.5 , 0.2 );
		for(int i=0; i < 128; i+= 12)
		{
			glVertex2f(  i, maxdb ); 
			glVertex2f(  i, mindb );
		}


	glEnd();
	glTranslatef( 0.0, 0.0, -0.8 );
	

	glBegin( GL_QUADS );

	glNormal3f( 0, 0, -1 );	

	for(int inst = 7; inst >= 0; inst--)
	{
		if( active[inst] )
		{
			int x = 0; //-inst;
			int y = inst * 0.2f;
			int z = inst / 8.0f;
			float alph = 0.4f;
			if( selinst == inst ) alph = 0.6;
			
			instColor( inst, alph );
			
			for(int i=0; i < 127; i++)
			{
				float 
					f1 = filt[inst][i],
					f2 = filt[inst][i+1];
				glVertex3f(   i + x , mindb + y , z );
				glVertex3f(   i + x ,  f1 + y , z );
				glVertex3f( i+1 + x ,  f2 + y , z );
				glVertex3f( i+1 + x , mindb + y , z );
			}
		}
	}
	//glVertex2f( 128, mindb );
	glEnd();
	
	
	glPopMatrix();
}

void Canvas3D::updateFilter(int inst, int i, fl y)
{
	filt[inst][i] = y;
	selinst = inst;
}
