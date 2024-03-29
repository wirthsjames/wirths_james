// Wirths_James_Lab3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Rasterizer.h"



int main()
{
	unsigned int pixels[RWIDTH * RHEIGHT];

	RS_Initialize(RWIDTH, RHEIGHT);

	//Clears The Buffer
	ClearRaster(pixels);

	const VERTEX points[3] = {	{0.0f, 0.5f, 0,0, 0xFFFF0000}, { -0.5, -0.5f, 0, 0, 0xFF00FF00 }, { 0.5, -0.5f, 0, 0, 0xFF0000FF}};

	TRIANGLE tri(points[0],points[1],points[2]);

	//FillTriangle(tri, pixels);
	
	float time = 0;

	DrawTriangle(tri, pixels);

	while (RS_Update(pixels, RWIDTH * RHEIGHT))
	{
		ClearRaster(pixels);
		VertexShader = VS_World;
		SV_WorldMatrix = ZRotationMatrixMultiplication(GetRadiansOverTime(time));
		DrawTriangle(tri, pixels);
	}


	RS_Shutdown();


	return 0;
}

