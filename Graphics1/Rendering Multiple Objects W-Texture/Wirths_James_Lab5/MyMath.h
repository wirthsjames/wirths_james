#pragma once

#include "defines.h"

MATRIX_4X4 CreateTranslationMatrix(float x, float y, float z);
VERTEX CartesianToScreen(VERTEX cartesianVertex);
MATRIX_4X4 CreateYRotationMatrix(float theta);
MATRIX_4X4 FastInverse(MATRIX_4X4 M);
MATRIX_4X4 MuliplyMatrixByMatrix(MATRIX_4X4 A, MATRIX_4X4 B);
VERTEX MultiplyVertexByMatrix_4X4(VERTEX V, MATRIX_4X4 M);
VERTEX MultiplyVertexByMatrix_3X3(VERTEX V, MATRIX_3X3 M);
MATRIX_4X4 MatrixTranspose_3X3_Of_4X4(MATRIX_4X4 M);

unsigned int ColorConverter(unsigned int pixel) //Converts BGRA -> ARGB
{
	unsigned int newColor = 0;

	//// Extracts 4 channels to 1 byte chars
	unsigned int alpha = (pixel & 0x000000FF) << 24;
	unsigned int red = (pixel & 0x0000FF00) << 8;
	unsigned int green = (pixel & 0x00FF0000) >> 8;
	unsigned int blue = (pixel & 0xFF000000) >> 24;

	return newColor = alpha | red | blue | green | blue;
}

unsigned int PixelLerping(unsigned int pixelA, unsigned int pixelB, float ratio)
{
	//B is raster
	//A is sourcePixel
	//(B - A) * alpha/255 + A

	int aAlpha = ((pixelA & 0xFF000000) >> 24);
	int bAlpha = ((pixelB & 0xFF000000) >> 24);

	int aRed = ((pixelA & 0x00FF0000) >> 16);
	int bRed = ((pixelB & 0x00FF0000) >> 16);

	int aGreen = ((pixelA & 0x0000FF00) >> 8);
	int bGreen = ((pixelB & 0x0000FF00) >> 8);

	int aBlue = (pixelA & 0x000000FF);
	int bBlue = (pixelB & 0x000000FF);

	float r = ratio;

	unsigned int LerpedAlpha = (((bAlpha - aAlpha) * r) + aAlpha);
	LerpedAlpha <<= 24;

	unsigned int LerpedRed = (((bRed - aRed) * r) + aRed);
	LerpedRed <<= 16;

	unsigned int LerpedGreen = (((bGreen - aGreen) * r) + aGreen);
	LerpedGreen <<= 8;

	unsigned int LerpedBlue = (((bBlue - aBlue) * r) + aBlue);


	unsigned int answer = LerpedAlpha | LerpedRed | LerpedGreen | LerpedBlue;

	return answer;

}

void ClearRaster(unsigned int* raster)
{
	for (unsigned int i = 0; i < RWIDTH * RHEIGHT; i++)
	{
		raster[i] = 0xFF6da5fe;
	}
}

float ImplicitLineEquation(int x1, int y1, int currX, int currY, int x2, int y2)
{
	return (float)(y1 - y2) * currX + (x2 - x1) * currY + x1 * y2 - y1 * x2;
}

unsigned int TwoDOneDConversion(unsigned int x, unsigned int y, unsigned int width)
{
	return y * width + x;
}

void PlotPixel(unsigned int currX, unsigned int currY, float depth, unsigned int* raster, unsigned int color)//Writes Line To Raster Screen
{
	if (currX >= RWIDTH || currY >= RHEIGHT || currX < 0 || currY < 0)
	{
		return;
	}
	if (depth < zBuffer[TwoDOneDConversion(currX, currY, RWIDTH)])
	{
		zBuffer[TwoDOneDConversion(currX, currY, RWIDTH)] = depth;
		raster[TwoDOneDConversion(currX, currY, RWIDTH)] = color;
	}
}

void PlotPixelOne(unsigned int currX, unsigned int currY, unsigned int* raster, unsigned int color)
{
	if (currX >= RWIDTH || currY >= RHEIGHT || currX < 0 || currY < 0)
	{
		return;
	}
	raster[TwoDOneDConversion(currX, currY, RWIDTH)] = color;
}

MATRIX_4X4 CreateTranslationMatrix(float x, float y, float z)
{
	MATRIX_4X4 temp =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		x,y,z,1
	};

	return temp;
}

VERTEX CartesianToScreen(VERTEX cartesianVertex)
{
	VERTEX T = cartesianVertex;
	T.x = ((T.x + 1) * (RWIDTH / 2));
	T.y = ((1 - T.y) * (RHEIGHT / 2));

	return T;
}

MATRIX_4X4 CreateYRotationMatrix(float theta)
{
	MATRIX_4X4 Temp;

	Temp._e11 = cosf(theta);  Temp._e12 = 0; Temp._e13 = sinf(theta); Temp._e14 = 0;
	Temp._e21 = 0;			  Temp._e22 = 1; Temp._e23 = 0;			  Temp._e24 = 0;
	Temp._e31 = -sinf(theta); Temp._e32 = 0; Temp._e33 = cosf(theta); Temp._e34 = 0;
	Temp._e41 = 0;			  Temp._e42 = 0; Temp._e43 = 0;			  Temp._e44 = 1;

	return Temp;
}

MATRIX_4X4 CreateXRotationMatrix(float theta)
{
	MATRIX_4X4 Temp;

	Temp._e11 = 1;	Temp._e12 = 0;			 Temp._e13 = 0;				 Temp._e14 = 0;
	Temp._e21 = 0;	Temp._e22 = cosf(theta); Temp._e23 = -sinf(theta);	 Temp._e24 = 0;
	Temp._e31 = 0;  Temp._e32 = sinf(theta); Temp._e33 = cosf(theta);   Temp._e34 = 0;
	Temp._e41 = 0;  Temp._e42 = 0;			 Temp._e43 = 0;			     Temp._e44 = 1;

	return Temp;
}

VERTEX VertexInverse(VERTEX V)
{
	V.x *= -1;
	V.y *= -1;
	V.z *= -1;
	V.w *= -1;
	return V;
}

MATRIX_4X4 PerspectiveProjectionMatrix(float nearPlane, float farPlane, float vFov, float aR)
{
	float xScale;
	float yScale;
	float zNear;
	float zFar;

	float cosOfFov = cosf((0.5 * vFov) * (M_PI / 180.0f));
	float sinOfFov = sinf((0.5 * vFov) * (M_PI / 180.0f));


	yScale = cosOfFov / sinOfFov;
	xScale = yScale * RHEIGHT / RWIDTH;

	if (nearPlane != 0)
	{
		zNear = nearPlane;
	}
	if (farPlane > nearPlane)
	{
		zFar = farPlane;
	}

	MATRIX_4X4 t = {
		xScale,0,	  0,                           0,
		0,	   yScale,0,                           0,
		0,     0,   zFar / (zFar - zNear),           1,
		0,	   0, -(zFar * zNear) / (zFar - zNear),0
	};

	return t;
}

void PerspectiveDivide(VERTEX& multiplyMe)
{
	multiplyMe.x = multiplyMe.x / multiplyMe.w;
	multiplyMe.y = multiplyMe.y / multiplyMe.w;
	multiplyMe.z = multiplyMe.z / multiplyMe.w;
	//multiplyMe.w = multiplyMe.w / multiplyMe.w;
}

MATRIX_4X4 CreateIdentityMatrix()
{
	MATRIX_4X4 temp = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	return temp;
}

MATRIX_4X4 FastInverse(MATRIX_4X4 M)
{
	MATRIX_4X4 M1 = M;

	float x = M1._e41;
	float y = M1._e42;
	float z = M1._e43;

	MATRIX_3X3 M2 = {
		M1._e11,M1._e12,M1._e13,
		M1._e21,M1._e22,M1._e23,
		M1._e31,M1._e32,M1._e33
	};

	MATRIX_3X3 M2Transverse = {
		M2._e11, M2._e21,M2._e31,
		M2._e12, M2._e22,M2._e32,
		M2._e13, M2._e23,M2._e33
	};

	//Hard Coded Vertex By Matrix Multiplication
	float tempX = (x * M2Transverse._e11) + (y * M2Transverse._e21) + (z * M2Transverse._e31);
	float tempY = (x * M2Transverse._e12) + (y * M2Transverse._e22) + (z * M2Transverse._e32);
	float tempZ = (x * M2Transverse._e13) + (y * M2Transverse._e23) + (z * M2Transverse._e33);

	tempX *= -1;
	tempY *= -1;
	tempZ *= -1;

	M1._e11 = M2Transverse._e11; M1._e12 = M2Transverse._e12; M1._e13 = M2Transverse._e13;
	M1._e21 = M2Transverse._e21; M1._e22 = M2Transverse._e22; M1._e23 = M2Transverse._e23;
	M1._e31 = M2Transverse._e31; M1._e32 = M2Transverse._e32; M1._e33 = M2Transverse._e33;
	M1._e41 = tempX;			 M1._e42 = tempY;			  M1._e43 = tempZ;

	return M1;
}

MATRIX_4X4 MuliplyMatrixByMatrix(MATRIX_4X4 m, MATRIX_4X4 n)
{
	MATRIX_4X4 ans;
	ans._e11 = ((m._e11 * n._e11) + (m._e12 * n._e21) + (m._e13 * n._e31) + (m._e14 * n._e41));
	ans._e12 = ((m._e11 * n._e12) + (m._e12 * n._e22) + (m._e13 * n._e32) + (m._e14 * n._e42));
	ans._e13 = ((m._e11 * n._e13) + (m._e12 * n._e23) + (m._e13 * n._e33) + (m._e14 * n._e43));
	ans._e14 = ((m._e11 * n._e14) + (m._e12 * n._e24) + (m._e13 * n._e34) + (m._e14 * n._e44));

	ans._e21 = ((m._e21 * n._e11) + (m._e22 * n._e21) + (m._e23 * n._e31) + (m._e24 * n._e41));
	ans._e22 = ((m._e21 * n._e12) + (m._e22 * n._e22) + (m._e23 * n._e32) + (m._e24 * n._e42));
	ans._e23 = ((m._e21 * n._e13) + (m._e22 * n._e23) + (m._e23 * n._e33) + (m._e24 * n._e43));
	ans._e24 = ((m._e21 * n._e14) + (m._e22 * n._e24) + (m._e23 * n._e34) + (m._e24 * n._e44));

	ans._e31 = ((m._e31 * n._e11) + (m._e32 * n._e21) + (m._e33 * n._e31) + (m._e34 * n._e41));
	ans._e32 = ((m._e31 * n._e12) + (m._e32 * n._e22) + (m._e33 * n._e32) + (m._e34 * n._e42));
	ans._e33 = ((m._e31 * n._e13) + (m._e32 * n._e23) + (m._e33 * n._e33) + (m._e34 * n._e43));
	ans._e34 = ((m._e31 * n._e14) + (m._e32 * n._e24) + (m._e33 * n._e34) + (m._e34 * n._e44));

	ans._e41 = ((m._e41 * n._e11) + (m._e42 * n._e21) + (m._e43 * n._e31) + (m._e44 * n._e41));
	ans._e42 = ((m._e41 * n._e12) + (m._e42 * n._e22) + (m._e43 * n._e32) + (m._e44 * n._e42));
	ans._e43 = ((m._e41 * n._e13) + (m._e42 * n._e23) + (m._e43 * n._e33) + (m._e44 * n._e43));
	ans._e44 = ((m._e41 * n._e14) + (m._e42 * n._e24) + (m._e43 * n._e34) + (m._e44 * n._e44));

	return ans;
}

VERTEX MultiplyVertexByMatrix_4X4(VERTEX V, MATRIX_4X4 M)
{
	VERTEX temp = V;

	temp.x = (V.x * M._e11) + (V.y * M._e21) + (V.z * M._e31) + (V.w * M._e41);
	temp.y = (V.x * M._e12) + (V.y * M._e22) + (V.z * M._e32) + (V.w * M._e42);
	temp.z = (V.x * M._e13) + (V.y * M._e23) + (V.z * M._e33) + (V.w * M._e43);
	temp.w = (V.x * M._e14) + (V.y * M._e24) + (V.z * M._e34) + (V.w * M._e44);

	return temp;
}

VERTEX MultiplyVertexByMatrix_3X3(VERTEX V, MATRIX_3X3 M)
{
	VERTEX temp = V;

	temp.x = (V.x * M._e11) + (V.y * M._e21) + (V.z * M._e31);
	temp.y = (V.x * M._e12) + (V.y * M._e22) + (V.z * M._e32);
	temp.z = (V.x * M._e13) + (V.y * M._e23) + (V.z * M._e33);


	return temp;
}

MATRIX_4X4 MatrixTranspose_3X3_Of_4X4(MATRIX_4X4 M)
{
	MATRIX_4X4 temp;

	temp._e11 = M._e11; temp._e12 = M._e21; temp._e13 = M._e31; temp._e14 = M._e14;
	temp._e21 = M._e12; temp._e22 = M._e22; temp._e23 = M._e32; temp._e24 = M._e24;
	temp._e31 = M._e13; temp._e32 = M._e23; temp._e33 = M._e33; temp._e34 = M._e34;
	temp._e41 = M._e41; temp._e42 = M._e42; temp._e43 = M._e43; temp._e44 = M._e44;

	return temp;
}

float GetRadiansOverTime(float &theta)
{
	theta += 1;
	float t = theta * (M_PI / 180.0f);
	return t;

}

float BasicLerp(float A, float B, float ratio)
{
	return ((B - A) * ratio) + A;
}

VERTEX VertexLerp(VERTEX& A, VERTEX& B, float ratio)
{
	VERTEX temp;

	temp.x = ((A.x - B.x) * ratio) + A.x;
	temp.y = ((A.y - B.y) * ratio) + A.y;
	temp.z = ((A.z - B.z) * ratio) + A.z;
	temp.w = ((A.w - B.w) * ratio) + A.w;
	temp.u = ((A.u - B.u) * ratio) + A.u;
	temp.v = ((A.v - B.v) * ratio) + A.v;
	return temp;
}

float Vector_Length(VECTOR v)
{
	// TODO LAB 1: Replace with your implementation.
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

VECTOR Vector_Normalize(VECTOR v)
{
	// TODO LAB 1: Replace with your implementation.
	float len = Vector_Length(v);
	v.x /= len;
	v.y /= len;
	v.z /= len;

	return v;
}

VECTOR Vector_Cross(VECTOR v, VECTOR w)
{
	// TODO LAB 1: Replace with your implementation.
	float x, y, z;
	x = (v.y * w.z) - (v.z * w.y);
	y = (v.z * w.x) - (v.x * w.z);
	z = (v.x * w.y) - (v.y * w.x);

	v.x = x;
	v.y = y;
	v.z = z;

	return v;
}

float Vector_Dot(VECTOR v, VECTOR w)
{
	// TODO LAB 1: Replace with your implementation.	
	return  (v.x * w.x) + (v.y * w.y) + (v.z * w.z);
}

uint AddClamp(uint A, uint B)
{
	uint value = A + B;
	if (value > 255)
	{
		return 255;
	}
	return value;
}

float Clamp(float value)
{
	if (value < 0)
	{
		return 0;
	}
	else if (value >= 1)
	{
		return 1;
	}
	return value;
}

unsigned int AddColors(unsigned int colorA, unsigned int colorB)
{
	uint aAlpha = (colorA & 0xFF000000) >> 24;
	uint aRed = (colorA & 0x00FF0000) >> 16;
	uint aGreen = (colorA & 0x0000FF00) >> 8;
	uint aBlue = (colorA & 0x000000FF);

	uint bAlpha = (colorB & 0xFF000000) >> 24;
	uint bRed = (colorB & 0x00FF0000) >> 16;
	uint bGreen = (colorB & 0x0000FF00) >> 8;
	uint bBlue = (colorB & 0x000000FF);

	uint alpha = AddClamp(aAlpha, bAlpha);
	uint red = AddClamp(aRed, bRed);
	uint green = AddClamp(aGreen, bGreen);
	uint blue = AddClamp(aBlue, bBlue);

	return alpha | red | green | blue;
}

unsigned int ModulateColors(uint A, float ratio)
{
	uint aAlpha	= ((A & 0xFF000000) >> 24);
	uint aRed	= ((A & 0x00FF0000) >> 16);
	uint aGreen = ((A & 0x0000FF00) >> 8);
	uint aBlue	= (A & 0x000000FF);

	aAlpha *= ratio;
	aRed   *= ratio;
	aGreen *= ratio;
	aBlue  *= ratio;

	return aAlpha | aRed | aGreen | aBlue;
}

unsigned int MultiplyColors(uint A, uint B)
{
	float aAlpha =	((A & 0xFF000000) >> 24) / 255.0f;
	float aRed   =	((A & 0x00FF0000) >> 16) / 255.05;
	float aGreen =	((A & 0x0000FF00) >> 8) / 255.0f;
	float aBlue  =	( A & 0x000000FF) / 255.0f;

	float bAlpha =  ((B & 0xFF000000) >> 24) / 255.0f;
	float bRed   =	((B & 0x00FF0000) >> 16) / 255.0f;
	float bGreen =  ((B & 0x0000FF00) >> 8) / 255.0f;
	float bBlue  =	( B & 0x000000FF) / 255.0f;

	float Al = Clamp(aAlpha * bAlpha);
	float Re = Clamp(aRed * bAlpha);
	float Gr = Clamp(aGreen * bAlpha);
	float Bl = Clamp(aBlue * bAlpha);

	Al *= 255;
	Re *= 255;
	Gr *= 255;
	Bl *= 255;

	unsigned int fA = ((unsigned int)Al) << 24;
	unsigned int fR = ((unsigned int)Re) << 16;
	unsigned int fG = ((unsigned int)Gr) << 8;
	unsigned int fB = ((unsigned int)Bl);

	return fA | fR | fG | fB;
}

float RandFloat(float min, float max)
{
	float r = (float)rand() / RAND_MAX;

	return BasicLerp(min, max, r);
}