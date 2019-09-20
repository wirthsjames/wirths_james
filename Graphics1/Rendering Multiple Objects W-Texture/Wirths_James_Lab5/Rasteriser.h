#pragma once

#include "Shaders.h"


float BaryCentricLerp(float z1, float z2, float z3, VERTEX V)
{
	return (V.z * z1 + V.x * z2 + V.y * z3);
}

unsigned int BaryCentricBlend(unsigned int pixelA, unsigned int pixelB, unsigned int pixelC, VERTEX V)
{
	int aAlpha = ((pixelA & 0xFF000000) >> 24);
	int bAlpha = ((pixelB & 0xFF000000) >> 24);
	int cAlpha = ((pixelC & 0xFF000000) >> 24);

	int aRed = ((pixelA & 0x00FF0000) >> 16);
	int bRed = ((pixelB & 0x00FF0000) >> 16);
	int cRed = ((pixelC & 0x00FF0000) >> 16);

	int aGreen = ((pixelA & 0x0000FF00) >> 8);
	int bGreen = ((pixelB & 0x0000FF00) >> 8);
	int cGreen = ((pixelC & 0x0000FF00) >> 8);

	int aBlue = (pixelA & 0x000000FF);
	int bBlue = (pixelB & 0x000000FF);
	int cBlue = (pixelC & 0x000000FF);

	unsigned int Al = (unsigned int)(V.z * aAlpha + V.x * bAlpha + V.y * cAlpha) << 24;
	unsigned int Re = (unsigned int)(V.z * aRed + V.x * bRed + V.y * cRed) << 16;
	unsigned int Gr = (unsigned int)(V.z * aGreen + V.x * bGreen + V.y *  cGreen) << 8;
	unsigned int Bl = (unsigned int)(V.z * aBlue + V.x * bBlue + V.y * cBlue);

	return Al | Re | Gr | Bl;
}

void FillTriangle(TRIANGLE tri, unsigned int* raster, unsigned int color);

int CheckForClipping(VERTEX& start, VERTEX& end)
{
	if (start.z < NEARPLANE)
	{
		float deltaZ = (abs(start.z));
		float deltaPZ = (abs(start.z - end.z));
		VERTEX c = VertexLerp(start, end, deltaZ / deltaPZ);
		start = c;

		return 1;
	}
	else if (end.z < NEARPLANE)
	{
		float deltaPZ = (abs(end.z));
		float deltaZ = (abs(end.z - start.z));
		VERTEX c = VertexLerp(end, start, deltaZ / deltaPZ);
		end = c;

		return 1;
	}
	else if (end.z < NEARPLANE && start.z < NEARPLANE)
	{
		return 2;
	}
	else if (end.z >= NEARPLANE && start.z >= NEARPLANE)
	{
		return 0;
	}
}

void DrawLine(const VERTEX &start, const VERTEX &end, unsigned int* raster, unsigned int color)
{
	VERTEX copy_start = start;
	VERTEX copy_end = end;

	if (VertexShader)
	{
		VertexShader(copy_start);
		VertexShader(copy_end);
	}

	unsigned int clipCase = CheckForClipping(copy_start, copy_end);

	if (clipCase == 0)// If None are clipping
	{

	}
	else if (clipCase == 1)// If one is clipping
	{

	}
	else if (clipCase == 2)// If both are clipping
	{
		return;
	}

	PerspectiveDivide(copy_start);
	PerspectiveDivide(copy_end);

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

void DrawStar(VERTEX& star)
{
	VERTEX Copy_Star = star;

	if (VertexShader)
	{
		VertexShader(Copy_Star);
	}
	PerspectiveDivide(Copy_Star);

	Copy_Star = CartesianToScreen(Copy_Star);

	PlotPixel(Copy_Star.x, Copy_Star.y, Copy_Star.z, pixels, Copy_Star.color);
}

void DrawTriangle(TRIANGLE& tri, unsigned int* raster, unsigned int color)
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

	float bDivBeta = b / beta;
	float yDivGamma = y / gamma;
	float aDivAlpha = a / alpha;

	if (bDivBeta >= 0 && bDivBeta <= 1)
	{
		if (yDivGamma >= 0 && yDivGamma <= 1)
		{
			if (aDivAlpha >= 0 && aDivAlpha <= 1)
			{
				Temp.x = bDivBeta;
				Temp.y = yDivGamma;
				Temp.z = aDivAlpha;
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

	PerspectiveDivide(Copy_A);
	PerspectiveDivide(Copy_B);
	PerspectiveDivide(Copy_C);

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

#pragma region Perspective Texture Fixing Optimization
	float linearZA = 1.0f / Copy_A.w;
	float linearZB = 1.0f / Copy_B.w;
	float linearZC = 1.0f / Copy_C.w;

	float AUL = (Copy_A.u / Copy_A.w);
	float AVL = (Copy_A.v / Copy_A.w);
	float BUL = (Copy_B.u / Copy_B.w);
	float BVL = (Copy_B.v / Copy_B.w);
	float CUL = (Copy_C.u / Copy_C.w);
	float CVL = (Copy_C.v / Copy_C.w);
#pragma endregion



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

				float lerpAllZ = BaryCentricLerp(linearZA, linearZB, linearZC, Temp);

				P.u = BaryCentricLerp(AUL, BUL, CUL, Temp);
				P.v = BaryCentricLerp(AVL, BVL, CVL, Temp);

				P.u /= lerpAllZ;
				P.v /= lerpAllZ;

				if (PixelShader)
				{
					float Pz = BaryCentricLerp(Copy_A.w, Copy_B.w, Copy_C.w, Temp);

					int level = (Pz - NEARPLANE) / (FARPLANE - NEARPLANE) * StoneHenge_numlevels;


					unsigned int newHeight = StoneHenge_height >> level;
					unsigned int newWidth = StoneHenge_width >> level;

					unsigned int lerped = BaryCentricBlend(Copy_A.color, Copy_B.color, Copy_C.color, Temp);


					PixelShader(P, newWidth, newHeight, level, lerped);
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

#pragma region U and V Positions
	/////////////////////////////////////////////////////////
	triangles_Copy[0].A.u = 1; triangles_Copy[0].A.v = 0;  //
	triangles_Copy[0].B.u = 0; triangles_Copy[0].B.v = 0;  //
	triangles_Copy[0].C.u = 0; triangles_Copy[0].C.v = 1;  //
	FillTriangle(triangles_Copy[0], pixels, 0xFFFF0000);   //
	/////////////////////////////////////////////////////////
	triangles_Copy[1].A.u = 1; triangles_Copy[1].A.v = 0;  //
	triangles_Copy[1].B.u = 1; triangles_Copy[1].B.v = 1;  //
	triangles_Copy[1].C.u = 0; triangles_Copy[1].C.v = 1;  //
	FillTriangle(triangles_Copy[1], pixels, 0xFFFF0000);   //
	/////////////////////////////////////////////////////////
	triangles_Copy[2].A.u = 0; triangles_Copy[2].A.v = 0;  //
	triangles_Copy[2].B.u = 1; triangles_Copy[2].B.v = 0;  //
	triangles_Copy[2].C.u = 1; triangles_Copy[2].C.v = 1;  //
	FillTriangle(triangles_Copy[2], pixels, 0xFF00FF00);   //
	/////////////////////////////////////////////////////////
	triangles_Copy[3].A.u = 0; triangles_Copy[3].A.v = 0;  //
	triangles_Copy[3].B.u = 0; triangles_Copy[3].B.v = 1;  //
	triangles_Copy[3].C.u = 1; triangles_Copy[3].C.v = 1;  //
	FillTriangle(triangles_Copy[3], pixels, 0xFF00FF00);   //
	/////////////////////////////////////////////////////////
	triangles_Copy[4].A.u = 1; triangles_Copy[4].A.v = 0;  //
	triangles_Copy[4].B.u = 0; triangles_Copy[4].B.v = 0;  //
	triangles_Copy[4].C.u = 0; triangles_Copy[4].C.v = 1;  //
	FillTriangle(triangles_Copy[4], pixels, 0xFF0000FF);   //
	/////////////////////////////////////////////////////////
	triangles_Copy[5].A.u = 1; triangles_Copy[5].A.v = 0;  //
	triangles_Copy[5].B.u = 1; triangles_Copy[5].B.v = 1;  //
	triangles_Copy[5].C.u = 0; triangles_Copy[5].C.v = 1;  //
	FillTriangle(triangles_Copy[5], pixels, 0xFF0000FF);   //
	/////////////////////////////////////////////////////////
	triangles_Copy[6].A.u = 0; triangles_Copy[6].A.v = 1;  //
	triangles_Copy[6].B.u = 1; triangles_Copy[6].B.v = 1;  //
	triangles_Copy[6].C.u = 1; triangles_Copy[6].C.v = 0;  //
	FillTriangle(triangles_Copy[6], pixels, 0xFFFFFF00);   //
	/////////////////////////////////////////////////////////
	triangles_Copy[7].A.u = 0; triangles_Copy[7].A.v = 1;  //
	triangles_Copy[7].B.u = 0; triangles_Copy[7].B.v = 0;  //
	triangles_Copy[7].C.u = 1; triangles_Copy[7].C.v = 0;  //
	FillTriangle(triangles_Copy[7], pixels, 0xFFFFFF00);   //
	/////////////////////////////////////////////////////////
	triangles_Copy[8].A.u = 1; triangles_Copy[8].A.v = 1;  //
	triangles_Copy[8].B.u = 1; triangles_Copy[8].B.v = 0;  //
	triangles_Copy[8].C.u = 0; triangles_Copy[8].C.v = 0;  //
	FillTriangle(triangles_Copy[8], pixels, 0xFFFF00FF);   //
	/////////////////////////////////////////////////////////
	triangles_Copy[9].A.u = 1; triangles_Copy[9].A.v = 1;  //
	triangles_Copy[9].B.u = 0; triangles_Copy[9].B.v = 1;  //
	triangles_Copy[9].C.u = 0; triangles_Copy[9].C.v = 0;  //
	FillTriangle(triangles_Copy[9], pixels, 0xFFFF00FF);   //
	/////////////////////////////////////////////////////////
	triangles_Copy[10].A.u = 0; triangles_Copy[10].A.v = 1;//
	triangles_Copy[10].B.u = 1; triangles_Copy[10].B.v = 1;//
	triangles_Copy[10].C.u = 1; triangles_Copy[10].C.v = 0;//
	FillTriangle(triangles_Copy[10], pixels, 0xFF00FFFF);  //
	/////////////////////////////////////////////////////////
	triangles_Copy[11].A.u = 0; triangles_Copy[11].A.v = 1;//
	triangles_Copy[11].B.u = 0; triangles_Copy[11].B.v = 0;//
	triangles_Copy[11].C.u = 1; triangles_Copy[11].C.v = 0;//
	FillTriangle(triangles_Copy[11], pixels, 0xFF00FFFF);  //
	/////////////////////////////////////////////////////////  
#pragma endregion

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

