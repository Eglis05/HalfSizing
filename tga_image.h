#ifndef TGA_IMAGE_H_
#define TGA_IMAGE_H_

struct TGAIMAGE {
	// Length of id string
	char	idLength;

	// Image storage info
	char	colourMapType;
	char	imageType;

	// Colour Map
	short	firstEntry;
	short	numEntries;
	char	bitsPerEntry;

	// Image description
	short	xOrigin;
	short	yOrigin;
	short	width;
	short	height;
	char	bitsPerPixel;
	char	descriptor;

	//Extra by me
	int lefttoread;
	long long int imageDataSize;
};
#endif