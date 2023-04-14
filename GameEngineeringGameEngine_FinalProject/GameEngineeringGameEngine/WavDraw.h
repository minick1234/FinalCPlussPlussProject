#ifndef WAV_DRAW_H
#define WAV_DRAW_H


#include "StandardIncludes.h"

class Renderer;
class Asset;

#pragma pack(push,1)
typedef struct {
	char RIFFHeader[4];
	unsigned int WavSize;
	char WavHeader[4];


	char FMTHeader[4];
	unsigned int FMTChunkSize;
	unsigned short AudioFormat;
	unsigned short NumChannels;
	unsigned int SampleRate;
	unsigned int ByteRate;
	unsigned short SampleAlignment;
	unsigned short BitDepth;

	char DataHeader[4];
	unsigned int DataBytes;

}WAVHeader;

#pragma pack(pop)


class WavDraw {
public:
	WavDraw();
	virtual ~WavDraw();


	void DrawWave(Asset * _rawWave, Renderer * _renderer, float _yZoom);
	
private :
	bool CompareFileExt(string source, string ext);
	void IntegrityChecks(Asset * _rawWav);

	WAVHeader m_header;
	Asset* m_data;
	Point m_wSize;
	int m_stepSize;
	short* m_wavData;
	int m_xPos;
	Point m_prevLeft;
	Point m_prevRight;

};

#endif // !WAV_DRAW_H



