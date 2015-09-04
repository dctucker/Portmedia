/*
 *  draw_flag.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-07-22.
 *  Copyright 2010. All rights reserved.
 *
 */

void drawCubanFlag()
{
	glPushMatrix();
	glTranslatef( 0, 0, 0 );
	glScalef( 0.001, 0.0015, 0.001 );
	glTranslatef( -310, 200, 0 );
	float alpha = 0.3;
	glBegin(GL_QUADS);
	
		glColor4f( 0, 0, 1, alpha ); // blue
		glVertex2i( 0, 0 );
		glVertex2i( 620, 0  );
		glVertex2i( 620, 60 );
		glVertex2i( 112 , 60 ); //112 * sinf( 2*M_PI*6./36. ), 60 );

		glColor4f( 1, 1, 1, alpha ); // white
		glVertex2i( 112, 60 );
		glVertex2i( 620, 60  );
		glVertex2i( 620, 120 );
		glVertex2i( 224, 120 );

		glColor4f( 1, 1, 1, alpha ); // white
		glVertex2i( 224, 180 );
		glVertex2i( 620, 180  );
		glVertex2i( 620, 240 );
		glVertex2i( 112, 240 );

		glColor4f( 0, 0, 1, alpha ); // blue
		glVertex2i( 112, 240 );
		glVertex2i( 280+340, 240  );
		glVertex2i( 280+340, 300 );
		glVertex2i( 0, 300 );
		
	glEnd();
	glBegin( GL_TRIANGLE_FAN );
		glColor4f( 0, 0, 1, alpha ); // blue
		glVertex2i( 280, 150 );
		glVertex2i( 224, 120 );
		glVertex2i( 620, 120 );
		glVertex2i( 620, 180 );
		glVertex2i( 224, 180 );

	glEnd();
	
	/*
	glBegin( GL_TRIANGLES );
		glColor4f( 1, 0, 0, alpha ); // red
		glVertex2i( 0, 0 );
		glVertex2i( 280, 150 );
		glVertex2i( 0, 300 );
	glEnd();
	*/
	glBegin( GL_QUADS );

	const float xp[] = {
		sinf( 2.*M_PI * 0.0 ),
		sinf( 2.*M_PI * 0.1 ),
		sinf( 2.*M_PI * 0.2 ),
		sinf( 2.*M_PI * 0.3 ),
		sinf( 2.*M_PI * 0.4 ),
		sinf( 2.*M_PI * 0.5 ),
		sinf( 2.*M_PI * 0.6 ),
		sinf( 2.*M_PI * 0.7 ),
		sinf( 2.*M_PI * 0.8 ),
		sinf( 2.*M_PI * 0.9 ),
		sinf( 2.*M_PI * 1.0 ),
		sinf( 2.*M_PI * 1.1 )
	};
	const float yp[] = {
		cosf( 2.*M_PI * 0.0 ),
		cosf( 2.*M_PI * 0.1 ),
		cosf( 2.*M_PI * 0.2 ),
		cosf( 2.*M_PI * 0.3 ),
		cosf( 2.*M_PI * 0.4 ),
		cosf( 2.*M_PI * 0.5 ),
		cosf( 2.*M_PI * 0.6 ),
		cosf( 2.*M_PI * 0.7 ),
		cosf( 2.*M_PI * 0.8 ),
		cosf( 2.*M_PI * 0.9 ),
		cosf( 2.*M_PI * 1.0 ),
		cosf( 2.*M_PI * 1.1 )
	};
	int is = 18;
	int os = 45;

	for(int i=1; i < 10; i+=2){
		glColor4f( 1,1,1, alpha );
		glVertex2i( 93, 150 ); 
		glVertex2i( 93 + is * xp[i-1], 150 - is * yp[i-1] );
		glVertex2i( 93 + os * xp[ i ], 150 - os * yp[ i ] );
		glVertex2i( 93 + is * xp[i+1], 150 - is * yp[i+1] );
	}

	glEnd();
	glBegin(GL_TRIANGLES);
	glColor4f( 1,0,0, alpha );
	for(int i=1; i < 11; i+=2){
		glVertex2i( 93 + os * xp[ i ], 150 - os * yp[ i ] );
		glVertex2i( 93 + is * xp[i+1], 150 - is * yp[i+1] );
		glVertex2i( 93 + os * xp[i+2], 150 - os * yp[i+2] );
	}
	glEnd();
	glBegin(GL_QUADS);
		glVertex2i( 112, 60 );
		glVertex2i( 0,0 );
		glVertex2i( 93 + os * xp[ 9  ], 150 - os * yp[ 9 ] );
		glVertex2i( 93 + os * xp[ 11 ], 150 - os * yp[ 11 ] );

		glVertex2i( 0,0 );
		glVertex2i( 0, 150 );
		glVertex2i( 93 + os * xp[ 7 ], 150 - os * yp[ 7 ] );
		glVertex2i( 93 + os * xp[ 9 ], 150 - os * yp[ 9 ] );
		
		glVertex2i( 0, 300 );
		glVertex2i( 93 + os * xp[ 5 ], 150 - os * yp[ 5 ] );
		glVertex2i( 93 + os * xp[ 7 ], 150 - os * yp[ 7 ] );
		glVertex2i( 0, 150 );

		glVertex2i( 280, 150 );
		glVertex2i( 93 + os * xp[ 3 ], 150 - os * yp[ 3 ] );
		glVertex2i( 93 + os * xp[ 5 ], 150 - os * yp[ 5 ] );
		glVertex2i( 0, 300 );

		glVertex2i( 112, 60 );
		glVertex2i( 93 + os * xp[ 1 ], 150 - os * yp[ 1 ] );
		glVertex2i( 93 + os * xp[ 3 ], 150 - os * yp[ 3 ] );
		glVertex2i( 280, 150 );
		
	glEnd();
	
	glPopMatrix();
}


void Canvas3D::drawCube()
{
	
	float alpha = 0.6f;
	glPushMatrix();
	//glOrtho( 0.0, 1.0, 1.0, 0.0, -1.0, 1.0 );
	//glFrustum( 0.1, 0.9, 1.0, 0.0, -10.0, 10.0 );
	
	
	glTranslatef( 0.3, -0.3, -0.5 );
	
	glScalef( 0.25, 0.25, 0.1 );

	glRotatef(rot, 0, 1, 0);
	glRotatef(30, 1, 0, 0);
	glRotatef(0, 0, 0, 1);
	
	rot += 0.5f;
	
	
	glBegin(GL_QUADS);
		glColor4f( 1.0, 0.0, 0.0, alpha );
        glNormal3f( 0.0f, 0.0f, 1.0f);
        glVertex3f( 0.5f, 0.5f, 0.5f); glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f,-0.5f, 0.5f); glVertex3f( 0.5f,-0.5f, 0.5f);

		glColor4f( 0.0, 1.0, 1.0, alpha );
        glNormal3f( 0.0f, 0.0f,-1.0f);
        glVertex3f(-0.5f,-0.5f,-0.5f); glVertex3f(-0.5f, 0.5f,-0.5f);
        glVertex3f( 0.5f, 0.5f,-0.5f); glVertex3f( 0.5f,-0.5f,-0.5f);

		glColor4f( 0.0, 1.0, 0.0, alpha );
        glNormal3f( 0.0f, 1.0f, 0.0f);
        glVertex3f( 0.5f, 0.5f, 0.5f); glVertex3f( 0.5f, 0.5f,-0.5f);
        glVertex3f(-0.5f, 0.5f,-0.5f); glVertex3f(-0.5f, 0.5f, 0.5f);

		glColor4f( 1.0, 0.0, 1.0, alpha );
        glNormal3f( 0.0f,-1.0f, 0.0f);
        glVertex3f(-0.5f,-0.5f,-0.5f); glVertex3f( 0.5f,-0.5f,-0.5f);
        glVertex3f( 0.5f,-0.5f, 0.5f); glVertex3f(-0.5f,-0.5f, 0.5f);

		glColor4f( 0.0, 0.0, 1.0, alpha );
        glNormal3f( 1.0f, 0.0f, 0.0f);
        glVertex3f( 0.5f, 0.5f, 0.5f); glVertex3f( 0.5f,-0.5f, 0.5f);
        glVertex3f( 0.5f,-0.5f,-0.5f); glVertex3f( 0.5f, 0.5f,-0.5f);

		glColor4f( 1.0, 1.0, 0.0, alpha );
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-0.5f,-0.5f,-0.5f); glVertex3f(-0.5f,-0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f); glVertex3f(-0.5f, 0.5f,-0.5f);	
	glEnd();
	
	glPopMatrix();
	//*/
}
