/*
 *  draw_led.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-07-22.
 *  Copyright 2010. All rights reserved.
 *
 */

const char led8_ee[] = {
	0x00, //	{ 0,0,0,0, 0,0,0,0 }, // 
	0x0c, //	{ 0,0,0,0, 1,1,0,0 }, //!
	0x44, //	{ 0,1,0,0, 0,1,0,0 }, //"
	0x6c, //	{ 0,1,1,0, 1,1,0,0 }, //#
	0xb7, //	{ 1,0,1,1, 0,1,1,1 }, //$
	0x24, //	{ 0,0,1,0, 0,1,0,0 }, //%
	0xde, //	{ 1,1,0,1, 1,1,1,0 }, //&
	0xc0, //	{ 1,1,0,0, 0,0,0,0 }, //'
	0x9c, //	{ 1,0,0,1, 1,1,0,0 }, //(
	0xf0, //	{ 1,1,1,1, 0,0,0,0 }, //)
	0xc6, //	{ 1,1,0,0, 0,1,1,0 }, //*
	0x62, //	{ 0,1,1,0, 0,0,1,0 }, //+
	0x30, //	{ 0,0,1,1, 0,0,0,0 }, //,
	0x02, //	{ 0,0,0,0, 0,0,1,0 }, //-
	0x10, //	{ 0,0,0,1, 0,0,0,0 }, //.
	0x48, //	{ 0,1,0,0, 1,0,0,0 }, ///
	0xfc, //	{ 1,1,1,1, 1,1,0,0 }, //0
	0x60, //	{ 0,1,1,0, 0,0,0,0 }, //1
	0xda, //	{ 1,1,0,1, 1,0,1,0 }, //2
	0xf2, //	{ 1,1,1,1, 0,0,1,0 }, //3
	0x66, //	{ 0,1,1,0, 0,1,1,0 }, //4
	0xb6, //	{ 1,0,1,1, 0,1,1,0 }, //5
	0xbe, //	{ 1,0,1,1, 1,1,1,0 }, //6
	0xe0, //	{ 1,1,1,0, 0,0,0,0 }, //7
	0xfe, //	{ 1,1,1,1, 1,1,1,0 }, //8
	0xf6, //	{ 1,1,1,1, 0,1,1,0 }, //9
	0x90, //	{ 1,0,0,1, 0,0,0,0 }, //:
	0xb0, //	{ 1,0,1,1, 0,0,0,0 }, //;
	0x86, //	{ 1,0,0,0, 0,1,1,0 }, //<
	0x12, //	{ 0,0,0,1, 0,0,1,0 }, //=
	0xc2, //	{ 1,1,0,0, 0,0,1,0 }, //>
	0xca, //	{ 1,1,0,0, 1,0,1,0 }, //?
	0xfa, //	{ 1,1,1,1, 1,0,1,0 }, //@
	0xee, //	{ 1,1,1,0, 1,1,1,0 }, //A
	0x3e, //	{ 0,0,1,1, 1,1,1,0 }, //b
	0x9c, //	{ 1,0,0,1, 1,1,0,0 }, //C
	0x7a, //	{ 0,1,1,1, 1,0,1,0 }, //d
	0x9e, //	{ 1,0,0,1, 1,1,1,0 }, //E
	0x8e, //	{ 1,0,0,0, 1,1,1,0 }, //F
	0xbc, //	{ 1,0,1,1, 1,1,0,0 }, //G
	0x2e, //	{ 0,0,1,0, 1,1,1,0 }, //h
	0x01, //	{ 0,0,0,0, 0,0,0,1 }, //I
	0xf8, //	{ 1,1,1,1, 1,0,0,0 }, //J
	0x4f, //	{ 0,1,0,0, 1,1,1,1 }, //K
	0x1c, //	{ 0,0,0,1, 1,1,0,0 }, //L
	0x2b, //	{ 0,0,1,0, 1,0,1,1 }, //m
	0x2a, //	{ 0,0,1,0, 1,0,1,0 }, //n
	0x3a, //	{ 0,0,1,1, 1,0,1,0 }, //o
	0xce, //	{ 1,1,0,0, 1,1,1,0 }, //P
	0xc7, //	{ 1,1,0,0, 0,1,1,1 }, //q
	0xcf, //	{ 1,1,0,0, 1,1,1,1 }, //r
	0xb6, //	{ 1,0,1,1, 0,1,1,0 }, //s
	0x1e, //	{ 0,0,0,1, 1,1,1,0 }, //t
	0x19, //	{ 0,0,0,1, 1,0,0,1 }, //u
	0x3c, //	{ 0,0,1,1, 1,1,0,0 }, //V
	0x39, //	{ 0,0,1,1, 1,0,0,1 }, //W
	0x4a, //	{ 0,1,0,0, 1,0,1,0 }, //X
	0x76, //	{ 0,1,1,1, 0,1,1,0 }, //y
	0xd8  //	{ 1,1,0,1, 1,0,0,0 } //Z
};

/*
								
			0		1			
		7		9		2		
			8		10			
			15		11			
			14		12			
		6		13		3		
			5		4			
								
*/

const u16 led16_ee[] = {
	0x0000, // { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 }, // 
	0x0022, // { 0,0,0,0, 0,0,0,0, 0,1,0,0, 0,1,0,0 }, //! 
	0x2100, // { 0,0,1,0, 0,0,0,1, 0,0,0,0, 0,0,0,0 }, //"
	0x3c55, // { 0,0,1,1, 1,1,0,0, 0,1,0,1, 0,1,0,1 }, //#
	0xdd55, // { 1,1,0,1, 1,1,0,1, 0,1,0,1, 0,1,0,1 }, //$
	0x1122, // { 0,0,0,1, 0,0,0,1, 0,0,1,0, 0,0,1,0 }, //%
	0xc2a9, // { 1,1,0,0, 0,0,1,0, 1,0,1,0, 1,0,0,1 }, //&
	0x0040, // { 0,0,0,0, 0,0,0,0, 0,1,0,0, 0,0,0,0 }, //'
	0x4844, // { 0,1,0,0, 1,0,0,0, 0,1,0,0, 0,1,0,0 }, //(
	0x8444, // { 1,0,0,0, 0,1,0,0, 0,1,0,0, 0,1,0,0 }, //)
	0x00ee, // { 0,0,0,0, 0,0,0,0, 1,1,1,0, 1,1,1,0 }, //*
};

void drawLEDchar(char c, float r, float g, float b, float alpha){
	#define LEDRED   glColor4f( r, g, b, alpha )
	#define LEDBLACK glColor4f( 0, 0, 0, alpha )
	#define LEDIF(x) if( e & x ) LEDRED; else LEDBLACK

	//const int *e;
	char e;
	
	if( c == '.' or c == ',' or c == ')' or c == '(' or c == '"' or c == '\'' or c == ':' or c == ';')
		glScalef( 0.4, 1.0, 1.0 );
	
	if( c >= ' ' and c <= 'Z') 
		e = led8_ee[c - ' '];
	else
		e = led8_ee[0];
	
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
		LEDIF(0x80);
		glVertex2f( 		2, 1.0
		); glVertex2f(		3, 0.5
		); glVertex2f(		7, 0.5
		); glVertex2f(		8, 1.0
		); glVertex2f(		7, 1.5
		); glVertex2f(		3, 1.5
		);

	glEnd(); glBegin( GL_POLYGON );

		// upper right
		LEDIF(0x40);
		glVertex2f( 		7, 2
		); glVertex2i(		7, 4
		); glVertex2i(		8, 5
		); glVertex2i(		9, 4
		); glVertex2i(		9, 2
		); glVertex2i(		8, 1
		);
		
	glEnd(); glBegin( GL_POLYGON );
		
		// lower right
		LEDIF(0x20);
		glVertex2f( 		7, 8
		); glVertex2i(		7, 6
		); glVertex2i(		8, 5
		); glVertex2i(		9, 6
		); glVertex2i(		9, 8
		); glVertex2i(		8, 9
		);

	glEnd(); glBegin( GL_POLYGON );
		
		// bottom
		LEDIF(0x10);
		glVertex2f(			2, 9.0
		); glVertex2f(		3, 8.5
		); glVertex2f(		7, 8.5
		); glVertex2f(		8, 9.0
		); glVertex2f(		7, 9.5
		); glVertex2f(		3, 9.5
		);

	glEnd(); glBegin( GL_POLYGON );
		
		// lower left
		LEDIF(0x08);
		glVertex2f( 		1, 8
		); glVertex2i(		1, 6
		); glVertex2i(		2, 5
		); glVertex2i(		3, 6
		); glVertex2i(		3, 8
		); glVertex2i(		2, 9
		);

	glEnd(); glBegin( GL_POLYGON );
		
		// upper left
		LEDIF(0x04);
		glVertex2f( 		1, 2
		); glVertex2i(		1, 4
		); glVertex2i(		2, 5
		); glVertex2i(		3, 4
		); glVertex2i(		3, 2
		); glVertex2i(		2, 1
		);
		
	glEnd(); glBegin(GL_POLYGON);

		// middle
		LEDIF(0x02);
		glVertex2f( 		2, 5.0
		); glVertex2f(		3, 4.5
		); glVertex2f(		7, 4.5
		); glVertex2f(		8, 5.0
		); glVertex2f(		7, 5.5
		); glVertex2f(		3, 5.5
		);

	glEnd(); glBegin(GL_POLYGON);
		// lower center
		LEDIF(0x01);
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
	
	if( ledAlpha >= 0.4 )
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

