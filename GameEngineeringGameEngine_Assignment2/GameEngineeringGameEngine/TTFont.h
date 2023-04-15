#ifndef TTFONT_H
#define TTFONT_H

#include "StandardIncludes.h"

class Renderer;

class TTFont {

public :
	TTFont();
	virtual ~TTFont();

	void Initialize(int _pointSize);
	void Write(SDL_Renderer * _renderer, const char * _text, SDL_Color _color, SDL_Point _pos);
	void Shutdown();

private :
	TTF_Font * m_font;
	SDL_Rect destRect;
	

};

#endif // !TTFONT_H
