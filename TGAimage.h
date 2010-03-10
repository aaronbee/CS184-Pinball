// TGAImage.h: interface for the TGAimage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGAIMAGE_H__3A4BBFA2_A1A9_4E72_8C0A_E6DF70F30F95__INCLUDED_)
#define AFX_TGAIMAGE_H__3A4BBFA2_A1A9_4E72_8C0A_E6DF70F30F95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Image.h"

class TGAImage : public Image
{

public:
	//These must be implemented for each type of image (i.e. TGA, DDS, JPG...)
	//Save the image to filename.  Requires generating proper header
	void save(char* filename);
	//fill the image data from the file filename
	void open(char* filename);
	void open() {Image::open();};
};

#endif // !defined(AFX_TGAIMAGE_H__3A4BBFA2_A1A9_4E72_8C0A_E6DF70F30F95__INCLUDED_)
