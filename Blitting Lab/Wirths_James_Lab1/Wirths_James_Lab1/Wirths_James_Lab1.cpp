// Wirths_James_Lab1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "RasterSurface.h"
#include <iostream>
#include "tiles_12.h"
#include "fire_02.h"//128x128 blocks
#include <ctime>
#include <random>

#define RWIDTH 500
#define RHEIGHT 500

// Raster Pixels
unsigned int Pixels[250000];

void SetAllPixels();
unsigned int Return1DIndex(unsigned int xPOS, unsigned int yPos, unsigned int width);
void Blit(unsigned int rXPos, unsigned int rYPos, const unsigned int* sourceImage, unsigned int* destination, /*Starting Point*/unsigned int top, unsigned int bottom, /*Starting Point*/ unsigned int left, unsigned int right, unsigned int sourceWidth);
void BlitAnim(unsigned int rXPos, unsigned int rYPos, const unsigned int* sourceImage, unsigned int* destination, unsigned int sourceWidth);
void DrawPixels(unsigned int xPos, unsigned int yPos, unsigned int* source, unsigned int pixel, unsigned int width);
void ColorBuffer();
unsigned int ColorConverter(unsigned int pixel);


unsigned int PixelLerping(unsigned int pixelA, unsigned int pixelB)
{
	//B is raster
	//A is sourcePixel
	//(B - A) * sourceAlpha/255 + A

	int aAlpha = ((pixelA & 0xFF000000) >> 24);
	int bAlpha = ((pixelB & 0xFF000000) >> 24);

	int aRed = ((pixelA & 0x00FF0000) >> 16);
	int bRed = ((pixelB & 0x00FF0000) >> 16);

	int aGreen = ((pixelA & 0x0000FF00) >> 8);
	int bGreen = ((pixelB & 0x0000FF00) >> 8);

	int aBlue = (pixelA & 0x000000FF);
	int bBlue = (pixelB & 0x000000FF);

	float r = (float)(aAlpha/255);

	unsigned int LerpedAlpha = (unsigned int)(((bAlpha - aAlpha) * r) + aAlpha);
	LerpedAlpha <<= 24;

	unsigned int LerpedRed = (unsigned int)(((bRed - aRed) * r) + aRed);
	LerpedRed <<= 16;

	unsigned int LerpedGreen = (unsigned int)(((bGreen - aGreen) * r) + aGreen);
	LerpedGreen <<= 8;

	unsigned int LerpedBlue = (unsigned int)(((bBlue - aBlue) * r) + aBlue);


	unsigned int answer = LerpedAlpha | LerpedRed | LerpedGreen | LerpedBlue;

	return answer;

}


int main()
{


	RS_Initialize(RWIDTH, RHEIGHT);

	SetAllPixels();

	while (RS_Update(Pixels, RWIDTH * RHEIGHT))
	{
		for (unsigned int y = 0; y < RHEIGHT; y += 29)
		{
			for (unsigned int x = 0; x < RWIDTH; x += 29)
			{
				Blit(x, y, tiles_12_pixels, Pixels, 129, 158, 289, 318, tiles_12_width);
			}
		}

		for (unsigned int i = 0; i < 10; i++)
		{
			unsigned int tempX = rand() % 300;
			unsigned int tempY = rand() % 300;

			Blit(tempX, tempY, tiles_12_pixels, Pixels, 15, 100, 318, 386, tiles_12_width);
		}

		BlitAnim(122, 122, fire_02_pixels, Pixels, fire_02_width);
	}


	RS_Shutdown();

	return 0;
}

void SetAllPixels()
{
	for (unsigned int i = 0; i < 250000; i++)
	{
		Pixels[i] = 0;
	}
}

void Blit(unsigned int rXPos, unsigned int rYPos, const unsigned int* sourceImage, unsigned int* destination, /*Starting Point*/unsigned int top, unsigned int bottom, /*Starting Point*/ unsigned int left, unsigned int right, unsigned int sourceWidth)
{
	unsigned int _sourceHeight = bottom - top;
	unsigned int _sourceWidth = right - left;

	for (unsigned int y = 0; y < _sourceHeight; y++)
	{
		for (unsigned int x = 0; x < _sourceWidth; x++)
		{
			DrawPixels(x + rXPos, y + rYPos, Pixels, ColorConverter(tiles_12_pixels[Return1DIndex(x + left, y + top, sourceWidth)]), RWIDTH);
		}
	}
}

void BlitAnim(unsigned int rXPos, unsigned int rYPos, const unsigned int* sourceImage, unsigned int* destination, unsigned int sourceWidth)
{
	for (unsigned int y2 = 0; y2 < fire_02_height; y2 += 128)
	{
		for (unsigned int x2 = 0; x2 < fire_02_width; x2 += 128)
		{
			Blit(rXPos, rYPos, sourceImage, destination, y2, y2 + 128, x2, x2 + 128, sourceWidth);
		}
	}
}



void DrawPixels(unsigned int xPos, unsigned int yPos, unsigned int* source, unsigned int pixel, unsigned int width)
{
	Pixels[Return1DIndex(xPos, yPos, width)] = (pixel - Pixels[Return1DIndex(xPos, yPos, width)]) * (((pixel & 0xFF000000) >> 24) / 255) + Pixels[Return1DIndex(xPos, yPos, width)];
}

unsigned int Return1DIndex(unsigned int xPOS, unsigned int yPos, unsigned int width)
{
	return ((yPos * width) + xPOS);
}

void ColorBuffer()
{
	for (unsigned int i = 0; i < 250000; i++)
	{
		Pixels[i] = 0;
	}
}

unsigned int ColorConverter(unsigned int pixel) //Converts BGRA -> ARGB
{
	unsigned int newColor = 0;

	//// Extracts 4 channels to 1 byte chars
	unsigned int alpha = (pixel & 0x000000FF) << 24;
	unsigned int red = (pixel & 0x0000FF00) << 8;
	unsigned int green = (pixel & 0x00FF0000) >> 8;
	unsigned int blue = (pixel & 0xFF000000) >> 24;

	return newColor = alpha | red | blue | green | blue;

}