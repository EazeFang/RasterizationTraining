#pragma once

#include "CImage.h"

class CDraw
{
public:
	CDraw();
	~CDraw();

	void drawLine(int x0, int y0, int x1, int y1, CImage &image, ImageColor color);
};

