#pragma once

#include "MyMath.h"

void(*VertexShader)(VERTEX&) = 0;

void(*VectorShader)(VECTOR&) = 0;

void(*PixelShader)(VERTEX&, unsigned int,unsigned int,int,unsigned int) = 0;

// All Shader Variables (Always Pre-fixed by “SV_”)
MATRIX_4X4 SV_WorldMatrix = CreateIdentityMatrix();

MATRIX_4X4 SV_ViewMatrix = CreateIdentityMatrix();

MATRIX_4X4 pMatrix = PerspectiveProjectionMatrix(NEARPLANE, FARPLANE, FOV, (RHEIGHT / RWIDTH));


// Various custom vertex and pixel shaders, (Pre-fixed by “VS_” & “PS_”)
// Can be swapped using above function pointers as needed for flexibility. 

// Applys the current world matrix to all
void VS_World(VERTEX &multiplyMe)// 
{
	multiplyMe = MultiplyVertexByMatrix_4X4(multiplyMe, SV_WorldMatrix);
	multiplyMe = MultiplyVertexByMatrix_4X4(multiplyMe, SV_ViewMatrix);
	multiplyMe = MultiplyVertexByMatrix_4X4(multiplyMe, pMatrix);
	float lightRatio = Clamp(Vector_Dot(LightDir.normal, Vector_Normalize(multiplyMe.normal)));

	multiplyMe.color = PixelLerping(0, 0xFFC0C0F0, lightRatio);
}

void VS_SetWhite(VERTEX& makeColor)
{
	makeColor = MultiplyVertexByMatrix_4X4(makeColor, SV_WorldMatrix);
	makeColor = MultiplyVertexByMatrix_4X4(makeColor, SV_ViewMatrix);
	makeColor = MultiplyVertexByMatrix_4X4(makeColor, pMatrix);

	makeColor.color = WHITE;
}

// Basic pixel shader returns the color white
void PS_Draw(VERTEX &makeColor, unsigned int newWidth, unsigned int newHeight, int levels, unsigned int lerped)
{
	unsigned int u1 = (makeColor.u * newWidth);
	unsigned int v1 = (makeColor.v * newHeight);

	float uRatio = (makeColor.u * newWidth)  - u1;
	float vRatio = (makeColor.v * newHeight) - v1;


	unsigned int topLeft	 =		STONETEX[TwoDOneDConversion(u1, v1, newWidth)         +	StoneHenge_leveloffsets[levels]];
	unsigned int topRight	 =		STONETEX[TwoDOneDConversion(u1 + 1, v1, newWidth)     +	StoneHenge_leveloffsets[levels]];
	unsigned int bottomLeft	 =		STONETEX[TwoDOneDConversion(u1, v1 + 1, newWidth)     +	StoneHenge_leveloffsets[levels]];
	unsigned int bottomRight =		STONETEX[TwoDOneDConversion(u1 + 1, v1 + 1, newWidth) +	StoneHenge_leveloffsets[levels]];

	unsigned int lerpTop = PixelLerping(ColorConverter(topLeft), ColorConverter(topRight), uRatio);
	unsigned int lerpBottom = PixelLerping(ColorConverter(bottomLeft), ColorConverter(bottomRight), uRatio);

	unsigned int finalLerp = PixelLerping(lerpTop, lerpBottom, vRatio);

	makeColor.color = MultiplyColors(finalLerp, lerped);



	//makeColor.color = ColorConverter(FLOWER[TwoDOneDConversion(makeColor.u * flower_width, makeColor.v * flower_height, flower_width)]);
}

void PS_White(VERTEX &makeColor)
{
	makeColor.color = WHITE;
}
