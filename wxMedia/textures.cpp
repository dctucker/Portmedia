/*
 *  textures.cpp
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-05-05.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "textures.h"

#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/glcanvas.h"
 
// include OpenGL
#ifdef __WXMAC__
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
#else
#include <GL/glu.h>
#include <GL/gl.h>
#endif
 
Image* image = NULL;
Drawable* sprite;

/*
class MyApp: public wxApp
{
    virtual bool OnInit();
 
    wxFrame *frame;
    BasicGLPane * glPane;
public:
      
};
 
IMPLEMENT_APP(MyApp)
 
 
bool MyApp::OnInit()
{
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    frame = new wxFrame((wxFrame *)NULL, -1,  wxT("Hello GL World"), wxPoint(50,50), wxSize(400,200));
	
    int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
    
    glPane = new BasicGLPane( (wxFrame*) frame, args);
    sizer->Add(glPane, 1, wxEXPAND);
	
    frame->SetSizer(sizer);
    frame->SetAutoLayout(true);
	
    frame->Show();
    return true;
} 
 
 
BEGIN_EVENT_TABLE(BasicGLPane, wxGLCanvas)
EVT_MOTION(BasicGLPane::mouseMoved)
EVT_LEFT_DOWN(BasicGLPane::mouseDown)
EVT_LEFT_UP(BasicGLPane::mouseReleased)
EVT_RIGHT_DOWN(BasicGLPane::rightClick)
EVT_LEAVE_WINDOW(BasicGLPane::mouseLeftWindow)
EVT_SIZE(BasicGLPane::resized)
EVT_KEY_DOWN(BasicGLPane::keyPressed)
EVT_KEY_UP(BasicGLPane::keyReleased)
EVT_MOUSEWHEEL(BasicGLPane::mouseWheelMoved)
EVT_PAINT(BasicGLPane::render)
END_EVENT_TABLE()
 
 
// some useful events to use
void BasicGLPane::mouseMoved(wxMouseEvent& event) {}
void BasicGLPane::mouseDown(wxMouseEvent& event) {}
void BasicGLPane::mouseWheelMoved(wxMouseEvent& event) {}
void BasicGLPane::mouseReleased(wxMouseEvent& event) {}
void BasicGLPane::rightClick(wxMouseEvent& event) {}
void BasicGLPane::mouseLeftWindow(wxMouseEvent& event) {}
void BasicGLPane::keyPressed(wxKeyEvent& event) {}
void BasicGLPane::keyReleased(wxKeyEvent& event) {}
 
BasicGLPane::BasicGLPane(wxFrame* parent, int* args) :
wxGLCanvas(parent, wxID_ANY,  wxDefaultPosition, wxDefaultSize, 0, wxT("GLCanvas"),  args)
{
}
 
void BasicGLPane::resized(wxSizeEvent& evt)
{
    wxGLCanvas::OnSize(evt);
	
    Refresh();
}
 
void BasicGLPane::prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{
     //  Inits the OpenGL viewport for drawing in 3D.
	
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
    glClearDepth(1.0f);	// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST); // Enables Depth Testing
    glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
    glEnable(GL_COLOR_MATERIAL);
	
    glViewport(topleft_x, topleft_y, bottomrigth_x-topleft_x, bottomrigth_y-topleft_y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
    float ratio_w_h = (float)(bottomrigth_x-topleft_x)/(float)(bottomrigth_y-topleft_y);
    gluPerspective(45 //view angle,
		ratio_w_h, 
		0.1 //clip close, 
		200 //clip far 
	);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
}
 
void BasicGLPane::prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{
	
    // Inits the OpenGL viewport for drawing in 2D
	
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
    glEnable(GL_TEXTURE_2D);   // textures
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
    glViewport(topleft_x, topleft_y, bottomrigth_x-topleft_x, bottomrigth_y-topleft_y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluOrtho2D(topleft_x, bottomrigth_x, bottomrigth_y, topleft_y);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int BasicGLPane::getWidth()
{
    return GetSize().x;
}
 
int BasicGLPane::getHeight()
{
    return GetSize().y;
}
 
void BasicGLPane::render( wxPaintEvent& evt )
{
    if(!IsShown()) return;
    
    wxGLCanvas::SetCurrent();
 
    
    if(image == NULL)
    {
        image = new Image( wxT("myfile.png") );
        sprite = new Drawable(image);
    }
 
    wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    // render loaded image
    prepare2DViewport(0,0,getWidth(), getHeight());
    sprite->render();
	
    glFlush();
    SwapBuffers();
}
*/

Image::Image()
{
}
 
Image::Image(wxString path)
{
    load(path);
}
 
void Image::load(wxString path)
{
    ID=loadImage(path, &width, &height, &textureWidth, &textureHeight);
    
    tex_coord_x  = (float) width  / (float) textureWidth  ;
    tex_coord_y  = (float) height / (float) textureHeight ;
}
 
GLuint* Image::getID()
{
    return ID;
}
 
Image::~Image()
{
    glDeleteTextures (1, ID);
}

#include <iostream>
 
#ifdef __WXMAC__
#include "OpenGL/gl.h"
#else
#include <GL/gl.h>
#endif
 
#include "wx/wx.h"
 
/*
 * This is a simple class built on top of OpenGL that manages drawing images in
a higher-level and quicker way.
 */
 
Drawable::Drawable(Image* image_arg)
{
 
    x=0;
    y=0;
    hotspotX=0;
    hotspotY=0;
    angle=0;
    
    xscale=1;
    yscale=1;
 
    xflip=false;
    yflip=false;
    
    if(image_arg!=NULL) setImage(image_arg);
    else image=NULL;
}
 
void Drawable::setFlip(bool x, bool y)
{
    xflip=x;
    yflip=y;
}
 
void Drawable::setHotspot(int x, int y)
{
    hotspotX=x;
    hotspotY=y;
}
 
void Drawable::move(int x, int y)
{
    Drawable::x=x;
    Drawable::y=y;
}
 
void Drawable::scale(float x, float y)
{
    Drawable::xscale=x;
    Drawable::yscale=y;
}
 
void Drawable::scale(float k)
{
    Drawable::xscale=k;
    Drawable::yscale=k;
}
 
void Drawable::setImage(Image* image)
{
    Drawable::image=image;
}
 
void Drawable::rotate(int angle)
{
    Drawable::angle=angle;
}
 
void Drawable::render()
{
    assert(image!=NULL);
    
    glLoadIdentity();
 
    glTranslatef(x,y,0);
    
    if(xscale!=1 || yscale!=1)
	{
        glScalef(xscale, yscale, 1);
    }
    
    if(angle!=0)
	{
        glRotatef(angle, 0,0,1);   
    }
    
    glBindTexture(GL_TEXTURE_2D, image->getID()[0] );
    
    glBegin(GL_QUADS);
    
    glTexCoord2f(xflip? image->tex_coord_x : 0, yflip? 0 : image->tex_coord_y);
    glVertex2f( -hotspotX, -hotspotY );
    
    glTexCoord2f(xflip? 0 : image->tex_coord_x, yflip? 0 : image->tex_coord_y);
    glVertex2f( image->width-hotspotX, -hotspotY );
    
    glTexCoord2f(xflip? 0 : image->tex_coord_x, yflip? image->tex_coord_y : 0);
    glVertex2f( image->width-hotspotX, image->height-hotspotY );
    
    glTexCoord2f(xflip? image->tex_coord_x : 0, yflip? image->tex_coord_y : 0);
    glVertex2f( -hotspotX, image->height-hotspotY );
    
    glEnd();
    
}


#include <cmath>
 
#include "wx/image.h"
#include "wx/wx.h"
 
 
GLuint* loadImage(wxString path, int* imageWidth, int* imageHeight, int* textureWidth, int* textureHeight)
{
	
	GLuint* ID=new GLuint[1];
	glGenTextures( 1, &ID[0] );
	
	glBindTexture( GL_TEXTURE_2D, *ID );
	
	// the first time, init image handlers (remove this part if you do it somewhere else in your app)
	static bool is_first_time = true;
	if(is_first_time)
	{
		wxInitAllImageHandlers();
		
		is_first_time = false;
	}
	
	// check the file exists
	if(!wxFileExists(path))
	{
		wxMessageBox( _("Failed to load resource image") );
		exit(1);	
	}
    
	wxImage* img=new wxImage( path );
	
	(*imageWidth) = img->GetWidth();
	(*imageHeight) = img->GetHeight();
	
	glPixelStorei(GL_UNPACK_ALIGNMENT,   1   );
	
    /*
     * Many graphics card require that textures be power of two.
     * Below is a simple implementation, probably not optimal but working.
     * If your texture sizes are not restricted to power of 2s, you can
     * of course adapt the bit below as needed.
     */
    
	float power_of_two_that_gives_correct_width=std::log((float)(*imageWidth))/std::log(2.0);
	float power_of_two_that_gives_correct_height=std::log((float)(*imageHeight))/std::log(2.0);
	
        // check if image dimensions are a power of two
        if( (int)power_of_two_that_gives_correct_width == power_of_two_that_gives_correct_width &&
            (int)power_of_two_that_gives_correct_height == power_of_two_that_gives_correct_height)
        {
                // note: must make a local copy before passing the data to OpenGL, as GetData() returns RGB 
                // and we want the Alpha channel if it's present. Additionally OpenGL seems to interpret the 
                // data upside-down so we need to compensate for that.
                GLubyte *bitmapData=img->GetData();
                GLubyte *alphaData=img->GetAlpha();
                GLubyte *imageData;
 
                int bytesPerPixel = img->HasAlpha() ?  4 : 3;
 
                int imageSize = (*imageWidth) * (*imageHeight) * bytesPerPixel;
                imageData=(GLubyte *)malloc(imageSize);
 
                int rev_val=(*imageHeight)-1;
 
                for(int y=0; y<(*imageHeight); y++)
                {
                        for(int x=0; x<(*imageWidth); x++)
                        {
                                imageData[(x+y*(*imageWidth))*bytesPerPixel+0]=
                                        bitmapData[( x+(rev_val-y)*(*imageWidth))*3];
 
                                imageData[(x+y*(*imageWidth))*bytesPerPixel+1]=
                                        bitmapData[( x+(rev_val-y)*(*imageWidth))*3 + 1];
 
                                imageData[(x+y*(*imageWidth))*bytesPerPixel+2]=
                                        bitmapData[( x+(rev_val-y)*(*imageWidth))*3 + 2];
 
                                if(bytesPerPixel==4) imageData[(x+y*(*imageWidth))*bytesPerPixel+3]=
                                        alphaData[ x+(rev_val-y)*(*imageWidth) ];
                        }//next
                }//next
 
                // if yes, everything is fine
                glTexImage2D(GL_TEXTURE_2D,
                             0,
                             bytesPerPixel,
                             *imageWidth,
                             *imageHeight,
                             0,
                             img->HasAlpha() ?  GL_RGBA : GL_RGB,
                             GL_UNSIGNED_BYTE,
                             imageData);
 
                (*textureWidth)  = (*imageWidth);
                (*textureHeight) = (*imageHeight);
 
                free(imageData);
        }
	else // texture is not a power of two. We need to resize it
	{
		
		int newWidth=(int)std::pow( 2.0, (int)(std::ceil(power_of_two_that_gives_correct_width)) );
		int newHeight=(int)std::pow( 2.0, (int)(std::ceil(power_of_two_that_gives_correct_height)) );
		
		//printf("Unsupported image size. Recommand values: %i %i\n",newWidth,newHeight);   
		
		GLubyte	*bitmapData=img->GetData();
		GLubyte        *alphaData=img->GetAlpha();
		GLubyte	*imageData;
		
		int old_bytesPerPixel = 3;
		int bytesPerPixel = img->HasAlpha() ?  4 : 3;
		
		int imageSize = newWidth * newHeight * bytesPerPixel;
		imageData=(GLubyte *)malloc(imageSize);
		
		int rev_val=(*imageHeight)-1;
		
		for(int y=0; y<newHeight; y++)
		{
			for(int x=0; x<newWidth; x++)
			{
				
				if( x<(*imageWidth) && y<(*imageHeight) ){
					imageData[(x+y*newWidth)*bytesPerPixel+0]=
					bitmapData[( x+(rev_val-y)*(*imageWidth))*old_bytesPerPixel + 0];
					
					imageData[(x+y*newWidth)*bytesPerPixel+1]=
						bitmapData[( x+(rev_val-y)*(*imageWidth))*old_bytesPerPixel + 1];
					
					imageData[(x+y*newWidth)*bytesPerPixel+2]=
						bitmapData[( x+(rev_val-y)*(*imageWidth))*old_bytesPerPixel + 2];
					
					if(bytesPerPixel==4) imageData[(x+y*newWidth)*bytesPerPixel+3]=
						alphaData[ x+(rev_val-y)*(*imageWidth) ];
					
				}
				else
				{
					
					imageData[(x+y*newWidth)*bytesPerPixel+0] = 0;
					imageData[(x+y*newWidth)*bytesPerPixel+1] = 0;
					imageData[(x+y*newWidth)*bytesPerPixel+2] = 0;
					if(bytesPerPixel==4) imageData[(x+y*newWidth)*bytesPerPixel+3] = 0;
				}
				
			}//next
		}//next
		
		
		glTexImage2D(GL_TEXTURE_2D,
					 0,
					 img->HasAlpha() ?  4 : 3,
					 newWidth,
					 newHeight,
					 0, 
					 img->HasAlpha() ?  GL_RGBA : GL_RGB, 
					 GL_UNSIGNED_BYTE,
					 imageData);
		
		(*textureWidth)=newWidth;
		(*textureHeight)=newHeight;
		
		free(imageData);
	}
	
	// set texture parameters as you wish
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // GL_LINEAR
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	return ID;
	
}
