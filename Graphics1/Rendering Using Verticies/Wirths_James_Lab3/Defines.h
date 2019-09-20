#pragma once

#include "stdafx.h"
#include <algorithm>
#include <Windows.h>
#include "RasterSurface.h"


#define RWIDTH 500
#define RHEIGHT 500

#pragma region Colors

#define RED 0xFFFF0000
#define GREEN 0xFF00FF00
#define BLUE 0xFF0000FF
#define WHITE 0xFFFFFFFF
#define CYAN 0xFF00FFFF
#define MAGENTA 0xFFFF00FF
#define BLACK 0

#pragma endregion

#define PI 3.14159

void PlotPixel(unsigned int currX, unsigned int currY, unsigned int* raster, unsigned int color);


struct VERTEX
{
	// Vertex Array
	float x, y, z, w;
	unsigned int color;
};



struct TRIANGLE
{
	VERTEX A;
	VERTEX B;
	VERTEX C;

	TRIANGLE(const VERTEX& _A,const VERTEX& _B,const VERTEX& _C)
	{
		A = _A;
		B = _B;
		C = _C;
	}
};

void ClearRaster(unsigned int* raster)
{
	for (unsigned int i = 0; i < RWIDTH * RHEIGHT; i++)
	{
		raster[i] = 0;
	}
}

float ImplicitLineEquation(int x1, int y1, int currX, int currY, int x2, int y2)// Midpoint ImplicitLineEquation Function
{
	return (float)(y1 - y2) * currX + (x2 - x1) * currY + x1 * y2 - y1 * x2;
}



unsigned int TwoDOneDConversion(unsigned int x, unsigned int y, unsigned int width)
{
	return y * width + x;
}

void PlotPixel(unsigned int currX, unsigned int currY, unsigned int* raster, unsigned int color)//Writes Line To Raster Screen
{
	if (currX > RWIDTH || currY > RHEIGHT || currX < 0 || currY < 0)
	{
		return;
	}
	raster[TwoDOneDConversion(currX, currY, RWIDTH)] = color;
}

