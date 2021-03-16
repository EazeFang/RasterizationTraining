#include "CDraw.h"
#include <cmath>
#include <algorithm>


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

void CDraw::drawLine(Vec2i v0, Vec2i v1, CImage &image, ImageColor color)
{
	bool steep = false;
	int x0 = v0.raw[0];
	int x1 = v1.raw[0];
	int y0 = v0.raw[1];
	int y1 = v1.raw[1];
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

void CDraw::drawTriangle(Vec2i v0, Vec2i v1, Vec2i v2, CImage &image, ImageColor color)
{
//  分段扫描线法绘制2d三角形
	// sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!) 
	if (v0.y > v1.y) std::swap(v0, v1);
	if (v0.y > v2.y) std::swap(v0, v2);
	if (v1.y > v2.y) std::swap(v1, v2);
// 	drawLine(v0, v1, image, green);
// 	drawLine(v1, v2, image, green);
// 	drawLine(v2, v0, image, red);
 	int total_height = v2.y - v0.y;
	int segment_height = v1.y - v0.y + 1;
	for (int y = v0.y; y <= v1.y; y++) 
	{
		float alpha = (float)(y - v0.y) / total_height;
		float beta = (float)(y - v0.y) / segment_height; // be careful with divisions by zero 
		Vec2i A = v0 + (v2 - v0)*alpha;
		Vec2i B = v0 + (v1 - v0)*beta;
		//画一条线从A到B
		if (A.x > B.x) std::swap(A, B);
		for (int j = A.x; j <= B.x; j++) 
		{
			image.set(j, y, color); // attention, due to int casts t0.y+i != A.y 
		}
	}
	segment_height = v2.y - v1.y + 1;
	for (int y = v1.y; y <= v2.y; y++) 
	{
		float alpha = (float)(y - v0.y) / total_height;
		float beta = (float)(y - v1.y) / segment_height; // be careful with divisions by zero 
		Vec2i A = v0 + (v2 - v0)*alpha;
		Vec2i B = v1 + (v2 - v1)*beta;
		if (A.x > B.x) std::swap(A, B);
		for (int j = A.x; j <= B.x; j++) 
		{
			image.set(j, y, color); // attention, due to int casts t0.y+i != A.y 
		}
	}
	//重心法判断绘制三角形todo
// 	Vec2i bboxmin(image.getWidth() - 1, image.getHeight() - 1);
// 	Vec2i bboxmax(0, 0);
// 	Vec2i clamp(image.getWidth() - 1, image.getHeight() - 1);
// 	int maxX = 0;
// 	maxX = v0.x > v1.x ? v0.x : v1.x;
// 	if (v2.x > maxX) maxX = v2.x;
// 
// 
// 	int maxY = 0;
// 	if (v0.y > v1.y) std::swap(v0, v1);
// 	if (v0.y > v2.y) std::swap(v0, v2);
// 	if (v1.y > v2.y) std::swap(v1, v2);
// 
// 	bboxmin.raw[1] = std::max(0, std::min(bboxmin.raw[1],v2.y));
// 	bboxmax.raw[1] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
}

void CDraw::drawTriangle(Vec2i *pts, CImage &image, ImageColor color)
{
	Vec2i bboxmin(image.getWidth() - 1, image.getHeight() - 1);
	Vec2i bboxmax(0, 0);
	Vec2i clamp(image.getWidth() - 1, image.getHeight() - 1);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			bboxmin.raw[j] = std::max(0, std::min(bboxmin.raw[j], pts[i].raw[j]));
			bboxmax.raw[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
		}
	}
	std::cout << "bbox mim = " << bboxmin.x << bboxmin.y << std::endl;
	std::cout << "bbox max = " << bboxmax.x << bboxmax.y << std::endl;
	Vec2i P;
	for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
		for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
// 			Vec3f bc_screen = barycentric(pts, P);
// 			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
// 			image.set(P.x, P.y, color);
			if (pointInTriangle(pts, P))
			{
				image.set(P.x, P.y, color);
			}
		}
	}
}

//https://www.cnblogs.com/graphics/archive/2010/08/05/1793393.html
bool CDraw::pointInTriangle(Vec2i *pts, Vec2i P)
{
	Vec2i v0 = pts[1] - pts[0];
	Vec2i v1 = pts[2] - pts[0];
	Vec2i v2 = P - pts[0];

	float dot00 = v0.Dot(v0);
	float dot01 = v0.Dot(v1);
	float dot02 = v0.Dot(v2);
	float dot11 = v1.Dot(v1);
	float dot12 = v1.Dot(v2);

	//v0 = v1;

	float inverDeno = 1 / (dot00 * dot11 - dot01 * dot01);

	//float inverDeno = 1 / (dot00 * dot11 - dot01 * dot01);

	float u = (dot11 * dot02 - dot01 * dot12) * inverDeno;
	if (u < 0 || u > 1) // if u out of range, return directly
	{
		return false;
	}

	float v = (dot00 * dot12 - dot01 * dot02) * inverDeno;
	if (v < 0 || v > 1) // if v out of range, return directly
	{
		return false;
	}

	return u + v <= 1;


}
