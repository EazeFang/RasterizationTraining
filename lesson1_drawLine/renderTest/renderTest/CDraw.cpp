#include "CDraw.h"
#include <math.h>


CDraw::CDraw()
{
}


CDraw::~CDraw()
{
}

void CDraw::drawLine(int x0, int y0, int x1, int y1, CImage &image, ImageColor color)
{
// 	for (float t = 0.; t < 1.; t += .01) {
// 		int x = x0 + (x1 - x0)*t;
// 		int y = y0 + (y1 - y0)*t;
// 		image.set(x, y, color);
// 	}
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) { // make it left−to−right 
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int dx = x1 - x0;
	int dy = y1 - y0;
// 	float derror = std::abs(dy / float(dx));
// 	float error = 0;

	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
	int y = y0;

	for (int x = x0; x <= x1; x++)
	{
		float t = (x - x0) / (float)(x1 - x0);
		int y = y0 * (1. - t) + y1 * t;
		if (steep) {
			image.set(y, x, color); // if transposed, de−transpose 
		}
		else {
			image.set(x, y, color);
		}

// 		error += derror;
// 		if (error > .5) {
// 			y += (y1 > y0 ? 1 : -1);
// 			error -= 1.;
// 		}
		error2 += derror2;
		if (error2 > dx) {
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}
