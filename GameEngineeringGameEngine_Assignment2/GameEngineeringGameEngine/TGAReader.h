#ifndef TGA_READER_H
#define TGA_READER_H

#include "StandardIncludes.h"

class Asset;

#pragma pack(push, 1) // enable memory packing
typedef struct {
	char IDLength; // the size of id field that follows 18 byte header (0 usually)
	char ColourMapType; // type ofd colour map 0=none, 1=has palette
	char DataTypeCode;	//Type of image 0=none , 1 = indexed, 2=rgb, 3=grey, +8=rle packed
	short ColourMapStart; //First colour map entry in palette
	short ColourMapLength; // number of colours in the palette
	char ColourMapDepth; // the image number of bits per palette entry 15,16,24,32
	short x_Origin; // the image x origin
	short y_Origin; // the image y origin
	short Width; // the width of the image in pixels
	short Height; // The height of the image in pixels
	char BitsPerPixel;	//The size of each colour value, - ex - 8, 16, 24, 32 bpp
	char ImageDescriptor; // Imaged descriptor byte
} TGAHeader;

#pragma pack(pop) // disable memory packing
typedef struct {
	short Width;
	short Height;
	short BitsPerPixel;
	short DataOffset;
} ImageInfo;

class TGAReader {
public:
	TGAReader();
	virtual ~TGAReader();

	void ProcessAsset(Asset* _rawTGA, ImageInfo* _imageInfo);
	Asset* LoadTGAFromFile(string _file, ImageInfo* _imageInfo);

private:
	TGAHeader m_header;
	Asset* m_data;
};

#endif // !TGA_READER_H
