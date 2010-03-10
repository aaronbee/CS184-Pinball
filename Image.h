// Image.h: interface for the Image class.
//
// Base class for images stored in RGBA order
// When writting and reading, R and G often have to be switched
// This switching is cheap enough that it's worth the standardization
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGE_H__A449F1F2_5D0D_4556_8808_0451CB0E1A35__INCLUDED_)
#define AFX_IMAGE_H__A449F1F2_5D0D_4556_8808_0451CB0E1A35__INCLUDED_


#include <stdio.h>
#include "GLUT/glut.h"

class Image  
{
protected:
	int width;
	int height;
	//kept in rgba
	unsigned char* data;
	unsigned int textureID;

public:
	Image();
	virtual ~Image();

	int getWidth();
	int getHeight();
	unsigned char* getData();

	//These must be implemented for each type of image (i.e. TGA, DDS, JPG...)
	//Save the image to filename.  Requires generating proper header
	virtual void save(char* filename) {};
	//fill the image data from the GL framebuffer using glReadPixels
	virtual void open() {};
	//fill the image data from the file filename
	virtual void open(char* filename) {};
	virtual void updateTexture();


};

#endif // !defined(AFX_IMAGE_H__A449F1F2_5D0D_4556_8808_0451CB0E1A35__INCLUDED_)
