#pragma once

#include "Shaders.h"


float BaryCentricLerp(float z1, float z2, float z3, VERTEX V)
{
	return (V.z * z1 + V.x * z2 + V.y * z3);
}


void FillTriangle(TRIANGLE tri, unsigned int* raster, unsigned int color);

void DrawLine(const VERTEX &start, const VERTEX &end, unsigned int* raster, unsigned int color)
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

		float depth = BasicLerp(copy_start.z, copy_end.z, ratio);

		PlotPixel(cX, floorf(cY + 0.5f), depth, raster, color);
	}
}

void DrawTriangle(TRIANGLE tri, unsigned int* raster, unsigned int color)
{
	FillTriangle(tri, raster, color);
	DrawLine(tri.A, tri.B, raster, color);
	DrawLine(tri.B, tri.C, raster, color);
	DrawLine(tri.C, tri.A, raster, color);
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

void FillTriangle(TRIANGLE tri, unsigned int* raster, unsigned int color)
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
				float depth = BaryCentricLerp(Copy_A.z, Copy_B.z, Copy_C.z, Temp);

				P.u = BaryCentricLerp(Copy_A.u, Copy_B.u, Copy_C.u, Temp);
				P.v = BaryCentricLerp(Copy_A.v, Copy_B.v, Copy_C.v, Temp);


				if (PixelShader)
				{
					PixelShader(P);
				}
				PlotPixel(P.x, P.y, depth, raster, P.color);
			}
		}
	}
}

void DrawFilledCube(const TRIANGLE* triangles, unsigned int* raster)
{
	TRIANGLE triangles_Copy[12];
	for (unsigned int i = 0; i < 12; i++)
	{
		triangles_Copy[i] = triangles[i];
	}


	triangles_Copy[0].A.u = 1; triangles_Copy[0].A.v = 0;
	triangles_Copy[0].B.u = 0; triangles_Copy[0].B.v = 0;
	triangles_Copy[0].C.u = 0; triangles_Copy[0].C.v = 1;
	FillTriangle(triangles_Copy[0], pixels, 0xFFFF0000);
	/////////////////////////////////////////////////////
	triangles_Copy[1].A.u = 1; triangles_Copy[1].A.v = 0;
	triangles_Copy[1].B.u = 1; triangles_Copy[1].B.v = 1;
	triangles_Copy[1].C.u = 0; triangles_Copy[1].C.v = 1;
	FillTriangle(triangles_Copy[1], pixels, 0xFFFF0000);
	/////////////////////////////////////////////////////
	triangles_Copy[2].A.u = 0; triangles_Copy[2].A.v = 0;
	triangles_Copy[2].B.u = 1; triangles_Copy[2].B.v = 0;
	triangles_Copy[2].C.u = 1; triangles_Copy[2].C.v = 1;
	FillTriangle(triangles_Copy[2], pixels, 0xFF00FF00);
	/////////////////////////////////////////////////////
	triangles_Copy[3].A.u = 0; triangles_Copy[3].A.v = 0;
	triangles_Copy[3].B.u = 0; triangles_Copy[3].B.v = 1;
	triangles_Copy[3].C.u = 1; triangles_Copy[3].C.v = 1;
	FillTriangle(triangles_Copy[3], pixels, 0xFF00FF00);
	/////////////////////////////////////////////////////
	triangles_Copy[4].A.u = 1; triangles_Copy[4].A.v = 0;
	triangles_Copy[4].B.u = 0; triangles_Copy[4].B.v = 0;
	triangles_Copy[4].C.u = 0; triangles_Copy[4].C.v = 1;
	FillTriangle(triangles_Copy[4], pixels, 0xFF0000FF);
	/////////////////////////////////////////////////////
	triangles_Copy[5].A.u = 1; triangles_Copy[5].A.v = 0;
	triangles_Copy[5].B.u = 1; triangles_Copy[5].B.v = 1;
	triangles_Copy[5].C.u = 0; triangles_Copy[5].C.v = 1;
	FillTriangle(triangles_Copy[5], pixels, 0xFF0000FF);
	/////////////////////////////////////////////////////
	triangles_Copy[6].A.u = 0; triangles_Copy[6].A.v = 1;
	triangles_Copy[6].B.u = 1; triangles_Copy[6].B.v = 1;
	triangles_Copy[6].C.u = 1; triangles_Copy[6].C.v = 0;
	FillTriangle(triangles_Copy[6], pixels, 0xFFFFFF00);
	/////////////////////////////////////////////////////
	triangles_Copy[7].A.u = 0; triangles_Copy[7].A.v = 1;
	triangles_Copy[7].B.u = 0; triangles_Copy[7].B.v = 0;
	triangles_Copy[7].C.u = 1; triangles_Copy[7].C.v = 0;
	FillTriangle(triangles_Copy[7], pixels, 0xFFFFFF00);
	/////////////////////////////////////////////////////
	triangles_Copy[8].A.u = 1; triangles_Copy[8].A.v = 1;
	triangles_Copy[8].B.u = 0; triangles_Copy[8].B.v = 0;
	triangles_Copy[8].C.u = 0; triangles_Copy[8].C.v = 0;
	FillTriangle(triangles_Copy[8], pixels, 0xFFFF00FF); 
	/////////////////////////////////////////////////////
	triangles_Copy[9].A.u = 1; triangles_Copy[9].A.v = 1;
	triangles_Copy[9].B.u = 0; triangles_Copy[9].B.v = 1;
	triangles_Copy[9].C.u = 0; triangles_Copy[9].C.v = 0;
	FillTriangle(triangles_Copy[9], pixels, 0xFFFF00FF);
	/////////////////////////////////////////////////////
	triangles_Copy[10].A.u = 0; triangles_Copy[10].A.v = 1;
	triangles_Copy[10].B.u = 1; triangles_Copy[10].B.v = 1;
	triangles_Copy[10].C.u = 1; triangles_Copy[10].C.v = 0;
	FillTriangle(triangles_Copy[10], pixels, 0xFF00FFFF);
	/////////////////////////////////////////////////////
	triangles_Copy[11].A.u = 0; triangles_Copy[11].A.v = 1;
	triangles_Copy[11].B.u = 0; triangles_Copy[11].B.v = 0;
	triangles_Copy[11].C.u = 0; triangles_Copy[11].C.v = 1;
	FillTriangle(triangles_Copy[11], pixels, 0xFF00FFFF);

}
void DrawCube(const VERTEX *rect, unsigned int* raster, unsigned int color)
{
	DrawLine(rect[0], rect[1], raster, color);
	DrawLine(rect[1], rect[2], raster, color);
	DrawLine(rect[2], rect[3], raster, color);
	DrawLine(rect[3], rect[0], raster, color);

	DrawLine(rect[4], rect[5], raster, color);
	DrawLine(rect[5], rect[6], raster, color);
	DrawLine(rect[6], rect[7], raster, color);
	DrawLine(rect[7], rect[4], raster, color);

	//3D (Z lines)
	DrawLine(rect[0], rect[4], raster, color);
	DrawLine(rect[1], rect[5], raster, color);
	DrawLine(rect[2], rect[6], raster, color);
	DrawLine(rect[3], rect[7], raster, color);


}

void DrawGrid(const VERTEX *gridX, const VERTEX* gridY, unsigned int* raster, unsigned int color)
{
	for (unsigned int i = 0; i < 22; i += 2)
	{
		DrawLine(gridX[i], gridX[i + 1], raster, color);
		DrawLine(gridY[i], gridY[i + 1], raster, color);
	}
}