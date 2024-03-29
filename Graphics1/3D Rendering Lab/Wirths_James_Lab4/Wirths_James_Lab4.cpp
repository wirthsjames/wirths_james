// Wirths_James_Lab4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include"Rasteriser.h"

int main()
{
	unsigned int pixels[RWIDTH * RHEIGHT];
	//float zBuffer[RWIDTH * RHEIGHT];

	RS_Initialize(RWIDTH, RHEIGHT);

	for (unsigned int i = 0; i < RWIDTH * RHEIGHT; i++)
	{
		pixels[i] = 0;
	}

	const VERTEX horozontalLines[22] = {
		{-0.5,	 0,		0.5,	1,	0x0FF00FF00 },	{0.5,	0,	 0.5,	1,	0x0FF00FF00 },
		{-0.5,	 0,		0.4,	1,	0x0FF00FF00 },	{0.5,	0,	 0.4,	1,	0x0FF00FF00 },
		{-0.5,	 0,		0.3,	1,	0x0FF00FF00 },	{0.5,	0,	 0.3,	1,	0x0FF00FF00 },
		{-0.5,	 0,		0.2,	1,	0x0FF00FF00 },	{0.5,	0,	 0.2,	1,	0x0FF00FF00 },
		{-0.5,	 0,		0.1,	1,	0x0FF00FF00 },	{0.5,	0,	 0.1,	1,	0x0FF00FF00 },
		{-0.5,	 0,		0.0,	1,	0x0FF00FF00 },	{0.5,	0,	 0.0,	1,	0x0FF00FF00 },
		{-0.5,	-0,	   -0.1,	1,	0x0FF00FF00 },	{0.5,	0,	-0.1,	1,	0x0FF00FF00 },
		{-0.5,	-0,	   -0.2,	1,	0x0FF00FF00 },	{0.5,	0,	-0.2,	1,	0x0FF00FF00 },
		{-0.5,	-0,	   -0.3,	1,	0x0FF00FF00 },	{0.5,	0,	-0.3,	1,	0x0FF00FF00 },
		{-0.5,	-0,	   -0.4,	1,	0x0FF00FF00 },	{0.5,	0,	-0.4,	1,	0x0FF00FF00 },
		{-0.5,	-0,	   -0.5,	1,	0x0FF00FF00 },	{0.5,	0,	-0.5,	1,	0x0FF00FF00 }
	};

	const VERTEX verticalLines[22] = {
		{ 0.5,	 0,	-0.5,	1,		0x0FF00FF00 },	{ 0.5,	0,	0.5,	1,	0x0FF00FF00 },
		{ 0.4,	 0,	-0.5,	1,		0x0FF00FF00 },	{ 0.4,	0,	0.5,	1,	0x0FF00FF00 },
		{ 0.3,	 0,	-0.5,	1,		0x0FF00FF00 },	{ 0.3,	0,	0.5,	1,	0x0FF00FF00 },
		{ 0.2,	 0,	-0.5,	1,		0x0FF00FF00 },	{ 0.2,	0,	0.5,	1,	0x0FF00FF00 },
		{ 0.1,	 0,	-0.5,	1,		0x0FF00FF00 },	{ 0.1,	0,	0.5,	1,	0x0FF00FF00 },
		{ 0.0,	 0,	-0.5,	1,		0x0FF00FF00 },	{ 0.0,	0,	0.5,	1,	0x0FF00FF00 },
		{-0.1,	 0,	-0.5,	1,		0x0FF00FF00 },	{-0.1,	0,	0.5,	1,	0x0FF00FF00 },
		{-0.2,	 0,	-0.5,	1,		0x0FF00FF00 },	{-0.2,	0,	0.5,	1,	0x0FF00FF00 },
		{-0.3,	 0,	-0.5,	1,		0x0FF00FF00 },	{-0.3,	0,	0.5,	1,	0x0FF00FF00 },
		{-0.4,	 0,	-0.5,	1,		0x0FF00FF00 },	{-0.4,	0,	0.5,	1,	0x0FF00FF00 },
		{-0.5,	 0,	-0.5,	1,		0x0FF00FF00 },	{-0.5,	0,	0.5,	1,	0x0FF00FF00 }
	};

	const VERTEX rectangle1[8] = {
		{.25f,.25,.25,1,0xFF00FF00},  {-.25f,.25,.25,1,0xFF00FF00},{-.25f,-.25,.25,1,0xFF00FF00},{.25f,-.25,.25,1,0xFF00FF00 },
		{.25f,.25,-.25,1,0xFF00FF00},{-.25f,.25,-.25,1,0xFF00FF00},{-.25f,-.25,-.25,1,0xFF00FF00},{.25f,-.25,-.25,1,0xFF00FF00 }
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

	viewMatrix = MuliplyMatrixByMatrix(viewMatrix,CreateXRotationMatrix(-18.0f * (M_PI / 180.0f)) );

	



	SV_ViewMatrix = FastInverse(viewMatrix);

	

	float time = 0;
	while (RS_Update(pixels, RWIDTH * RHEIGHT))
	{
		ClearRaster(pixels);
		
		SV_WorldMatrix = MuliplyMatrixByMatrix(CreateYRotationMatrix(GetRadiansOverTime(time)), worldRect);
		VertexShader = VS_World;
		DrawCube(rectangle1, pixels, 0xFF00FF00);

		SV_WorldMatrix = gridMatrix;
		VertexShader = VS_World;
		DrawGrid(horozontalLines, verticalLines, pixels, 0xFFFFFFFF);

	}

	RS_Shutdown();
	return 0;
}

