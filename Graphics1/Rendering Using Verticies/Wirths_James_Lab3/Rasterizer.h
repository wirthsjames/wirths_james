#pragma once

#include "Shaders.h"

void FillTriangle(TRIANGLE tri, unsigned int* raster);

void DrawLine(const VERTEX &start, const VERTEX &end, unsigned int* raster)
{

	VERTEX copy_start = start;
	VERTEX copy_end = end;

	if (VertexShader)
	{
		VertexShader(copy_start);
		VertexShader(copy_end);
	}

	copy_start = CartesianToScreen(copy_start);
	copy_end = CartesianToScreen(copy_end);

	unsigned int deltaX = abs((float)copy_end.x - (float)copy_start.x);
	unsigned int deltaY = abs((float)copy_end.y - (float)copy_start.y);

	float longest = max(deltaX, deltaY);

	for (float i = 0; i < longest; i++)
	{
		float ratio = (float)i / longest;
		unsigned int cX = (int)((copy_end.x - copy_start.x) * ratio) + copy_start.x;
		unsigned int cY = (int)((copy_end.y - copy_start.y) * ratio) + copy_start.y;

		PlotPixel(cX, floorf(cY + 0.5f), raster, 0xFFFFFFFF);
	}
}

unsigned int BaryCentricLerp(unsigned int c1, unsigned c2, unsigned int c3, VERTEX V)
{
#pragma region Color Channel Seperation
	int a1 = ((c1 & 0xFF000000) >> 24);
	int a2 = ((c2 & 0xFF000000) >> 24);
	int a3 = ((c3 & 0xFF000000) >> 24);

	int r1 = ((c1 & 0x00FF0000) >> 16);
	int r2 = ((c2 & 0x00FF0000) >> 16);
	int r3 = ((c3 & 0x00FF0000) >> 16);

	int g1 = ((c1 & 0x0000FF00) >> 8);
	int g2 = ((c2 & 0x0000FF00) >> 8);
	int g3 = ((c3 & 0x0000FF00) >> 8);

	int b1 = ((c1 & 0x000000FF));
	int b2 = ((c2 & 0x000000FF));
	int b3 = ((c3 & 0x000000FF));
#pragma endregion

	unsigned int burpedA = (unsigned int)(V.z * a1 + V.x * a2 + V.y * a3);
	unsigned int burpedB = (unsigned int)(V.z * r1 + V.x * r2 + V.y * r3);
	unsigned int burpedC = (unsigned int)(V.z * g1 + V.x * g2 + V.y * g3);
	unsigned int burpedD = (unsigned int)(V.z * b1 + V.x * b2 + V.y * b3);

	unsigned int finalA = burpedA << 24;
	unsigned int finalR = burpedB << 16;
	unsigned int finalG = burpedC << 8;
	unsigned int finalB = burpedD;

	return finalA | finalR | finalG | finalB;

}

void DrawTriangle(TRIANGLE tri, unsigned int* raster)
{
	FillTriangle(tri, raster);
	DrawLine(tri.A, tri.B, raster);
	DrawLine(tri.B, tri.C, raster);
	DrawLine(tri.C, tri.A, raster);
}

VERTEX FindBaryCentric(VERTEX A, VERTEX B, VERTEX C, VERTEX P)
{
	float beta = ImplicitLineEquation(B.x, B.y, A.x, A.y, C.x, C.y);
	float gamma = ImplicitLineEquation(C.x, C.y, B.x, B.y, A.x, A.y);
	float alpha = ImplicitLineEquation(A.x, A.y, C.x, C.y, B.x, B.y);

	float b = ImplicitLineEquation(P.x, P.y, A.x, A.y, C.x, C.y);
	float y = ImplicitLineEquation(P.x, P.y, B.x, B.y, A.x, A.y);
	float a = ImplicitLineEquation(P.x, P.y, C.x, C.y, B.x, B.y);
	VERTEX Temp;

	if (b / beta >= 0 && b / beta <= 1)
	{
		if (y / gamma >= 0 && y / gamma <= 1)
		{
			if (a / alpha >= 0 && a / alpha <= 1)
			{
				Temp.x = b / beta;
				Temp.y = y / gamma;
				Temp.z = a / alpha;
				return Temp;
			}
		}
	}

	Temp.x = -1;
	Temp.y = -1;
	Temp.z = -1;
	return Temp;

}

void FillTriangle(TRIANGLE tri, unsigned int* raster)
{
	VERTEX Copy_A = tri.A;
	VERTEX Copy_B = tri.B;
	VERTEX Copy_C = tri.C;

	if (VertexShader)
	{
		VertexShader(Copy_A);
		VertexShader(Copy_B);
		VertexShader(Copy_C);
	}

	Copy_A = CartesianToScreen(Copy_A);
	Copy_B = CartesianToScreen(Copy_B);
	Copy_C = CartesianToScreen(Copy_C);

	float firstx = std::fminf(Copy_A.x, Copy_B.x);
	float StartX = std::fminf(firstx, Copy_C.x);

	float firstY = std::fminf(Copy_A.y, Copy_B.y);
	float StartY = std::fminf(firstY, Copy_C.y);

	float LastX = std::fmaxf(Copy_A.x, Copy_B.x);
	float EndX = std::fmaxf(LastX, Copy_C.x);

	float lastY = std::fmaxf(Copy_A.y, Copy_B.y);
	float EndY = std::fmaxf(lastY, Copy_C.y);


	for (float CurrY = StartY; CurrY <= EndY; CurrY++)
	{
		for (float CurrX = StartX; CurrX <= EndX; CurrX++)
		{
			VERTEX P;
			P.x = CurrX;
			P.y = CurrY;

			VERTEX Temp = FindBaryCentric(Copy_A, Copy_B, Copy_C, P);

			if (Temp.x != -1)
			{
				PlotPixel(P.x, P.y, raster, BaryCentricLerp(0xFFFF0000, 0xFF00FF00, 0xFF0000FF, Temp));
			}
		}
	}
}