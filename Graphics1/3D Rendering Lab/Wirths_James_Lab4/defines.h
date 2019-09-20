#pragma once

#define _USE_MATH_DEFINES
#include "stdafx.h"
#include <algorithm>
#include <Windows.h>
#include <cmath>
#include "RasterSurface.h"


#define RWIDTH	500
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
	float x, y, z, w = 1;
	unsigned int color;
};

struct TRIANGLE
{
	VERTEX A;
	VERTEX B;
	VERTEX C;

	TRIANGLE(const VERTEX& _A, const VERTEX& _B, const VERTEX& _C)
	{
		A = _A;
		B = _B;
		C = _C;
	}
};

struct MATRIX_3X3
{
	// Individual parts of matrix

	float _e11, _e12, _e13;
	float _e21, _e22, _e23;
	float _e31, _e32, _e33;

};

struct MATRIX_4X4
{
	// Individual parts of matrix

	float _e11, _e12, _e13, _e14;
	float _e21, _e22, _e23, _e24;
	float _e31, _e32, _e33, _e34;
	float _e41, _e42, _e43, _e44;

};
