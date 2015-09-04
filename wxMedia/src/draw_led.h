/*
 *  draw_led.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-07-22.
 *  Copyright 2010. All rights reserved.
 *
 */

void drawLEDchar(char c, float r, float g, float b, float alpha){
	#define LEDRED   glColor4f( r, g, b, alpha )
	#define LEDBLACK glColor4f( 0, 0, 0, alpha )
	#define LEDIF(x) if( e[x] ) LEDRED; else LEDBLACK

	int  ee[][8] = 
	{
	{ 0,0,0,0,0,0,0,0 }, // 
	{ 0,0,0,0,1,1,0,0 }, //!
	{ 0,1,0,0,0,1,0,0 }, //"
	{ 0,1,1,0,1,1,0,0 }, //#
	{ 1,0,1,1,0,1,1,1 }, //$
	{ 0,0,1,0,0,1,0,0 }, //%
	{ 1,1,0,1,1,1,1,0 }, //&
	{ 1,1,0,0,0,0,0,0 }, //'
	{ 1,0,0,1,1,1,0,0 }, //(
	{ 1,1,1,1,0,0,0,0 }, //)
	{ 1,1,0,0,0,1,1,0 }, //*
	{ 0,1,1,0,0,0,1,0 }, //+
	{ 0,0,1,1,0,0,0,0 }, //,
	{ 0,0,0,0,0,0,1,0 }, //-
	{ 0,0,0,1,0,0,0,0 }, //.
	{ 0,1,0,0,1,0,0,0 }, ///
	{ 1,1,1,1,1,1,0,0 }, //0
	{ 0,1,1,0,0,0,0,0 }, //1
	{ 1,1,0,1,1,0,1,0 }, //2
	{ 1,1,1,1,0,0,1,0 }, //3
	{ 0,1,1,0,0,1,1,0 }, //4
	{ 1,0,1,1,0,1,1,0 }, //5
	{ 1,0,1,1,1,1,1,0 }, //6
	{ 1,1,1,0,0,0,0,0 }, //7
	{ 1,1,1,1,1,1,1,0 }, //8
	{ 1,1,1,1,0,1,1,0 }, //9
	{ 1,0,0,1,0,0,0,0 }, //:
	{ 1,0,1,1,0,0,0,0 }, //;
	{ 1,0,0,0,0,1,1,0 }, //<
	{ 0,0,0,1,0,0,1,0 }, //=
	{ 1,1,0,0,0,0,1,0 }, //>
	{ 1,1,0,0,1,0,1,0 }, //?
	{ 1,1,1,1,1,0,1,0 }, //@
	{ 1,1,1,0,1,1,1,0 }, //A
	{ 0,0,1,1,1,1,1,0 }, //b
	{ 1,0,0,1,1,1,0,0 }, //C
	{ 0,1,1,1,1,0,1,0 }, //d
	{ 1,0,0,1,1,1,1,0 }, //E
	{ 1,0,0,0,1,1,1,0 }, //F
	{ 1,0,1,1,1,1,0,0 }, //G
	{ 0,0,1,0,1,1,1,0 }, //h
	{ 0,0,0,0,0,0,0,1 }, //I
	{ 1,1,1,1,1,0,0,0 }, //J
	{ 0,1,0,0,1,1,1,1 }, //K
	{ 0,0,0,1,1,1,0,0 }, //L
	{ 0,0,1,0,1,0,1,1 }, //m
	{ 1,1,1,0,1,1,0,0 }, //n
	{ 0,0,1,1,1,0,1,0 }, //o
	{ 1,1,0,0,1,1,1,0 }, //P
	{ 1,1,0,0,0,1,1,1 }, //q
	{ 1,1,0,0,1,1,1,1 }, //r
	{ 1,0,1,1,0,1,1,0 }, //s
	{ 0,0,0,1,1,1,1,0 }, //t
	{ 0,0,0,1,1,0,0,1 }, //u
	{ 0,0,1,1,1,1,0,0 }, //V
	{ 0,0,1,1,1,0,0,1 }, //W
	{ 0,1,0,0,1,0,1,0 }, //X
	{ 0,1,1,1,0,1,1,0 }, //y
	{ 1,1,0,1,1,0,0,0 } //Z
	};
	
	int *e;
	
	if( c == '.' or c == ',' or c == ')' or c == '(' or c == '"' or c == '\'' or c == ':' or c == ';')
		glScalef( 0.4, 1.0, 1.0 );
	
	if( c >= ' ' and c <= 'Z') 
		e = ee[c - ' '];
	else
		e = ee[0];
	
	glBegin( GL_POLYGON );
	
		glColor4f( 0.0, 0.0, 0.0, alpha );
		glVertex3f( 0, 0, 0.001 );
		glVertex3f( 1, 0, 0.001 );
		glVertex3f( 1, 1, 0.001 );
		glVertex3f( 0, 1, 0.001 );
	
	glEnd();
	
	glScalef( 0.1, 0.1, 1.0 );	
	glBegin( GL_POLYGON );

		// top
		LEDIF(0);
		glVertex2f( 		2, 1.0
		); glVertex2f(		3, 0.5
		); glVertex2f(		7, 0.5
		); glVertex2f(		8, 1.0
		); glVertex2f(		7, 1.5
		); glVertex2f(		3, 1.5
		);

	glEnd(); glBegin( GL_POLYGON );

		// upper right
		LEDIF(1);
		glVertex2f( 		7, 2
		); glVertex2i(		7, 4
		); glVertex2i(		8, 5
		); glVertex2i(		9, 4
		); glVertex2i(		9, 2
		); glVertex2i(		8, 1
		);
		
	glEnd(); glBegin( GL_POLYGON );
		
		// lower right
		LEDIF(2);
		glVertex2f( 		7, 8
		); glVertex2i(		7, 6
		); glVertex2i(		8, 5
		); glVertex2i(		9, 6
		); glVertex2i(		9, 8
		); glVertex2i(		8, 9
		);

	glEnd(); glBegin( GL_POLYGON );
		
		// bottom
		LEDIF(3);
		glVertex2f(			2, 9.0
		); glVertex2f(		3, 8.5
		); glVertex2f(		7, 8.5
		); glVertex2f(		8, 9.0
		); glVertex2f(		7, 9.5
		); glVertex2f(		3, 9.5
		);

	glEnd(); glBegin( GL_POLYGON );
		
		// lower left
		LEDIF(4);
		glVertex2f( 		1, 8
		); glVertex2i(		1, 6
		); glVertex2i(		2, 5
		); glVertex2i(		3, 6
		); glVertex2i(		3, 8
		); glVertex2i(		2, 9
		);

	glEnd(); glBegin( GL_POLYGON );
		
		// upper left
		LEDIF(5);
		glVertex2f( 		1, 2
		); glVertex2i(		1, 4
		); glVertex2i(		2, 5
		); glVertex2i(		3, 4
		); glVertex2i(		3, 2
		); glVertex2i(		2, 1
		);
		
	glEnd(); glBegin(GL_POLYGON);

		// middle
		LEDIF(6);
		glVertex2f( 		2, 5.0
		); glVertex2f(		3, 4.5
		); glVertex2f(		7, 4.5
		); glVertex2f(		8, 5.0
		); glVertex2f(		7, 5.5
		); glVertex2f(		3, 5.5
		);

	glEnd(); glBegin(GL_POLYGON);
		// middle
		LEDIF(7);
		glVertex2f( 		4, 5.5
		); glVertex2f(		5, 5.5
		); glVertex2f(		6, 5.5
		); glVertex2f(		6, 8.5
		); glVertex2f(		5, 8.5
		); glVertex2f(		4, 8.5
		);
	glEnd();

	glScalef( 10.0, 10.0, 1.0 );		
	glTranslatef( 1.0, 0.0, 0.0 );

	if( c == '.' or c == ',' or c == ')' or c == '(' or c == '"' or c == '\'' or c == ':' or c == ';')
		glScalef( 2.5, 1.0, 1.0 );

}

void Canvas3D::drawLED( char *str, float r, float g, float b, float a )
{
	for(int i=0; i < strlen(str); i++)
	{
		drawLEDchar(str[i], r,g,b,a);
	}	
}

void Canvas3D::drawLED()
{
//	GLuint index = glGenLists(60);
//	GLubyte lists[256];
	
	ledAlpha -= 0.01;
	
	glPushMatrix();
	glTranslatef( 0.0, 0.82, 0.2f );
	glScalef( 0.02, 0.04, 1.0 );
	
	drawLED( str, 0.f,1.f,0.f, ledAlpha );
	glPopMatrix();
}


void drawNumber( char c ){
	glPushMatrix();
	
	glEnable( GL_POLYGON_OFFSET_FILL );
	
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
	
	glBegin(GL_LINE_STRIP);

	switch( c ){
	case '0':
		glVertex2f( 0.1, 0.1 );
		glVertex2f( 0.9, 0.1 );
		glVertex2f( 0.9, 0.9 );
		glVertex2f( 0.1, 0.9 );
		glVertex2f( 0.1, 0.1 );
		break;
	case '1':
		glVertex2f( 0.5, 0.1 );
		glVertex2f( 0.5, 0.9 );
		break;
	case '2':
		glVertex2f( 0.1, 0.1 );
		glVertex2f( 0.9, 0.1 );
		glVertex2f( 0.9, 0.5 );
		glVertex2f( 0.1, 0.5 );
		glVertex2f( 0.1, 0.9 );
		glVertex2f( 0.9, 0.9 );
		break;
	case '3':
		glVertex2f( 0.1, 0.1 );
		glVertex2f( 0.9, 0.1 );
		glVertex2f( 0.9, 0.5 );
		glVertex2f( 0.1, 0.5 );
		glVertex2f( 0.9, 0.5 );
		glVertex2f( 0.9, 0.9 );
		glVertex2f( 0.1, 0.9 );
		break;
	case '4':
		glVertex2f( 0.1, 0.1 );
		glVertex2f( 0.1, 0.5 );
		glVertex2f( 0.9, 0.5 );
		glVertex2f( 0.9, 0.1 );
		glVertex2f( 0.9, 0.9 );
		break;
	case '5':
		glVertex2f( 0.9, 0.1 );
		glVertex2f( 0.1, 0.1 );
		glVertex2f( 0.1, 0.5 );
		glVertex2f( 0.9, 0.5 );
		glVertex2f( 0.9, 0.9 );
		glVertex2f( 0.1, 0.9 );
		break;
	case '6':
		glVertex2f( 0.9, 0.1 );
		glVertex2f( 0.1, 0.1 );
		glVertex2f( 0.1, 0.9 );
		glVertex2f( 0.9, 0.9 );
		glVertex2f( 0.9, 0.5 );
		glVertex2f( 0.1, 0.5 );
		break;
	case '7':
		glVertex2f( 0.1, 0.1 );
		glVertex2f( 0.9, 0.1 );
		glVertex2f( 0.5, 0.5 );
		glVertex2f( 0.3, 0.5 );
		glVertex2f( 0.7, 0.5 );
		glVertex2f( 0.5, 0.5 );
		glVertex2f( 0.1, 0.9 );
		break;
	case '8':
		glVertex2f( 0.9, 0.1 );
		glVertex2f( 0.1, 0.1 );
		glVertex2f( 0.1, 0.4 );
		glVertex2f( 0.9, 0.6 );
		glVertex2f( 0.9, 0.9 );
		glVertex2f( 0.1, 0.9 );
		glVertex2f( 0.1, 0.6 );
		glVertex2f( 0.9, 0.4 );
		glVertex2f( 0.9, 0.1 );
		break;
	case '9':
		glVertex2f( 0.9, 0.5 );
		glVertex2f( 0.1, 0.5 );
		glVertex2f( 0.1, 0.1 );
		glVertex2f( 0.9, 0.1 );
		glVertex2f( 0.9, 0.9 );
		glVertex2f( 0.1, 0.9 );
		break;
	case '.':
		glVertex2f( 0.4, 0.8 );
		glVertex2f( 0.6, 0.8 );
		glVertex2f( 0.6, 0.9 );
		glVertex2f( 0.4, 0.9 );
		glVertex2f( 0.4, 0.8 );
		break;
	
	}
	glEnd();
	//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	glPopMatrix();	
}

