// Wirths_James_Lab2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "RasterSurface.h"
#include <Windows.h>
#include <ctime>
#include <random>
#include <assert.h>
#include <math.h>

#define RWIDTH 500
#define RHEIGHT 500


#define MAGENTA 0xFFFF00FF
#define CYAN 0xFF00FFFF

unsigned int TwoDimentionalConverter(unsigned int xPos, unsigned int yPos, unsigned int width);
float Lerp(float start, float end, float ratio);
void PlotPixel(unsigned int currX, unsigned int currY, unsigned int* raster, unsigned int color);
float ImplicitLineEquation(float _startX, float _startY, float _endX, float _endY, float midX, float midY);
unsigned int PixelLerping(unsigned int pixelA, unsigned int pixelB, float ratio);

void DrawParametric(float _startX, float _startY, float _endX, float _endY, unsigned int* raster, unsigned int color);
void DrawBressenham(float _startX, float _startY, float _endX, float _endY, unsigned int* raster, unsigned int color);
void DrawMidpoint(float _startX, float _startY, float _endX, float _endY, unsigned int* raster, unsigned int color);

void ClearBuffer(unsigned int* raster)
{
	for (unsigned int i = 0; i < RWIDTH * RHEIGHT; i++)
	{
		raster[i] = 0;
	}
}

void Draw3StartingLines(unsigned int* raster)
{
	DrawBressenham(0, 100, 499, 400, raster, 0xFFFF0000);
	DrawMidpoint(0, 110, 499, 410, raster, 0xff00FF00);
	DrawParametric(0, 120, 499, 420, raster, MAGENTA);

}

void DrawStarts(unsigned int* raster)
{
	for (unsigned int i = 0; i < rand() % 500 + 2000; i++)
	{
		unsigned int tempX = rand() % 500;
		unsigned int tempY = rand() % 500;
		PlotPixel(tempX, tempY, raster, 0xFFFFFFFF);
	}
}

void PlotPixel(unsigned int currX, unsigned int currY, unsigned int* raster, unsigned int color)
{
	raster[TwoDimentionalConverter(currX, currY, RWIDTH)] = color;
}

int main()
{
	srand(time(nullptr));

	unsigned int pixels[RWIDTH * RHEIGHT]; // 250,000 pixels

	RS_Initialize(RWIDTH, RHEIGHT);

	ClearBuffer(pixels);

	DrawStarts(pixels);

	Draw3StartingLines(pixels);


	while (RS_Update(pixels, RWIDTH * RHEIGHT))
	{
		if (GetAsyncKeyState(49))
		{
			ClearBuffer(pixels);
			Draw3StartingLines(pixels);
			DrawBressenham(rand() % 500, rand() % 500, rand() % 500, rand() % 500, pixels, 0xFF00FF00);
			Sleep(50);
		}
		else if (GetAsyncKeyState(50))
		{
			ClearBuffer(pixels);
			Draw3StartingLines(pixels);
			Sleep(50);
		} 
		else if (GetAsyncKeyState(51))
		{
			ClearBuffer(pixels);
			Draw3StartingLines(pixels);
			DrawParametric(rand() % 500, rand() % 500, rand() % 500, rand() % 500, pixels, 0xFF0000FF);
			Sleep(50);
		}
		else if (GetAsyncKeyState(52))
		{
			ClearBuffer(pixels);
			Draw3StartingLines(pixels);
			DrawStarts(pixels);
			Sleep(50);
		}
	}

	RS_Shutdown();

	return 0;
}

void DrawMidpoint(float _startX, float _startY, float _endX, float _endY, unsigned int* raster, unsigned int color)
{
	float startX;
	float startY;
	float endX;
	float endY;

	if (_startX > _endX)
	{
		startX = _endX;
		startY = _endY;

		endX = _startX;
		endY = _startY;
	}
	else
	{
		startX = _startX;
		startY = _startY;

		endX = _endX;
		endY = _endY;
	}
	unsigned int currY = startY;
	for (unsigned int i = startX; i < endX; i++)
	{
		unsigned int currX = i;
		PlotPixel(currX, currY, raster, color);
		float midPointX = currX + 1;
		float midPointY = currY + 1;

		if (ImplicitLineEquation(startX, startY, endX, endY, midPointX, midPointY) < 0)
		{
			currY++;
		}
	}

	PlotPixel(startX, startY, raster, 0xffffff00);
	PlotPixel(endX, endY, raster, 0xffffff00);
}

void DrawBressenham(float _startX, float _startY, float _endX, float _endY, unsigned int* raster, unsigned int color)
{

	float slope;

	float deltaY(abs((float)_endY - (float)_startY));
	float deltaX(abs((float)_endX - (float)_startX));

	unsigned int longest = max(deltaX, deltaY);

	if (_endX - _startX == 0)
	{
		slope = 0;
	}
	else
	{
		slope = (_endY - _startY) / (_endX - _startX);
	}

	float Error = 0;


	if (deltaX > deltaY)
	{
#pragma region DX > DY
		float deltaR = abs(deltaY / deltaX);

		if (_startX > _endX)
		{
			unsigned int CurrY = _startY;
			for (unsigned int i = _startX; i > longest; i--)
			{
				unsigned int CurrX = i;
				PlotPixel(CurrX, CurrY, raster, color);
				if (slope > 0)
				{
					Error += slope;
				}
				else if (slope < 0)
				{
					Error -= slope;
				}

				if (CurrY > 499)
				{
					break;
				}

				while (Error > 0.5f)
				{
					if (_endY > _startY)
					{
						CurrY += 1;
					}
					else
					{
						CurrY -= 1;
					}
					Error--;
				}
			}
		}
		else if (_endX > _startX)
		{
			unsigned int CurrY = _startY;
			for (unsigned int i = _startX; i <= longest; i++)
			{
				unsigned int CurrX = i;
				PlotPixel(CurrX, CurrY, raster, color);
				if (slope > 0)
				{
					Error += slope;
				}
				else if (slope < 0)
				{
					Error -= slope;
				}

				if (CurrY > 499)
				{
					break;
				}

				while (Error > 0.5f)
				{
					if (_endY > _startY)
					{
						CurrY += 1;
					}
					else
					{
						CurrY -= 1;
					}
					Error--;
				}
			}
		}
#pragma endregion
	}
	else if (deltaY > deltaX)
	{
#pragma region DY > DX
		if (_startY > _endY)
		{
			unsigned int CurrX = _startX;

			for (unsigned int i = _startY; i >= _endX; i--)
			{
				unsigned int CurrY = i;
				PlotPixel(CurrX, CurrY, raster, color);

				if (slope > 0)
				{
					Error += slope;
				}
				else if (slope < 0)
				{
					Error -= slope;
				}

				if (CurrX > 499)
				{
					break;
				}

				if (Error > 0.5f)
				{
					if (_endX > _startY)
					{
						CurrX += 1;
					}
					else
					{
						CurrX -= 1;
					}
					Error--;
				}
			}
		}
		else if (_endY > _startY)
		{
			unsigned int CurrX = _startX;

			for (unsigned int i = _startY; i <= _endX; i++)
			{
				unsigned int CurrY = i;
				PlotPixel(CurrX, CurrY, raster, color);

				if (slope > 0)
				{
					Error += slope;
				}
				else if (slope < 0)
				{
					Error -= slope;
				}

				if (CurrX > 499)
				{
					break;
				}

				while (Error > 0.5f)
				{
					if (_endX > _startY)
					{
						CurrX += 1;
					}
					else
					{
						CurrX -= 1;
					}
					Error--;
				}
			}
		}
#pragma endregion
	}
}

void DrawParametric(float _startX, float _startY, float _endX, float _endY, unsigned int* raster, unsigned int color)
{
	//Color Starts At Magenta
	unsigned int currentColor = color;


	unsigned int deltaX = abs(_endX - _startX);
	unsigned int deltaY = abs(_endY - _startY);

	unsigned int length = max(deltaX, deltaY);

	if (deltaX > deltaY)// if Change In X > Y//
	{
		if (_startX > _endX)
		{
			for (unsigned int i = _startX; i >= _endX; i--)
			{
				unsigned int currX = i;
				float ratio = (currX - _startX) / (_endX - _startX);
				float currY = Lerp((float)_startY, (float)_endY, ratio);
				PlotPixel(currX, floor(currY + 0.5f), raster, PixelLerping(MAGENTA, CYAN, ratio));
			}
		}
		else if (_endX > _startX)
		{
			for (unsigned int i = _startX; i <= _endX; i++)
			{
				unsigned int currX = i;
				float ratio = (float)(currX - _startX) / (float)(deltaX);
				float currY = Lerp((float)_startY, (float)_endY, ratio);
				PlotPixel(currX, floor(currY + 0.5f), raster, PixelLerping(MAGENTA, CYAN, ratio));
			}
		}
	}
	else if (deltaY > deltaX)// if Change In Y > X//
	{
		if (_startY > _endY)
		{
			for (unsigned int i = _startY; i >= _endY; i--)
			{
				unsigned int currY = i;
				float ratio = (float)(currY - _startY) / (float)(_endY - _startY);
				float currX = Lerp((float)_startX, (float)_endX, ratio);
				PlotPixel(currX, floor(currY + 0.5f), raster, PixelLerping(MAGENTA, CYAN, ratio));
			}
		}
		if (_endY > _startY)
		{
			for (unsigned int i = _startY; i <= _endY; i++)
			{
				unsigned int currY = i;
				float ratio = (float)(currY - _startY) / (float)(_endY - _startY);
				float currX = Lerp((float)_startX, (float)_endX, ratio);
				PlotPixel(currX, floor(currY + 0.5f), raster, PixelLerping(MAGENTA, CYAN, ratio));
			}
		}
	}

	raster[TwoDimentionalConverter(_startX, _startY, RWIDTH)] = 0xFFFFFF00;
	raster[TwoDimentionalConverter(_endX, _endY, RWIDTH)] = 0xFFFFFF00;
}

unsigned int TwoDimentionalConverter(unsigned int xPos, unsigned int yPos, unsigned int width)
{
	return (yPos * width) + xPos;
}

float ImplicitLineEquation(float _startX, float _startY, float _endX, float _endY, float midX, float midY)// Midpoint ImplicitLineEquation Function
{
	return (_startY - _endY)*midX + (_endX - _startX)*midY + (_startX * _endY) - (_startY * _endX);
}

float Lerp(float start, float end, float ratio)//Parametric Lerp Function
{
	return start + (ratio * (end - start));
}



// B is the Destination (End), A is Source(start)
unsigned int PixelLerping(unsigned int pixelA, unsigned int pixelB, float ratio)
{
	//B is raster
	//A is sourcePixel
	//(B - A) * alpha/255 + A

	int aAlpha = ((pixelA & 0xFF000000) >> 24);
	int bAlpha = ((pixelB & 0xFF000000) >> 24);

	int aRed = ((pixelA & 0x00FF0000) >> 16);
	int bRed = ((pixelB & 0x00FF0000) >> 16);

	int aGreen = ((pixelA & 0x0000FF00) >> 8);
	int bGreen = ((pixelB & 0x0000FF00) >> 8);

	int aBlue = (pixelA & 0x000000FF);
	int bBlue = (pixelB & 0x000000FF);

	float r = ratio;

	unsigned int LerpedAlpha = (((bAlpha - aAlpha) * r) + aAlpha);
	LerpedAlpha <<= 24;

	unsigned int LerpedRed = (((bRed - aRed) * r) + aRed);
	LerpedRed <<= 16;

	unsigned int LerpedGreen = (((bGreen - aGreen) * r) + aGreen);
	LerpedGreen <<= 8;

	unsigned int LerpedBlue = (((bBlue - aBlue) * r) + aBlue);


	unsigned int answer = LerpedAlpha | LerpedRed | LerpedGreen | LerpedBlue;

	return answer;

}

