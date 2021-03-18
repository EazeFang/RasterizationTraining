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
	//����
	void drawLine(int x0, int y0, int x1, int y1, CImage &image, ImageColor color);
	void drawLine(Vec2i v0, Vec2i v1, CImage &image, ImageColor color);
	//����y��������
	void rasterize(Vec2i v0, Vec2i v1, CImage &image, ImageColor color, int ybuffer[]);
	//��������
	void drawTriangle(Vec2i v0, Vec2i v1, Vec2i v2, CImage &image, ImageColor color);
	void drawTriangle(Vec2i *pts,  CImage &image, ImageColor color);
	bool pointInTriangle(Vec2i *pts, Vec2i P);
	//���������
	void drawTriangle(Vec3f *pts, float *zbuffer, int width, int height, CImage &image, ImageColor color);
	Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P);
	bool pointInTriangle(Vec3f pt0, Vec3f pt1, Vec3f pt2, Vec3f P);
	//�������궼�������
	void drawTriangle(Vec3f *pts, Vec3f *ptTexCoords, float intensity, float *zbuffer, CImage &image, CImage &textureImage);
};

