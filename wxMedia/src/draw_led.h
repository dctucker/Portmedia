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
     ---0-|-1---  
    7 \   9   / 2 
    |  8\ | /a  | 
    --f--- ---b-- 
    |  e/ | \c  | 
    6 /   d   \ 3 
     ---5-|-4---  
*/
const unsigned short led16_ee[] = {
	        //   0 1 2 3  4 5 6 7  8 9 a b  c d e f    //
	0x0000, // { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 }, // 
	0x8541, // { 1,0,0,0, 0,1,0,1, 0,1,0,0, 0,0,0,1 }, // 
	0x2100, // { 0,0,1,0, 0,0,0,1, 0,0,0,0, 0,0,0,0 }, //"
	0x3c55, // { 0,0,1,1, 1,1,0,0, 0,1,0,1, 0,1,0,1 }, //#
	//0xffff, // { 0,0,1,1, 1,1,0,0, 0,1,0,1, 0,1,0,1 }, //#
	0xdd55, // { 1,1,0,1, 1,1,0,1, 0,1,0,1, 0,1,0,1 }, //$
	0x9977, // { 1,0,0,1, 1,0,0,1, 0,1,1,1, 0,1,1,1 }, //%
	0x875d, // { 1,0,0,0, 0,1,1,1, 0,1,0,1, 1,1,0,1 }, //&
	0x0040, // { 0,0,0,0, 0,0,0,0, 0,1,0,0, 0,0,0,0 }, //'
	0x0028, // { 0,0,0,0, 0,0,0,0, 0,0,1,0, 1,0,0,0 }, //(
	0x0082, // { 0,0,0,0, 0,0,0,0, 1,0,0,0, 0,0,1,0 }, //)
	0x00ff, // { 0,0,0,0, 0,0,0,0, 1,1,1,1, 1,1,1,1 }, //*
	0x0055, // { 0,0,0,0, 0,0,0,0, 0,1,0,1, 0,1,0,1 }, //+
	0x0003, // { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,1,1 }, //,
	0x0011, // { 0,0,0,0, 0,0,0,0, 0,0,0,1, 0,0,0,1 }, //-
	0x0400, // { 0,0,0,0, 0,1,0,0, 0,0,0,0, 0,0,0,0 }, //.
	0x0022, // { 0,0,0,0, 0,0,0,0, 0,0,1,0, 0,0,1,0 }, ///
	0xff22, // { 1,1,1,1, 1,1,1,1, 0,0,1,0, 0,0,1,0 }, //0
	0x8c44, // { 1,0,0,0, 1,1,0,0, 0,1,0,0, 0,1,0,0 }, //1
	0xee11, // { 1,1,1,0, 1,1,1,0, 0,0,0,1, 0,0,0,1 }, //2
	0xfc11, // { 1,1,1,1, 1,1,0,0, 0,0,0,1, 0,0,0,1 }, //3
	0x3111, // { 0,0,1,1, 0,0,0,1, 0,0,0,1, 0,0,0,1 }, //4
	0xdc90, // { 1,1,0,1, 1,1,0,0, 1,0,0,1, 0,0,0,0 }, //5
	0xdf11, // { 1,1,0,1, 1,1,1,1, 0,0,0,1, 0,0,0,1 }, //6
	0xc023, // { 1,1,0,0, 0,0,0,0, 0,0,1,0, 0,0,1,1 }, //7
	0xff11, // { 1,1,1,1, 1,1,1,1, 0,0,0,1, 0,0,0,1 }, //8
	0xfd11, // { 1,1,1,1, 1,1,0,1, 0,0,0,1, 0,0,0,1 }, //9
	0x8400, // { 1,0,0,0, 0,1,0,0, 0,0,0,0, 0,0,0,0 }, //:
	0x8003, // { 1,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,1,1 }, //;
	0x0c22, // { 0,0,0,0, 1,1,0,0, 0,0,1,0, 0,0,1,0 }, //<
	0x0c11, // { 0,0,0,0, 1,1,0,0, 0,0,0,1, 0,0,0,1 }, //=
	0x0c88, // { 0,0,0,0, 1,1,0,0, 1,0,0,0, 1,0,0,0 }, //>
	0xe014, // { 1,1,1,0, 0,0,0,0, 0,0,0,1, 0,1,0,0 }, //?
	0xfe05, // { 1,1,1,1, 1,1,1,0, 0,0,0,0, 0,1,0,1 }, //@
	0x3032, // { 0,0,1,1, 0,0,0,0, 0,0,1,1, 0,0,1,0 }, //A
	0xfc54, // { 1,1,1,1, 1,1,0,0, 0,1,0,1, 0,1,0,0 }, //B
	0xcf00, // { 1,1,0,0, 1,1,1,1, 0,0,0,0, 0,0,0,0 }, //C
	0xfc44, // { 1,1,1,1, 1,1,0,0, 0,1,0,0, 0,1,0,0 }, //D
	0xcf11, // { 1,1,0,0, 1,1,1,1, 0,0,0,1, 0,0,0,1 }, //E
	0xc311, // { 1,1,0,0, 0,0,1,1, 0,0,0,1, 0,0,0,1 }, //F
	0xdf10, // { 1,1,0,1, 1,1,1,1, 0,0,0,1, 0,0,0,0 }, //G
	0x3311, // { 0,0,1,1, 0,0,1,1, 0,0,0,1, 0,0,0,1 }, //H
	0xcc44, // { 1,1,0,0, 1,1,0,0, 0,1,0,0, 0,1,0,0 }, //I
	0xc644, // { 1,1,0,0, 0,1,1,0, 0,1,0,0, 0,1,0,0 }, //J
	0x0329, // { 0,0,0,0, 0,0,1,1, 0,0,1,0, 1,0,0,1 }, //K
	0x0f00, // { 0,0,0,0, 1,1,1,1, 0,0,0,0, 0,0,0,0 }, //L
	0x33a4, // { 0,0,1,1, 0,0,1,1, 1,0,1,0, 0,1,0,0 }, //M
	0x3388, // { 0,0,1,1, 0,0,1,1, 1,0,0,0, 1,0,0,0 }, //N
	0xff00, // { 1,1,1,1, 1,1,1,1, 0,0,0,0, 0,0,0,0 }, //O
	0xe311, // { 1,1,1,0, 0,0,1,1, 0,0,0,1, 0,0,0,1 }, //P
	0xff08, // { 1,1,1,1, 1,1,1,1, 0,0,0,0, 1,0,0,0 }, //Q
	0xe319, // { 1,1,1,0, 0,0,1,1, 0,0,0,1, 1,0,0,1 }, //R
	0xee88, // { 1,1,1,0, 1,1,1,0, 1,0,0,0, 1,0,0,0 }, //S
	0xc044, // { 1,1,0,0, 0,0,0,0, 0,1,0,0, 0,1,0,0 }, //T
	0x3f00, // { 0,0,1,1, 1,1,1,1, 0,0,0,0, 0,0,0,0 }, //U
	0x0322, // { 0,0,0,0, 0,0,1,1, 0,0,1,0, 0,0,1,0 }, //V
	0x334a, // { 0,0,1,1, 0,0,1,1, 0,1,0,0, 1,0,1,0 }, //W
	0x00aa, // { 0,0,0,0, 0,0,0,0, 1,0,1,0, 1,0,1,0 }, //X
	0x00a4, // { 0,0,0,0, 0,0,0,0, 1,0,1,0, 0,1,0,0 }, //Y
	0xcc22, // { 1,1,0,0, 1,1,0,0, 0,0,1,0, 0,0,1,0 }, //Z
	0x4844, // { 0,1,0,0, 1,0,0,0, 0,1,0,0, 0,1,0,0 }, //[
	0x0088, // { 0,0,0,0, 0,0,0,0, 1,0,0,0, 1,0,0,0 }, //\/
	0x8444, // { 1,0,0,0, 0,1,0,0, 0,1,0,0, 0,1,0,0 }, //]
	0x2020, // { 0,0,1,0, 0,0,0,0, 0,0,1,0, 0,0,0,0 }, //^
	0x0c00, // { 0,0,0,0, 1,1,0,0, 0,0,0,0, 0,0,0,0 }, //_
	0x0080, // { 0,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,0,0 }, //`
	//
	0x3032, // { 0,0,1,1, 0,0,0,0, 0,0,1,1, 0,0,1,0 }, //A
	0xfc54, // { 1,1,1,1, 1,1,0,0, 0,1,0,1, 0,1,0,0 }, //B
	0xcf00, // { 1,1,0,0, 1,1,1,1, 0,0,0,0, 0,0,0,0 }, //C
	0xfc44, // { 1,1,1,1, 1,1,0,0, 0,1,0,0, 0,1,0,0 }, //D
	0xcf11, // { 1,1,0,0, 1,1,1,1, 0,0,0,1, 0,0,0,1 }, //E
	0xc311, // { 1,1,0,0, 0,0,1,1, 0,0,0,1, 0,0,0,1 }, //F
	0xdf10, // { 1,1,0,1, 1,1,1,1, 0,0,0,1, 0,0,0,0 }, //G
	0x3311, // { 0,0,1,1, 0,0,1,1, 0,0,0,1, 0,0,0,1 }, //H
	0xcc44, // { 1,1,0,0, 1,1,0,0, 0,1,0,0, 0,1,0,0 }, //I
	0xc644, // { 1,1,0,0, 0,1,1,0, 0,1,0,0, 0,1,0,0 }, //J
	0x0329, // { 0,0,0,0, 0,0,1,1, 0,0,1,0, 1,0,0,1 }, //K
	0x0f00, // { 0,0,0,0, 1,1,1,1, 0,0,0,0, 0,0,0,0 }, //L
	0x33a4, // { 0,0,1,1, 0,0,1,1, 1,0,1,0, 0,1,0,0 }, //M
	0x3388, // { 0,0,1,1, 0,0,1,1, 1,0,0,0, 1,0,0,0 }, //N
	0xff00, // { 1,1,1,1, 1,1,1,1, 0,0,0,0, 0,0,0,0 }, //O
	0xe311, // { 1,1,1,0, 0,0,1,1, 0,0,0,1, 0,0,0,1 }, //P
	0xff08, // { 1,1,1,1, 1,1,1,1, 0,0,0,0, 1,0,0,0 }, //Q
	0xe319, // { 1,1,1,0, 0,0,1,1, 0,0,0,1, 1,0,0,1 }, //R
	0xee88, // { 1,1,1,0, 1,1,1,0, 1,0,0,0, 1,0,0,0 }, //S
	0xc044, // { 1,1,0,0, 0,0,0,0, 0,1,0,0, 0,1,0,0 }, //T
	0x3f00, // { 0,0,1,1, 1,1,1,1, 0,0,0,0, 0,0,0,0 }, //U
	0x0322, // { 0,0,0,0, 0,0,1,1, 0,0,1,0, 0,0,1,0 }, //V
	0x334a, // { 0,0,1,1, 0,0,1,1, 0,1,0,0, 1,0,1,0 }, //W
	0x00aa, // { 0,0,0,0, 0,0,0,0, 1,0,1,0, 1,0,1,0 }, //X
	0x00a4, // { 0,0,0,0, 0,0,0,0, 1,0,1,0, 0,1,0,0 }, //Y
	0xcc22, // { 1,1,0,0, 1,1,0,0, 0,0,1,0, 0,0,1,0 }, //Z
	// a-z
	0x4845, // { 0,1,0,0, 1,0,0,0, 0,1,0,0, 0,1,0,1 }, //{
	0x0044, // { 0,0,0,0, 0,0,0,0, 0,1,0,0, 0,1,0,0 }, //|
	0x8454, // { 1,0,0,0, 0,1,0,0, 0,1,0,1, 0,1,0,0 }, //}
	0xa150, // { 1,0,1,0, 0,0,0,1, 0,1,0,1, 0,0,0,0 }, //~
};

static const GLint led16_vertex_data[] = {
	// top left
	-9,  9,
	-8, 10,
	-1, 10,
	 0,  9,
	-1,  8,
	-8,  8,
	// top right
	 0,  9,
	 1, 10,
	 8, 10,
	 9,  9,
	 8,  8,
	 1,  8,
	// upper right
	 8,  8,
	 8,  1,
	 9,  0,
	10,  1,
	10,  8,
	 9,  9,
	// lower right
	 8, -8,
	 8, -1,
	 9,  0,
	10, -1,
	10, -8,
	 9, -9,
	// bottom right
	 9,  -9,
	 8, -10,
	 1, -10,
	 0,  -9,
	 1,  -8,
	 8,  -8,
	// bottom left
	-9,  -9,
	-8, -10,
	-1, -10,
	-0,  -9,
	-1,  -8,
	-8,  -8,
	// lower left
	-9,   0,
	-10, -1,
	-10, -8,
	-9,  -9,
	-8,  -8,
	-8,  -1,
	// upper left
	-9,  0,
	-10, 1,
	-10, 8,
	-9,  9,
	-8,  8,
	-8,  1,
	// diagonal upper left
	 0, 0,
	-1, 1,
	-8, 7,
	-8, 8,
	-7, 8,
	-1, 2,
	// top center
	 0, 0,
	-1, 2,
	-1, 8,
	 0, 9,
	 1, 8,
	 1, 2,
	// diagonal upper right
	 0, 0,
	 1, 1,
	 8, 7,
	 8, 8,
	 7, 8,
	 1, 2,
	// right middle
	 0,  0,
	 2,  1,
	 8,  1,
	 9,  0,
	 8, -1,
	 2, -1,
	// diagonal lower right
	 0, -0,
	 1, -1,
	 8, -7,
	 8, -8,
	 7, -8,
	 1, -2,
	// bottom center
	 0,  0,
	-1, -2,
	-1, -8,
	 0, -9,
	 1, -8,
	 1, -2,
	// diagonal lower left
	 0,  0,
	-1, -1,
	-8, -7,
	-8, -8,
	-7, -8,
	-1, -2,
	// left middle
	 0,  0,
	-2,  1,
	-8,  1,
	-9,  0,
	-8, -1,
	-2, -1,
};

#define LEDRED   glColor4f( r, g, b, alpha )
#define LEDBLACK glColor4f( 0, 0, 0, alpha )
#define LED8IF(x)  if( e & x ) LEDRED; else LEDBLACK
#define LED16IF(x) if( e & x ) LEDRED; else LEDBLACK

void drawLED8char(char c, float r, float g, float b, float alpha)
{
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
		LED8IF(0x80);
		glVertex2f( 		2, 1.0
		); glVertex2f(		3, 0.5
		); glVertex2f(		7, 0.5
		); glVertex2f(		8, 1.0
		); glVertex2f(		7, 1.5
		); glVertex2f(		3, 1.5
		);

	glEnd(); glBegin( GL_POLYGON );

		// upper right
		LED8IF(0x40);
		glVertex2f( 		7, 2
		); glVertex2i(		7, 4
		); glVertex2i(		8, 5
		); glVertex2i(		9, 4
		); glVertex2i(		9, 2
		); glVertex2i(		8, 1
		);
		
	glEnd(); glBegin( GL_POLYGON );
		
		// lower right
		LED8IF(0x20);
		glVertex2f( 		7, 8
		); glVertex2i(		7, 6
		); glVertex2i(		8, 5
		); glVertex2i(		9, 6
		); glVertex2i(		9, 8
		); glVertex2i(		8, 9
		);

	glEnd(); glBegin( GL_POLYGON );
		
		// bottom
		LED8IF(0x10);
		glVertex2f(			2, 9.0
		); glVertex2f(		3, 8.5
		); glVertex2f(		7, 8.5
		); glVertex2f(		8, 9.0
		); glVertex2f(		7, 9.5
		); glVertex2f(		3, 9.5
		);

	glEnd(); glBegin( GL_POLYGON );
		
		// lower left
		LED8IF(0x08);
		glVertex2f( 		1, 8
		); glVertex2i(		1, 6
		); glVertex2i(		2, 5
		); glVertex2i(		3, 6
		); glVertex2i(		3, 8
		); glVertex2i(		2, 9
		);

	glEnd(); glBegin( GL_POLYGON );
		
		// upper left
		LED8IF(0x04);
		glVertex2f( 		1, 2
		); glVertex2i(		1, 4
		); glVertex2i(		2, 5
		); glVertex2i(		3, 4
		); glVertex2i(		3, 2
		); glVertex2i(		2, 1
		);
		
	glEnd(); glBegin(GL_POLYGON);

		// middle
		LED8IF(0x02);
		glVertex2f( 		2, 5.0
		); glVertex2f(		3, 4.5
		); glVertex2f(		7, 4.5
		); glVertex2f(		8, 5.0
		); glVertex2f(		7, 5.5
		); glVertex2f(		3, 5.5
		);

	glEnd(); glBegin(GL_POLYGON);
		// lower center
		LED8IF(0x01);
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

void Canvas3D::drawLED16char(char c, float r, float g, float b, float alpha)
{

	//const int *e;
	unsigned short e;
	
	if( c == '.' or c == ',' or c == '!' or c == '"' or c == '\'' or c == ':' or c == ';')
		glScalef( 0.4, 1.0, 1.0 );
	
	if( c >= ' ' and c <= '~') 
		e = led16_ee[c - ' '];
	else
		e = led16_ee[0];
	
	glBegin( GL_POLYGON );
	
		glColor4f( 0.0, 0.0, 0.0, alpha );
		glVertex3f( 0, 0, 0.001 );
		glVertex3f( 1, 0, 0.001 );
		glVertex3f( 1, 1, 0.001 );
		glVertex3f( 0, 1, 0.001 );
	
	glEnd();
	
	glScalef( 0.05, 0.05, 1.0 );	
	glTranslatef( 10.0, 10.0, 0.0 );

	glScalef( 0.8, -1.0, 1.0 );		

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, ledVertexBuffer);
	glVertexPointer( 2, GL_INT, 0, (void *)0 );
	//glVertexAttribPointer( 0, 2, GL_INT, GL_FALSE, 0, (void *)0 );

	LED16IF(0x8000); glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
	LED16IF(0x4000); glDrawArrays(GL_TRIANGLE_FAN, 6, 6);
	LED16IF(0x2000); glDrawArrays(GL_TRIANGLE_FAN, 12, 6);
	LED16IF(0x1000); glDrawArrays(GL_TRIANGLE_FAN, 18, 6);
	LED16IF(0x0800); glDrawArrays(GL_TRIANGLE_FAN, 24, 6);
	LED16IF(0x0400); glDrawArrays(GL_TRIANGLE_FAN, 30, 6);
	LED16IF(0x0200); glDrawArrays(GL_TRIANGLE_FAN, 36, 6);
	LED16IF(0x0100); glDrawArrays(GL_TRIANGLE_FAN, 42, 6);
	LED16IF(0x0080); glDrawArrays(GL_TRIANGLE_FAN, 48, 6);
	LED16IF(0x0040); glDrawArrays(GL_TRIANGLE_FAN, 54, 6);
	LED16IF(0x0020); glDrawArrays(GL_TRIANGLE_FAN, 60, 6);
	LED16IF(0x0010); glDrawArrays(GL_TRIANGLE_FAN, 66, 6);
	LED16IF(0x0008); glDrawArrays(GL_TRIANGLE_FAN, 72, 6);
	LED16IF(0x0004); glDrawArrays(GL_TRIANGLE_FAN, 78, 6);
	LED16IF(0x0002); glDrawArrays(GL_TRIANGLE_FAN, 84, 6);
	LED16IF(0x0001); glDrawArrays(GL_TRIANGLE_FAN, 90, 6);

	glDisableVertexAttribArray(0);
	glDisableClientState(GL_VERTEX_ARRAY);
/*
	glBegin( GL_TRIANGLE_FAN );

		// top left
		LED16IF(0x8000);
		glVertex2i( 		-9,  9
		); glVertex2i(		-8, 10
		); glVertex2i(		-1, 10
		); glVertex2i(		 0,  9
		); glVertex2i(		-1,  8
		); glVertex2i(		-8,  8
		);

	glEnd(); glBegin( GL_TRIANGLE_FAN );

		// top right
		LED16IF(0x4000);
		glVertex2i( 		0, 9
		); glVertex2i(		1, 10
		); glVertex2i(		8, 10
		); glVertex2i(		9, 9
		); glVertex2i(		8, 8
		); glVertex2i(		1, 8
		);

	glEnd(); glBegin( GL_TRIANGLE_FAN );

		// upper right
		LED16IF(0x2000);
		glVertex2i( 		 8, 8
		); glVertex2i(		 8, 1
		); glVertex2i(		 9, 0
		); glVertex2i(		10, 1
		); glVertex2i(		10, 8
		); glVertex2i(		 9, 9
		);
		
	glEnd(); glBegin( GL_TRIANGLE_FAN );
		
		// lower right
		LED16IF(0x1000);
		glVertex2i( 		 8, -8
		); glVertex2i(		 8, -1
		); glVertex2i(		 9,  0
		); glVertex2i(		10, -1
		); glVertex2i(		10, -8
		); glVertex2i(		 9, -9
		);

	glEnd(); glBegin( GL_TRIANGLE_FAN );
		
		// bottom right
		LED16IF(0x0800);
		glVertex2i(			 9,  -9
		); glVertex2i(		 8, -10
		); glVertex2i(		 1, -10
		); glVertex2i(		 0,  -9
		); glVertex2i(		 1,  -8
		); glVertex2i(		 8,  -8
		);

	glEnd(); glBegin( GL_TRIANGLE_FAN );

		// bottom left
		LED16IF(0x0400);
		glVertex2f(			 -9,  -9
		); glVertex2i(		 -8, -10
		); glVertex2i(		 -1, -10
		); glVertex2i(		 -0,  -9
		); glVertex2i(		 -1,  -8
		); glVertex2i(		 -8,  -8
		);

	glEnd(); glBegin( GL_TRIANGLE_FAN );
		
		// lower left
		LED16IF(0x0200);
		glVertex2i( 		-9,   0
		); glVertex2i(		-10, -1
		); glVertex2i(		-10, -8
		); glVertex2i(		-9,  -9
		); glVertex2i(		-8,  -8
		); glVertex2i(		-8,  -1
		);

	glEnd(); glBegin( GL_TRIANGLE_FAN );
		
		// upper left
		LED16IF(0x0100);
		glVertex2i( 		-9,  0
		); glVertex2i(		-10, 1
		); glVertex2i(		-10, 8
		); glVertex2i(		-9,  9
		); glVertex2i(		-8,  8
		); glVertex2i(		-8,  1
		);
		
	glEnd(); glBegin(GL_TRIANGLE_FAN);
		
		// diagonal upper left
		LED16IF(0x0080);
		glVertex2i( 		 0, 0
		); glVertex2i(		-1, 1
		); glVertex2i(		-8, 7
		); glVertex2i(		-8, 8
		); glVertex2i(		-7, 8
		); glVertex2i(		-1, 2
		);

	glEnd(); glBegin(GL_TRIANGLE_FAN);
		
		// top center
		LED16IF(0x0040);
		glVertex2i( 		 0, 0
		); glVertex2i(		-1, 2
		); glVertex2i(		-1, 8
		); glVertex2i(		 0, 9
		); glVertex2i(		 1, 8
		); glVertex2i(		 1, 2
		);

	glEnd(); glBegin(GL_TRIANGLE_FAN);
		
		// diagonal upper right
		LED16IF(0x0020);
		glVertex2i( 		0, 0
		); glVertex2i(		1, 1
		); glVertex2i(		8, 7
		); glVertex2i(		8, 8
		); glVertex2i(		7, 8
		); glVertex2i(		1, 2
		);

	glEnd(); glBegin(GL_TRIANGLE_FAN);
		
		// right middle
		LED16IF(0x0010);
		glVertex2i( 		 0,  0
		); glVertex2i(		 2,  1
		); glVertex2i(		 8,  1
		); glVertex2i(		 9,  0
		); glVertex2i(		 8, -1
		); glVertex2i(		 2, -1
		);

	glEnd(); glBegin(GL_TRIANGLE_FAN);
		
		// diagonal lower right
		LED16IF(0x0008);
		glVertex2i( 		0, -0
		); glVertex2i(		1, -1
		); glVertex2i(		8, -7
		); glVertex2i(		8, -8
		); glVertex2i(		7, -8
		); glVertex2i(		1, -2
		);

	glEnd(); glBegin(GL_TRIANGLE_FAN);
		
		// bottom center
		LED16IF(0x0004);
		glVertex2i( 		 0,  0
		); glVertex2i(		-1, -2
		); glVertex2i(		-1, -8
		); glVertex2i(		 0, -9
		); glVertex2i(		 1, -8
		); glVertex2i(		 1, -2
		);

	glEnd(); glBegin(GL_TRIANGLE_FAN);
		
		// diagonal lower left
		LED16IF(0x0002);
		glVertex2i( 		 0,  0
		); glVertex2i(		-1, -1
		); glVertex2i(		-8, -7
		); glVertex2i(		-8, -8
		); glVertex2i(		-7, -8
		); glVertex2i(		-1, -2
		);

	glEnd(); glBegin(GL_TRIANGLE_FAN);
		
		// left middle
		LED16IF(0x0001);
		glVertex2i( 		  0,  0
		); glVertex2i(		 -2,  1
		); glVertex2i(		 -8,  1
		); glVertex2i(		 -9,  0
		); glVertex2i(		 -8, -1
		); glVertex2i(		 -2, -1
		);

	glEnd();
*/

	glScalef( 1.25, -1.0, 1.0 );		

	glTranslatef( -10.0, -10.0, 0.0 );
	glScalef( 20.0, 20.0, 1.0 );		
	glTranslatef( 1.0, 0.0, 0.0 ); // advance

	if( c == '.' or c == ',' or c == '!' or c == '"' or c == '\'' or c == ':' or c == ';')
		glScalef( 2.5, 1.0, 1.0 );

}

void Canvas3D::drawLED( char *str, float r, float g, float b, float a )
{
	for(int i=0; i < strlen(str); i++)
	{
		drawLED16char(str[i], r,g,b,a);
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

