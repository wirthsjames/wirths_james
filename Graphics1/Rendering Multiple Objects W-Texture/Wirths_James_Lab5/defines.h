#pragma once

#define _USE_MATH_DEFINES
#include "stdafx.h"
#include "StoneHenge.h"
#include "StoneHenge_Texture.h"
#include <algorithm>
#include <Windows.h>
#include <cmath>
#include "RasterSurface.h"
#include "flower.h"
#include <assert.h>

#define uint unsigned int

#define RWIDTH	750
#define RHEIGHT 490
#define RSIZE (RWIDTH * RHEIGHT)
#define NEARPLANE 0.1f
#define FARPLANE 1000.0f
#define FOV 90.0f

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


unsigned int pixels[RSIZE];
float zBuffer[RSIZE];



struct VECTOR
{
	float x, y, z;
	
};

struct VERTEX
{
	// Vertex Array
	float x, y, z, w = 1;
	unsigned int color;
	float u, v;
	VECTOR normal;
};

VERTEX StoneHenge[1457];
VERTEX LightDir;

struct TRIANGLE
{
	VERTEX A;
	VERTEX B;
	VERTEX C;
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
