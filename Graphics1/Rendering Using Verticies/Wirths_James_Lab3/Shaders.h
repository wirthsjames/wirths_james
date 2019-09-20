#pragma once

#include "MyMath.h"

void(*VertexShader)(VERTEX&) = 0;

void(*PixelShader)(VERTEX&) = 0;

// All Shader Variables (Always Pre-fixed by “SV_”)
MATRIX_3X3 SV_WorldMatrix;

// Various custom vertex and pixel shaders, (Pre-fixed by “VS_” & “PS_”)
// Can be swapped using above function pointers as needed for flexibility. 

// Applys the current world matrix to all
void VS_World(VERTEX &multiplyMe)
{
	multiplyMe = MultiplyVertexByMatrix(multiplyMe, SV_WorldMatrix);
}
// Basic pixel shader returns the color white
void PS_White(VERTEX &makeWhite)
{
	makeWhite.color = 0xFFFFFFFF;
}

