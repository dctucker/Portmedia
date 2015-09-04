/*
 *  textures.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-05-05.
 *  Copyright 2010 UNCW. All rights reserved.
 *    mostly copied from wxWiki http://wiki.wxwidgets.org/Using_wxImage_to_load_textures_for_OpenGL
 */

/*
#ifndef _glpane_
#define _glpane_
 
#include "wx/wx.h"
#include "wx/glcanvas.h"
class BasicGLPane : public wxGLCanvas
{
    
public:
	BasicGLPane(wxFrame* parent, int* args);
    
	void resized(wxSizeEvent& evt);
    
	int getWidth();
	int getHeight();
    
	void render(wxPaintEvent& evt);
	void prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
	void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
    
	// events
	void mouseMoved(wxMouseEvent& event);
	void mouseDown(wxMouseEvent& event);
	void mouseWheelMoved(wxMouseEvent& event);
	void mouseReleased(wxMouseEvent& event);
	void rightClick(wxMouseEvent& event);
	void mouseLeftWindow(wxMouseEvent& event);
	void keyPressed(wxKeyEvent& event);
	void keyReleased(wxKeyEvent& event);
    
	DECLARE_EVENT_TABLE()
};
 
#endif 
*/
#ifndef _image_
#define _image_
 
// include OpenGL
#ifdef __WXMAC__
#include "OpenGL/gl.h"
#else
#include <GL/gl.h>
#endif
 
#include "wx/wx.h"
 
class Image
{
    GLuint* ID;
    
public:
 
	/*
	 * it is preferable to use textures that are a power of two. this loader will automatically
	 * resize texture to be a power of two, filling the remaining areas with black.
	 * width/height are the width of the actual loaded image.
	 * textureWidth/Height are the total width of the texture, including black filling.
	 * tex_coord_x/y are the texture coord parameter you must give OpenGL when rendering
	 * to get only the image, without the black filling.
	 */
    int width, height, textureWidth, textureHeight;
    float tex_coord_x, tex_coord_y;
    
    GLuint* getID();
    Image();
    Image(wxString path);
    ~Image();
    void load(wxString path);
        
};
 
 
#endif 

#ifndef _drawable_
#define _drawable_
  
class Drawable
{
public:
    int x,y, angle, hotspotX, hotspotY;
    float xscale, yscale;
    Image* image;
    bool xflip, yflip;
    
    Drawable(Image* image=(Image*)0);
    void setFlip(bool x, bool y);
    void move(int x, int y);
    void setHotspot(int x, int y);
    void scale(float x, float y);
    void scale(float k);
    void setImage(Image* image);
    void render();
    void rotate(int angle);
    
};
 
 
#endif 



#ifndef _image_loader
#define _image_loader
 
#ifdef __WXMAC__
#include "OpenGL/gl.h"
#else
#include <GL/gl.h>
#endif
#include "wx/wx.h"
	
GLuint* loadImage(wxString path, int* imageWidth, int* imageHeight, int* textureWidth, int* textureHeight);
 
#endif 
