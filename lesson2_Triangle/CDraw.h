#pragma once

#include "CImage.h"
#include "geometry.h"
#include <vector> 
#include <iostream> 

const ImageColor white = ImageColor(255, 255, 255, 255);
const ImageColor red = ImageColor(255, 0, 0, 255);
const ImageColor green = ImageColor(0, 255, 0, 255);
const ImageColor blue = ImageColor(0, 0, 255, 255);
class CDraw
{
public:
	CDraw();
	~CDraw();
	//»­Ïß
	void drawLine(int x0, int y0, int x1, int y1, CImage &image, ImageColor color);
	void drawLine(Vec2i v0, Vec2i v1, CImage &image, ImageColor color);
	//»­Èý½ÇÐÎ
	void drawTriangle(Vec2i v0, Vec2i v1, Vec2i v2, CImage &image, ImageColor color);
	void drawTriangle(Vec2i *pts,  CImage &image, ImageColor color);
	bool pointInTriangle(Vec2i *pts, Vec2i P);
};

