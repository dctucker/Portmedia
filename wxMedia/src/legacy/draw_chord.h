/*
 *  draw_chord.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-07-22.
 *  Copyright 2010. All rights reserved.
 *
 */

void drawSharp()
{
	glBegin( GL_QUADS );
		glVertex2i( 1,10 );
		glVertex2i( 2,10 );
		glVertex2i( 2,2 );
		glVertex2i( 1,2 );
		
		glVertex2i( 3,10 );
		glVertex2i( 4,10 );
		glVertex2i( 4,2 );
		glVertex2i( 3,2 );
		
		glVertex2i( 0,7 );
		glVertex2i( 0,8 );
		glVertex2i( 5,9 );
		glVertex2i( 5,8 );
		
		glVertex2i( 0,4 );
		glVertex2i( 0,5 );
		glVertex2i( 5,6 );
		glVertex2i( 5,5 );
	glEnd();
	glTranslatef( 5, 0,0 );
}

void drawFlat()
{
	glBegin( GL_QUAD_STRIP );
		glVertex2f( 1,3 );
		glVertex2f( 0,2 );
		glVertex2i( 2,4 );
		glVertex2i( 3,4 );

		glVertex2i( 2,6 );
		glVertex2i( 3,6 );
		glVertex2i( 2,5 );
		glVertex2i( 0,5 );
	glEnd();
	glBegin( GL_QUADS );

		glVertex2i( 0,2 );
		glVertex2i( 0,10 );
		glVertex2i( 1,10 );
		glVertex2i( 1,3 );
		
	glEnd();
	glTranslatef( 4, 0,0 );
}

void drawNote( int note )
{
	switch( note )
	{
	case 0:  // C
	case 1:  // C#
	glBegin( GL_TRIANGLE_STRIP );
		glVertex2i( 6, 6 );
		glVertex2i( 4, 6 );
		glVertex2i( 6, 8 );
		glVertex2i( 4, 10 );
		glVertex2i( 2, 8 );
		glVertex2i( 2, 10);
		glVertex2i( 0, 8 );
		glVertex2i( 2, 2 );
		glVertex2i( 0, 2 );
		glVertex2i( 2, 0 );
		glVertex2i( 4, 2 );
		glVertex2i( 4, 0 );
		glVertex2i( 6, 2 );
		glVertex2i( 4, 4 );
		glVertex2i( 6, 4 );
	glEnd();
	break;
	case 2:  // D
	glBegin( GL_TRIANGLE_STRIP );
		glVertex2i( 0,0 );
		glVertex2i( 2,2 );
		glVertex2i( 0,10 );
		glVertex2i( 2,8  );
		glVertex2i( 4,10 );
		glVertex2i( 6,8 );
		glVertex2i( 4,2 );
		glVertex2i( 6,2 );
		glVertex2i( 4,0 );
		glVertex2i( 2,2 );
		glVertex2i( 0,0 );
	glEnd();
	break;
	case 3:  // Eb
	case 4:  // E
	glBegin( GL_QUADS );
		glVertex2i( 0,0 );
		glVertex2i( 0,10 );
		glVertex2i( 2,10 );
		glVertex2i( 2,0 );
		
		glVertex2i( 2,6 );
		glVertex2i( 2,4 );
		glVertex2i( 4,4 );
		glVertex2i( 4,6 );
		
		glVertex2i( 2,10 );
		glVertex2i( 2,8 );
		glVertex2i( 6,8 );
		glVertex2i( 6,10 );
		
		glVertex2i( 2,0 );
		glVertex2i( 2,2 );
		glVertex2i( 6,2 );
		glVertex2i( 6,0 );
	glEnd();
	break;
	case 5:  // F
	case 6:  // F#
	glBegin( GL_QUADS );
		glVertex2i( 0,0 );
		glVertex2i( 0,10 );
		glVertex2i( 2,10 );
		glVertex2i( 2,0 );
		
		glVertex2i( 2,6 );
		glVertex2i( 2,4 );
		glVertex2i( 4,4 );
		glVertex2i( 4,6 );
		
		glVertex2i( 2,10 );
		glVertex2i( 2,8 );
		glVertex2i( 6,8 );
		glVertex2i( 6,10 );
		
	glEnd();
	break;
	case 7:  // G
	glBegin( GL_QUAD_STRIP );
		glVertex2i( 6,10 );
		glVertex2i( 6,8 );
		glVertex2i( 2,10 );
		glVertex2i( 4,8 );
		
		glVertex2i( 0,8 );
		glVertex2i( 2,6 );
		glVertex2i( 0,2 );
		glVertex2i( 2,4 );
		glVertex2i( 2,0 );
		glVertex2i( 4,2 );
		
		glVertex2i( 4,0 );
		glVertex2i( 6,0 );
		glVertex2i( 4,6 );
		glVertex2i( 6,6 );
	glEnd();
	break;
	case 8:  // Ab
	case 9:  // A
	glBegin( GL_QUAD_STRIP );
		glVertex2i( 0,0 );
		glVertex2i( 2,0 );
		glVertex2i( 0,6 );
		glVertex2i( 2,4 );

		glVertex2i( 0,8 );
		glVertex2i( 2,10 );
		glVertex2i( 6,8 );
		glVertex2i( 4,10 );
		
		glVertex2i( 6,0 );
		glVertex2i( 4,0 );		
		glVertex2i( 6,6 );
		glVertex2i( 4,4 );
		glVertex2i( 2,6 );
		glVertex2i( 2,4 );
	glEnd();
	break;
	case 10: // Bb
	case 11: // B
	glBegin( GL_QUAD_STRIP );
		glVertex2i( 0,5 );
		glVertex2i( 0,10 );
		glVertex2i( 2,8 );
		glVertex2i( 2,10 );

		glVertex2i( 6,8 );
		glVertex2i( 4,10 );
		glVertex2i( 6,6 );
		glVertex2i( 4,5 );

		glVertex2i( 2,6 );
		glVertex2i( 2,5 );
		glVertex2i( 2,8 );
		glVertex2i( 0,5 );
	glEnd();
	glBegin( GL_QUAD_STRIP );
		glVertex2i( 0,5 );
		glVertex2i( 0,0 );
		glVertex2i( 2,2 );
		glVertex2i( 2,0 );

		glVertex2i( 6,2 );
		glVertex2i( 4,0 );
		glVertex2i( 6,4 );
		glVertex2i( 4,5 );

		glVertex2i( 2,4 );
		glVertex2i( 2,5 );
		glVertex2i( 2,2 );
		glVertex2i( 0,5 );
		
	glEnd();
	break;
	}
	
	glTranslatef( 6.5, 0,0 );
	
	switch( note )
	{
	case 3:
	case 8:
	case 10:
		// flat
		drawFlat();
	break;
	case 1:
	case 6:
		// sharp
		drawSharp();
	}

}

void drawChord( chord *c )
{
	glPushMatrix();
	
	glTranslatef( 0,0,0 );
	glScalef( 0.1, -0.1, 0.1 );
	glScalef( 0.1, 0.1, 0.1 );
	glTranslatef( 1, -11, 0 );

	glColor4f( 0,1,.4,0.9 );
	
	drawNote( c->root );

	switch( c->type )
	{
		case 0: // major
		break;
		case 1: // minor
			glBegin( GL_TRIANGLE_STRIP );
				glVertex2i( 0,0 );
				glVertex2i( 1,0 );
				glVertex2i( 0,4 );
				glVertex2i( 1,4 );
				glVertex2i( 1,2 );
				glVertex2i( 1,3 );
				glVertex2i( 2,3 );
				glVertex2i( 2,4 );
				glVertex2i( 3,3 );
				glVertex2i( 2,0 );
				glVertex2i( 3,0 );
				glVertex2i( 3,2 );
				glVertex2i( 3,3 );
				glVertex2i( 4,3 );
				glVertex2i( 4,4 );
				glVertex2i( 5,3 );
				glVertex2i( 4,0 );
				glVertex2i( 5,0 );
			glEnd();
			glTranslatef( 6.2, 0,0 );
		break;
		case 2: // diminished
			glBegin( GL_TRIANGLE_STRIP );
				glVertex2i( 1,6 );
				glVertex2i( 0,6 );
				glVertex2i( 1,8 );
				glVertex2i( 0,9 );
				glVertex2i( 3,9 );
				glVertex2i( 2,8 );
				glVertex2i( 3,6 );
				glVertex2i( 2,7 );
				glVertex2i( 1,6 );
				glVertex2i( 1,7 );
				//glVertex2i( 0,6 );
			glEnd();
			glTranslatef( 4, 0,0 );
			break;
		case 3: // augmented
			glBegin( GL_QUADS );
				glVertex2i( 0,5 );
				glVertex2i( 0,6 );
				glVertex2i( 3,6 );
				glVertex2i( 3,5 );
				glVertex2i( 1,4 );
				glVertex2i( 1,7 );
				glVertex2i( 2,7 );
				glVertex2i( 2,4 );
				//glVertex2i( 0,6 );
			glEnd();		
			glTranslatef( 4, 0,0 );
			break;
	}

	glScalef( 0.8, 0.8, 0.8 );
	glTranslatef( 0, 2, 0 );

	if( c->add2 == 7 ){
		// seven
		glBegin( GL_TRIANGLE_STRIP );
			glVertex2i( 0,10 );
			glVertex2i( 0, 9 );
			glVertex2i( 3,10 );
			glVertex2i( 3, 9 );
			glVertex2i( 1, 5 );
			glVertex2i( 3, 8 );
			glVertex2i( 2, 5 );
		glEnd();
		glTranslatef( 4, 0,0 );
	}
	
	if( c->add2 == 6 ) 
	{
		glBegin( GL_TRIANGLE_STRIP );
			glVertex2i( 3,10 );
			glVertex2i( 3,9  );
			glVertex2i( 2,10 );
			glVertex2i( 2,9  );
			glVertex2i( 1,10 );
			glVertex2i( 1,9  );
			glVertex2i( 0,9  );
			glVertex2i( 1,8  );
			glVertex2i( 0,7  );
			glVertex2i( 1,6  );
			glVertex2i( 0,6  );
			glVertex2i( 1,5  );
			glVertex2i( 2,6  );
			glVertex2i( 2,5  );
			glVertex2i( 3,6  );
			glVertex2i( 2,7  );
			glVertex2i( 3,7  );
			glVertex2i( 2,8  );
			glVertex2i( 1,7  );
			glVertex2i( 1,8  );
		glEnd();
		glTranslatef( 4, 0,0 );
	}
	
	if( c->root != c->bass )
	{
		glTranslatef( -2, -2, 0 );
		glBegin( GL_TRIANGLE_STRIP ); // slash
			glVertex2f( 0,0  );
			glVertex2f( 1,0  );
			glVertex2f( 2,10 );
			glVertex2f( 3,10 );
			
		glEnd();
		glTranslatef( 3, 0, 0);
		glScalef( 0.7, 0.7, 0.7 );
		drawNote( c->bass );
	}

	glPopMatrix();
}
