#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>

#include "tga_image.h"

using namespace std;

void resizeTGA(const string& fileName, const string& halfsizeName) {

    TGAIMAGE tgaimage;

    cout << "Reading \"" << fileName << "\"..." << std::endl;
    ifstream tgaFile(fileName, std::ios::in | std::ios::binary);

    if (!tgaFile.is_open()) {
        cout << "Error opening " << fileName << std::endl;
        exit(1);
    }

    tgaFile.seekg(0, std::ios::end);
    int endPosition = (int)tgaFile.tellg();
    tgaFile.seekg(0, std::ios::beg);

    //Reading Header

    tgaFile.read(	&tgaimage.idLength,	sizeof(tgaimage.idLength));
    tgaFile.read(	&tgaimage.colourMapType,sizeof(tgaimage.colourMapType));
    tgaFile.read(	&tgaimage.imageType,	sizeof(tgaimage.imageType));
    tgaFile.read((char*)(&tgaimage.firstEntry),	sizeof(tgaimage.firstEntry));
    tgaFile.read((char*)(&tgaimage.numEntries),	sizeof(tgaimage.numEntries));
    tgaFile.read(	&tgaimage.bitsPerEntry,	sizeof(tgaimage.bitsPerEntry));
    tgaFile.read((char*)(&tgaimage.xOrigin),	sizeof(tgaimage.xOrigin));
    tgaFile.read((char*)(&tgaimage.yOrigin),	sizeof(tgaimage.yOrigin));
    tgaFile.read((char*)(&tgaimage.width),	sizeof(tgaimage.width));
    tgaFile.read((char*)(&tgaimage.height),	sizeof(tgaimage.height));
    tgaFile.read(	&tgaimage.bitsPerPixel,	sizeof(tgaimage.bitsPerPixel));
    tgaFile.read(	&tgaimage.descriptor,	sizeof(tgaimage.descriptor));

    //Checking Format
    if (tgaimage.imageType != 2 && tgaimage.imageType != 0){
        std::cout << "TGA Format Unsupported" << std::endl;
        exit(1);
    }

    char *IDString = new char[tgaimage.idLength];
    tgaFile.read(IDString, tgaimage.idLength);


    int colourMapSize = tgaimage.colourMapType * tgaimage.numEntries;
    char *ColourMap   = new char[colourMapSize];
    tgaFile.read(ColourMap, colourMapSize);


    int bytesPerPixel      = (tgaimage.bitsPerPixel / 8);
    tgaimage.imageDataSize = tgaimage.width * tgaimage.height * bytesPerPixel;	
    char *pixelData	   = new char[tgaimage.imageDataSize];

    //Reading the image data

    tgaFile.read(pixelData, tgaimage.imageDataSize);

    tgaimage.lefttoread  = endPosition - (int)tgaFile.tellg();
    char *lefttoreadData = new char[tgaimage.lefttoread];

    tgaFile.read(lefttoreadData, tgaimage.lefttoread);

    cout << "Done." << endl;
    tgaFile.close();

    cout << "Original size: " << tgaimage.width << "x" << tgaimage.height <<          endl;

    //Due to rounding error by dividing by two, things might go wrong
    short oldwidth   = tgaimage.width;

    tgaimage.width  /= 2;
    tgaimage.height /= 2;

    cout << "Resizing to: "   << tgaimage.width << "x" << tgaimage.height << "..." << endl;

    tgaimage.imageDataSize = tgaimage.width * tgaimage.height * bytesPerPixel;

    for (int i = 0; i < tgaimage.height; i++) {
        for (int j = 0; j < tgaimage.width; j++){
            for (int m = 0; m < bytesPerPixel; m++){
                //this is the position of the pixel we are writing and the m 
                //part depends on the byte of the pixel we are writing
                int position = (i * tgaimage.width + j) * bytesPerPixel + m;

                //the below data is used for easier understanding of the code
                int x[4]  = { 0 , 1 , 0 , 1 };
                char y[4] = { 0 , 0 , 1 , 1 };
                char pixelcolors[4];

                //here we calculate the byte of each of the pixels
                for (int k = 0; k < 4; k++){
                    int oldposition = ((2 * i + x[k]) * oldwidth + 2 * j + y[k]) * bytesPerPixel + m;
                    pixelcolors[k] = pixelData[oldposition];
                }

                int sum = 0;
                for (int k = 0; k < 4; k++) {
                    //we need to turn it into int to add it up normally
                    //or else if it is greater than 128 it will be counted as
                    //a negative number
                    for (int x = 1; x < 256; x *= 2)
                        if (x & pixelcolors[k])
                            sum += x;
                }
                sum /= 4;
                
                pixelData[position] = (char)sum;
            }
        }
    }

    cout << "Done." << endl;
    cout << "Saving \"" << halfsizeName << "\"..." << std::endl;
    ofstream halftgaFile(halfsizeName, std::ios::out | std::ios::binary);

    halftgaFile.write(	     &tgaimage.idLength,     sizeof(tgaimage.idLength));
    halftgaFile.write(	     &tgaimage.colourMapType,sizeof(tgaimage.colourMapType));
    halftgaFile.write(	     &tgaimage.imageType,    sizeof(tgaimage.imageType));
    halftgaFile.write((char*)(&tgaimage.firstEntry), sizeof(tgaimage.firstEntry));
    halftgaFile.write((char*)(&tgaimage.numEntries), sizeof(tgaimage.numEntries));
    halftgaFile.write(	     &tgaimage.bitsPerEntry, sizeof(tgaimage.bitsPerEntry));
    halftgaFile.write((char*)(&tgaimage.xOrigin),    sizeof(tgaimage.xOrigin));
    halftgaFile.write((char*)(&tgaimage.yOrigin),    sizeof(tgaimage.yOrigin));
    halftgaFile.write((char*)(&tgaimage.width),	     sizeof(tgaimage.width));
    halftgaFile.write((char*)(&tgaimage.height),     sizeof(tgaimage.height));
    halftgaFile.write(	     &tgaimage.bitsPerPixel, sizeof(tgaimage.bitsPerPixel));
    halftgaFile.write(	     &tgaimage.descriptor,   sizeof(tgaimage.descriptor));

    halftgaFile.write(IDString,       tgaimage.idLength);
    halftgaFile.write(ColourMap,      colourMapSize);
    halftgaFile.write(pixelData,      tgaimage.imageDataSize);
    halftgaFile.write(lefttoreadData, tgaimage.lefttoread);
    cout << "Done." << endl;

    free(IDString);
    free(ColourMap);
    free(pixelData);
    free(lefttoreadData);
}

int main(int argc, char *argv[]){
    if (argc != 3){
        cout << "ERROR ON INPUT!" << endl;
        exit(1);
    }
    resizeTGA(argv[1],argv[2]);
    return 0;
}
