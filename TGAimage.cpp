// TGAImage.cpp: implementation of the TGAimage class.
//
//////////////////////////////////////////////////////////////////////

#include "TGAImage.h"

#ifdef _WIN32
#  pragma warning (disable : 4996)
#endif

void TGAImage::save(char* filename) {
	FILE * fp = fopen(filename, "w");
	if (fp != NULL) {
		unsigned char header[18];
		header[0] = 0;		// size of ID field that follows 18 byte header (0 usually)
		header[1] = 0;		// type of colour map 0=none, 1=has palette
		header[2] = 2;		// type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed
		header[3] = 0;		// first colour map entry in palette
		header[4] = 0;
		header[5] = 0;		// number of colours in palette
		header[6] = 0;
		header[7] = 0;		// number of bits per palette entry 15,16,24,32

		header[8] = 0;		// image x origin
		header[9] = 0;
		header[10] = 0;		// image y origin
		header[11] = 0;
		header[12] = (unsigned char)((width & 0xFF) >> 0);	// image width in pixels low order bits
		header[13] = (unsigned char)((width & 0xFF00) >> 8);	// image width in pixels high order bits
		header[14] = (unsigned char)((height & 0xFF) >> 0);	// image height in pixels low order bits
		header[15] = (unsigned char)((height & 0xFF00) >> 8);	// image height in pixels high order bits
		header[16] = 32;	// image bits per pixel 8,16,24,32
		header[17] = 8;		// image descriptor bits (vh flip bits) I just copied '8' from Photoshop
		//switch R and B from RGBA to BGRA
		//the coming up loop may also require inverting the image on y
		int totallength = width*height*4;
		unsigned char * bgraData;
		bgraData = new unsigned char[totallength];
		for (int i = 0; i < totallength; i += 4) {
			bgraData[i+2] = data[i];
			bgraData[i+1] = data[i+1];
			bgraData[i+0] = data[i+2];
			bgraData[i+3] = data[i+3];
		}
		fwrite(header, sizeof(unsigned char), 18, fp);
		fwrite(bgraData, sizeof(unsigned char), totallength, fp);
		fclose(fp);
		delete bgraData;
	}
}

//fill the image data from the file filename
//only works on 24 bit TGA images
void TGAImage::open(char* filename){
	int totalLength;
	FILE *iFile;
	if (iFile = fopen(filename, "rb"))
	{
		unsigned char header[18];
		fread(&header, sizeof(unsigned char), 18, iFile);
		width = (header[13] << 8) + header[12];
		height = (header[15] << 8) + header[14];
		totalLength = width*height*3;
		data = new unsigned char[width*height*3]; //always stored internally as RGB 
		fseek(iFile, header[0], SEEK_CUR);
		fread(data, sizeof(unsigned char), totalLength, iFile);
		fclose(iFile);
		unsigned char temp;
		//switch R and B from BGR to RGB
		for (int i = 0; i < totalLength; i += 3) {
			temp = data[i];
			data[i] = data[i + 2];
			data[i + 2] = temp;
		}
	}
}
