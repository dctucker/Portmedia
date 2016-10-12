/*
 *  draw_bcr.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-07-22.
 *  Copyright 2010. All rights reserved.
 *
 */

void Canvas3D::drawBCR()
{
	float alpha = 0.8f;
	float
		bleft = 0.0, brigh = 0.15,
		btop = 0.6, bbot = 0.8,
		bwid = brigh - bleft,
		bhei = bbot - btop
		;
		//glColor4f( 1.0,1.0,1.0,1.0);

	//char i;	
	float val = 0.4;

	glPushMatrix();
		//glRotatef( 60, 0, 1, 0 );
		
		glColor4f( 0.2, 0.2, 0.4, alpha );
		
		glBegin(GL_QUADS);
			glNormal3f( 0, 0, -1 );
			glVertex2f( bleft, btop );
			glVertex2f( brigh, btop );
			glVertex2f( brigh, bbot );
			glVertex2f( bleft, bbot );
		glEnd();

		// zoom into the BCR panel now
		glTranslatef( bleft, btop, 0.0 );
		glScalef( bwid, bhei, 1.0 );
		
		for(int row = 0; row < 9; row++)
		{
			for(int col = 0; col < 8; col++)
			{
				switch(row)
				{
					case 0:
					case 1:
					case 2:
					case 3:
						val = bcr->getKnob(   (row + 4) * 10 + (col + 1) );
						break;
					case 4:
					case 5:
						val = bcr->getButton( (row + 4) * 10 + (col + 1) );
						break;
					case 6:
					case 7:
					case 8:
						val = bcr->getKnob(   (row - 5) * 10 + (col + 1) );
						break;

				}
			
				glPushMatrix();
				
					// zoom into the current element
					glTranslatef( 0.01 + col * 0.125, 0.075 + row * 0.1, 0.0 );
					glScalef( 0.10, 0.0625, 1.0 );

					glBegin(GL_QUADS);
						glNormal3f( 0, 0, -1 );

						glColor4f( 1.0, 0.0, 0.0, alpha );
						glVertex2f( 0.0, 0.0 );
						glVertex2f( val, 0.0 );
						glVertex2f( val, 1.0 );
						glVertex2f( 0.0, 1.0 );
					
						glColor4f( 0.0, 0.0, 0.0, alpha );
						glVertex2f( val, 0.0 );
						glVertex2f( 1.0, 0.0 );
						glVertex2f( 1.0, 1.0 );
						glVertex2f( val, 1.0 );
					glEnd();
					
				glPopMatrix();
			}
		}
	
	glPopMatrix();
}

