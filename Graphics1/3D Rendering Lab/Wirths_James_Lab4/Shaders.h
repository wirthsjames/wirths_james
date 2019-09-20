#pragma once

#include "MyMath.h"

void(*VertexShader)(VERTEX&) = 0;

void(*PixelShader)(VERTEX&) = 0;

// All Shader Variables (Always Pre-fixed by “SV_”)
MATRIX_4X4 SV_WorldMatrix = CreateIdentityMatrix();

MATRIX_4X4 SV_ViewMatrix = CreateIdentityMatrix();

MATRIX_4X4 pMatrix = PerspectiveProjectionMatrix(0.1f, 10.0f, 90.0f, RWIDTH / RHEIGHT);


// Various custom vertex and pixel shaders, (Pre-fixed by “VS_” & “PS_”)
// Can be swapped using above function pointers as needed for flexibility. 

// Applys the current world matrix to all
void VS_World(VERTEX &multiplyMe)// Cube
{
	multiplyMe = MultiplyVertexByMatrix_4X4(multiplyMe, SV_WorldMatrix);
	multiplyMe = MultiplyVertexByMatrix_4X4(multiplyMe, SV_ViewMatrix);
	multiplyMe = MultiplyVertexByMatrix_4X4(multiplyMe, pMatrix);
	multiplyMe.x = multiplyMe.x / multiplyMe.w;
	multiplyMe.y = multiplyMe.y / multiplyMe.w;
	multiplyMe.z = multiplyMe.z / multiplyMe.w;
	//multiplyMe.w = multiplyMe.w / multiplyMe.w;

}

void VS_Cells(VERTEX &multiplyMe)
{
	multiplyMe = MultiplyVertexByMatrix_4X4(multiplyMe, SV_WorldMatrix);
	multiplyMe = MultiplyVertexByMatrix_4X4(multiplyMe, SV_ViewMatrix);
	multiplyMe = MultiplyVertexByMatrix_4X4(multiplyMe, pMatrix);
	multiplyMe.x = multiplyMe.x / multiplyMe.w;
	multiplyMe.y = multiplyMe.y / multiplyMe.w;
	multiplyMe.z = multiplyMe.z / multiplyMe.w;
	//multiplyMe.w = multiplyMe.w / multiplyMe.w;

}

// Basic pixel shader returns the color white
void PS_White(VERTEX &makeWhite)
{
	makeWhite.color = 0xFFFFFFFF;
}