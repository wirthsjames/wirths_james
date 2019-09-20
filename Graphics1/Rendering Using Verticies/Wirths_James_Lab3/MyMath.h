#pragma once
#include "Defines.h"

struct MATRIX_3X3
{
	// Individual parts of matrix

	float _e11, _e12, _e13;
	float _e21, _e22, _e23;
	float _e31, _e32, _e33;

};

VERTEX MultiplyVertexByMatrix(VERTEX V, MATRIX_3X3 M)
{
	VERTEX temp;

	temp.x = (V.x * M._e11) + (V.y * M._e21) + (V.z * M._e31);
	temp.y = (V.x * M._e12) + (V.y * M._e22) + (V.z * M._e32);
	temp.z = (V.x * M._e13) + (V.y * M._e23) + (V.z * M._e33);

	return temp;
}

MATRIX_3X3 ZRotationMatrixMultiplication(float theta)
{
	MATRIX_3X3 temp;

	temp._e11 = cos(theta); temp._e12 = -(sin(theta)); temp._e13 = 0;
	temp._e21 = sin(theta);  temp._e22 = cos(theta);    temp._e23 = 0;
	temp._e31 = 0;			 temp._e32 = 0;				 temp._e33 = 1;

	return temp;
}

VERTEX CartesianToScreen(VERTEX cartesianVertex)
{
	VERTEX T = cartesianVertex;
	T.x = ((T.x + 1) * (RWIDTH / 2));
	T.y = ((1 - T.y) * (RHEIGHT / 2));

	return T;
}

float GetRadiansOverTime(float &time)
{
	return ((time += 1.5f) * (PI / 180)) * .125f;
}