// Wirths_James_Lab5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Rasteriser.h"
#include <ctime>
#define R rectangle1

float DegreesToRadians(float degree)
{
	return degree * (M_PI / 180.0f);
}



void CheckInput(MATRIX_4X4& viewMatrix)
{
	float rotX = 0, rotY = 0, rotZ = 0;

	if (GetAsyncKeyState(VK_DOWN))// DOWN : Rotate +X
	{
		rotX += DegreesToRadians(1);

		viewMatrix = MuliplyMatrixByMatrix(viewMatrix, CreateXRotationMatrix(rotX));
	}
	else if (GetAsyncKeyState(VK_UP))// UP : Rotate -X
	{
		rotX += DegreesToRadians(-1);

		viewMatrix = MuliplyMatrixByMatrix(viewMatrix, CreateXRotationMatrix(rotX));
	}
	if (GetAsyncKeyState(VK_LEFT))
	{
		rotY += DegreesToRadians(-1);
		viewMatrix = MuliplyMatrixByMatrix(viewMatrix, CreateYRotationMatrix(rotY));
	}
	else if (GetAsyncKeyState(VK_RIGHT))
	{
		rotY += DegreesToRadians(1);
		viewMatrix = MuliplyMatrixByMatrix(viewMatrix, CreateYRotationMatrix(rotY));
	}

	SV_ViewMatrix = FastInverse(viewMatrix);

}

TRIANGLE fullImage;

void LoadFromFile()
{
	for (unsigned int i = 0; i < 1457; i++)
	{
		StoneHenge[i].x = STONE[i].pos[0] * 0.1f;
		StoneHenge[i].y = STONE[i].pos[1] * 0.1f;
		StoneHenge[i].z = STONE[i].pos[2] * 0.1f;
		StoneHenge[i].w = 1;

		StoneHenge[i].u = STONE[i].uvw[0];
		StoneHenge[i].v = STONE[i].uvw[1];

		StoneHenge[i].normal.x = STONE[i].nrm[0];
		StoneHenge[i].normal.y = STONE[i].nrm[1];
		StoneHenge[i].normal.z = STONE[i].nrm[2];
	}
}




//bcg bfg
int main()
{
#pragma region SetUp

	LoadFromFile();

	//Sets LightDir
	LightDir.x = LightDir.y = LightDir.z = 0; LightDir.w = 1;
	LightDir.normal.x = 0.577f; LightDir.normal.y = 0.577f; LightDir.normal.z = -0.577f;
	//
	srand(time(0));
	float camX = 0;
	float camY = 0;
	float camZ = 0;
	RS_Initialize(RWIDTH, RHEIGHT);

	for (unsigned int i = 0; i < RSIZE; i++)
	{
		zBuffer[i] = 1;
	}

	MATRIX_4X4 viewMatrix = CreateIdentityMatrix();

	VERTEX stars[3000];

	for (unsigned int i = 0; i < 3000; i++)
	{
		stars[i].x = RandFloat(-1.0f, 1.0f);
		stars[i].y = RandFloat(-1.0f, 1.0f);
		stars[i].z = RandFloat(-1.0f, 1.0f);
		stars[i].w = 1;

		stars[i].x *= 50.0f;
		stars[i].y *= 50.0f;
		stars[i].z *= 50.0f;

		stars[i].color = WHITE;
	}

	viewMatrix = MuliplyMatrixByMatrix(viewMatrix, CreateTranslationMatrix(0, 1, -5));


	float time = 0;
#pragma endregion

	
	


	while (RS_Update(pixels, RWIDTH * RHEIGHT))
	{
		CheckInput(viewMatrix);
		ClearRaster(pixels);

		for (unsigned int i = 0; i < RSIZE; i++)
		{
			zBuffer[i] = 1;
		}

		VertexShader = VS_SetWhite;
		PixelShader = nullptr;
		for (unsigned int i = 0; i < 3000; i++)
		{
			DrawStar(stars[i]);
		}

		VertexShader = VS_World;
		PixelShader = PS_Draw;

		for (unsigned int i = 0; i < 2532; i += 3)
		{
			fullImage.A = StoneHenge[StoneHenge_indicies[i]];
			fullImage.B = StoneHenge[StoneHenge_indicies[i + 1]];
			fullImage.C = StoneHenge[StoneHenge_indicies[i + 2]];
			FillTriangle(fullImage, pixels, 0xFFFFFFFF);
		}	
		
	}

	RS_Shutdown();
	return 0;
}

