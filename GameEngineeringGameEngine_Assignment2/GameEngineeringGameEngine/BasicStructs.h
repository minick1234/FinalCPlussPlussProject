#ifndef BASIC_STRUCTS_H
#define BASIC_STRUCTS_H

#ifndef _WIN32
#include <Windows.h>

#else 
typedef unsigned char byte;

#endif // !_WIN32

struct Color {
	Color(byte r, byte g, byte b, byte a) {
		R = r;
		G = g;
		B = b;
		A = a;
	}

	byte R;
	byte G;
	byte B;
	byte A;
};

struct Point {
	Point(float _x, float _y) {
		X = _x;
		Y = _y;
	}

	int ReturnPointXAsPixelCoord() {
		return (int)X;
	}

	int ReturnPointYAsPixelCoord() {
		return (int)Y;
	}

	float X;
	float Y;
};

struct Rect {
	Rect(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
		X1 = x1;
		X2 = x2;
		Y1 = y1;
		Y2 = y2;
	}

	unsigned int X1;
	unsigned int Y1;
	unsigned int X2;
	unsigned int Y2;
};

#endif // !BASIC_STRUCTS_H
