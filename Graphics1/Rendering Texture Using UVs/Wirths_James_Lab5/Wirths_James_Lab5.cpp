// Wirths_James_Lab5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Rasteriser.h"
#define R rectangle1



int main()
{
	RS_Initialize(RWIDTH, RHEIGHT);

	for (unsigned int i = 0; i < RSIZE; i++)
	{
		pixels[i] = 0;
	}

	for (unsigned int i = 0; i < RSIZE; i++)
	{
		zBuffer[i] = 1;
	}

	const VERTEX horozontalLines[22] = {
	{-0.5,	 0,		0.5,	1,	0x0FF00FF00 },{ 0.5,	0,	 0.5,	1,	0x0FF00FF00},
	{-0.5,	 0,		0.4,	1,	0x0FF00FF00 },{ 0.5,	0,	 0.4,	1,	0x0FF00FF00},
	{-0.5,	 0,		0.3,	1,	0x0FF00FF00 },{ 0.5,	0,	 0.3,	1,	0x0FF00FF00},
	{-0.5,	 0,		0.2,	1,	0x0FF00FF00 },{ 0.5,	0,	 0.2,	1,	0x0FF00FF00},
	{-0.5,	 0,		0.1,	1,	0x0FF00FF00 },{ 0.5,	0,	 0.1,	1,	0x0FF00FF00},
	{-0.5,	 0,		0.0,	1,	0x0FF00FF00 },{ 0.5,	0,	 0.0,	1,	0x0FF00FF00},
	{-0.5,	-0,	   -0.1,	1,	0x0FF00FF00 },{ 0.5,	0,	-0.1,	1,	0x0FF00FF00},
	{-0.5,	-0,	   -0.2,	1,	0x0FF00FF00 },{ 0.5,	0,	-0.2,	1,	0x0FF00FF00},
	{-0.5,	-0,	   -0.3,	1,	0x0FF00FF00 },{ 0.5,	0,	-0.3,	1,	0x0FF00FF00},
	{-0.5,	-0,	   -0.4,	1,	0x0FF00FF00 },{ 0.5,	0,	-0.4,	1,	0x0FF00FF00},
	{-0.5,	-0,	   -0.5,	1,	0x0FF00FF00 },{ 0.5,	0,	-0.5,	1,	0x0FF00FF00}
	};

	const VERTEX verticalLines[22] = {
	{ 0.5,	 0,		-0.5,	1,	0x0FF00FF00 },{ 0.5,	0,	0.5,	1,	0x0FF00FF00},
	{ 0.4,	 0,		-0.5,	1,	0x0FF00FF00 },{ 0.4,	0,	0.5,	1,	0x0FF00FF00},
	{ 0.3,	 0,		-0.5,	1,	0x0FF00FF00 },{ 0.3,	0,	0.5,	1,	0x0FF00FF00},
	{ 0.2,	 0,		-0.5,	1,	0x0FF00FF00 },{ 0.2,	0,	0.5,	1,	0x0FF00FF00},
	{ 0.1,	 0,		-0.5,	1,	0x0FF00FF00 },{ 0.1,	0,	0.5,	1,	0x0FF00FF00},
	{ 0.0,	 0,		-0.5,	1,	0x0FF00FF00 },{ 0.0,	0,	0.5,	1,	0x0FF00FF00},
	{-0.1,	 0,		-0.5,	1,	0x0FF00FF00 },{-0.1,	0,	0.5,	1,	0x0FF00FF00},
	{-0.2,	 0,		-0.5,	1,	0x0FF00FF00 },{-0.2,	0,	0.5,	1,	0x0FF00FF00},
	{-0.3,	 0,		-0.5,	1,	0x0FF00FF00 },{-0.3,	0,	0.5,	1,	0x0FF00FF00},
	{-0.4,	 0,		-0.5,	1,	0x0FF00FF00 },{-0.4,	0,	0.5,	1,	0x0FF00FF00},
	{-0.5,	 0,		-0.5,	1,	0x0FF00FF00 },{-0.5,	0,	0.5,	1,	0x0FF00FF00}
	};

	const VERTEX rectangle1[8] = {
		{ .25f,.25,.25,1,0xFF00FF00 },{ -.25f,.25,.25,1,0xFF00FF00 },{ -.25f,-.25,.25,1,0xFF00FF00 },{ .25f,-.25,.25,1,0xFF00FF00 },
	{ .25f,.25,-.25,1,0xFF00FF00 },{ -.25f,.25,-.25,1,0xFF00FF00 },{ -.25f,-.25,-.25,1,0xFF00FF00 },{ .25f,-.25,-.25,1,0xFF00FF00 }
	};

	MATRIX_4X4 worldRect = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	MATRIX_4X4 gridMatrix = CreateIdentityMatrix();

	MATRIX_4X4 viewMatrix = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};



	worldRect = MuliplyMatrixByMatrix(worldRect, CreateTranslationMatrix(0, 0.25f, 0));

	viewMatrix = MuliplyMatrixByMatrix(viewMatrix, CreateTranslationMatrix(0, 0, -1));

	viewMatrix = MuliplyMatrixByMatrix(viewMatrix, CreateXRotationMatrix(-18.0f * (M_PI / 180.0f)));

	SV_ViewMatrix = FastInverse(viewMatrix);

	TRIANGLE triangles[12] = {
		{R[0],R[1],R[2]},{R[0],R[3],R[2]},//Front 0 , 1
		{R[0],R[4],R[7]},{R[0],R[3],R[7]},//Right 2 , 3
		{R[1],R[5],R[6]},{R[1],R[2],R[6]},//Left  4 , 5
		{R[7],R[6],R[5]},{R[7],R[4],R[5]},//Back  6 , 7
		{R[0],R[4],R[5]},{R[0],R[1],R[5]},//Top   8 , 9
		{R[2],R[3],R[7]},{R[2],R[6],R[7]},//Bottom 10, 11
	};

	

	float time = 0;
	while (RS_Update(pixels, RWIDTH * RHEIGHT))
	{
		for (unsigned int i = 0; i < RSIZE; i++)
		{
			zBuffer[i] = 1;
		}
		ClearRaster(pixels);

		SV_WorldMatrix = gridMatrix;
		VertexShader = VS_World;
		DrawGrid(horozontalLines, verticalLines, pixels, 0xFFFFFFFF);



		SV_WorldMatrix = MuliplyMatrixByMatrix(CreateYRotationMatrix(GetRadiansOverTime(time)), worldRect);
		//VertexShader = VS_World;
		//DrawCube(rectangle1, pixels, 0xFF00FF00);

		PixelShader = PS_Draw;
		DrawFilledCube(triangles, pixels);
	}

	RS_Shutdown();
	return 0;
}

