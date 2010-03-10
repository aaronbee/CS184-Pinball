// Image.cpp: implementation of the Image class.
//
//////////////////////////////////////////////////////////////////////

#include "Image.h"

Image::Image() 
:width(0), height(0), data(NULL) 
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	// We'll use clamp to edge as the default texture wrap mode for all tex types
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Image::~Image() {
		if (data != NULL) {
			delete data;
			data = NULL;
			width = 0;
			height = 0;
		}
	}

int Image::getWidth() {return width;}
int Image::getHeight() {return height;}
unsigned char* Image::getData() {return data;}

void Image::updateTexture()
{
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
}
