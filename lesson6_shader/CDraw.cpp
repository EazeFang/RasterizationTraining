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

void CDraw::drawLine(Vec3f v0, Vec3f v1, CImage &image, ImageColor color)
{
	//Vec3f 

	bool steep = false;
	if (std::abs(v0.x - v1.x) < std::abs(v0.y - v1.y)) {
		std::swap(v0.x, v0.y);
		std::swap(v1.x, v1.y);
		steep = true;
	}

	if (v0.x > v1.x) {
		std::swap(v0, v1);
	}

	for (int x = (int)v0.x; x <= (int)v1.x; x++) {
		float t = (x - (int)v0.x) / (float)(v1.x - v0.x);
		int y = v0.y*(1. - t) + v1.y*t + .5;
		if (steep) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}
	}
}

void CDraw::rasterize(Vec2i v0, Vec2i v1, CImage & image, ImageColor color, int ybuffer[])
{
	if (v0.x > v1.x) {
		std::swap(v0, v1);
	}
	for (int x = v0.x; x <= v1.x; x++) {
		float t = (x - v0.x) / (float)(v1.x - v0.x);
		int y = v0.y*(1. - t) + v1.y*t + .5;
		if (ybuffer[x] < y) {
			ybuffer[x] = y;
			image.set(x, 0, color);
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
}

void CDraw::drawTriangle(Vec2i *pts, CImage &image, ImageColor color)
{
	Vec2i bboxmin(image.getWidth() - 1, image.getHeight() - 1);
	Vec2i bboxmax(0, 0);
	Vec2i clamp(image.getWidth() - 1, image.getHeight() - 1);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			bboxmin.raw[j] = std::max(0, std::min(bboxmin.raw[j], pts[i].raw[j]));
			bboxmax.raw[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i].raw[j]));
		}
	}
// 	std::cout << "bbox mim = " << bboxmin.x << bboxmin.y << std::endl;
// 	std::cout << "bbox max = " << bboxmax.x << bboxmax.y << std::endl;
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


	float inverDeno = 1 / (dot00 * dot11 - dot01 * dot01);


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

bool CDraw::pointInTriangle(Vec3f pt0, Vec3f pt1, Vec3f pt2, Vec3f P)
{
	Vec3f v0 = pt1 - pt0;
	Vec3f v1 = pt2 - pt0;
	Vec3f v2 = P - pt0;

	float dot00 = v0*v0;
	float dot01 = v0*v1;
	float dot02 = v0*v2;
	float dot11 = v1*v1;
	float dot12 = v1*v2;

	float inverDeno = 1 / (dot00 * dot11 - dot01 * dot01);

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

Vec3f CDraw::pointInTriangleVec3(Vec3f pt0, Vec3f pt1, Vec3f pt2, Vec3f P)
{
// 	Vec2i v0 = Vec2i((int)(pt1[0] - pt0[0] + 0.5f), (int)(pt1[1] - pt0[1] + 0.5f));
// 	Vec2i v1 = Vec2i((int)(pt2[0] - pt0[0] + 0.5f), (int)(pt2[1] - pt0[1] + 0.5f));
// 	Vec2i v2 = Vec2i((int)(P[0] - pt0[0] + 0.5f), (int)(P[1] - pt0[1] + 0.5f));

	Vec2f v0 = Vec2f(pt1[0] - pt0[0], pt1[1] - pt0[1]);
	Vec2f v1 = Vec2f(pt2[0] - pt0[0], pt2[1] - pt0[1]);
	Vec2f v2 = Vec2f(P[0] - pt0[0], P[1] - pt0[1]);

	float dot00 = v0 * v0;
	float dot01 = v0 * v1;
	float dot02 = v0 * v2;
	float dot11 = v1 * v1;
	float dot12 = v1 * v2;

	float inverDeno = 1 / (dot00 * dot11 - dot01 * dot01);

	float u = (dot11 * dot02 - dot01 * dot12) * inverDeno;

	float v = (dot00 * dot12 - dot01 * dot02) * inverDeno;

// 	if (v > 0 && v < 1 && u > 0 && u < 1 && (u + v) < 1)
// 	{
 		return Vec3f(1.f-(u + v), u , v);
// 	}
// 	else
// 	{
// 		return Vec3f(-1, 1, 1);
// 	}
}

Vec3f CDraw::barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P)
{
	Vec3f s[2];
	for (int i = 2; i--; ) {
		s[i][0] = C[i] - A[i];
		s[i][1] = B[i] - A[i];
		s[i][2] = A[i] - P[i];
	}
	Vec3f u = cross(s[0], s[1]);
	if (std::abs(u[2]) > 1e-5) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
		return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
	return Vec3f(-1, 1, 1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
}


void CDraw::drawTriangle(Vec3f * pts, float * zbuffer, int width, int height, CImage & image, ImageColor color)
{
	Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
	Vec2f clamp(image.getWidth() - 1, image.getHeight() - 1);
	for (int i = 0; i < 3; i++) 
	{
		for (int j = 0; j < 2; j++) 
		{
			bboxmin.raw[j] = std::max(0.f, std::min(bboxmin.raw[j], pts[i][j]));
			bboxmax.raw[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
		}
	}

// 	std::cout << "bbox mim x= " << bboxmin.x << " y=" << bboxmin.y << std::endl;
// 	std::cout << "bbox max x= " << bboxmax.x << " y=" << bboxmax.y << std::endl;
	Vec3f P;
	for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) 
	{
		for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) 
		{
			Vec3f bc_screen = barycentric(pts[0], pts[1], pts[2], P);
			if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue;
			P.z = 0;
			for (int i = 0; i < 3; i++)
			{
				//由二维情形 int y = p0.y*(1.-t) + p1.y*t; 所以三维情形为  p0.z*(1-u-v) + p1.z*u + p2.z*v
				//三个顶点的坐标求出P点的坐标，取z的分量 P = (1-u-v)A + uB +vC
				P.z += pts[i][2] * bc_screen[i];
			}
			//std::cout << "pz =" << P.z << std::endl;
			if (zbuffer[int(P.x + P.y*width)] < P.z) {
				zbuffer[int(P.x + P.y*width)] = P.z;
				image.set(P.x, P.y, color);
			}
		}
	}
}

void CDraw::drawTriangle(Vec3f *pts, Vec3f *ptTexCoords, float intensity, float *zbuffer, CImage &image, CImage &textureImage)
{
	Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
	Vec2f clamp(image.getWidth() - 1, image.getHeight() - 1);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			bboxmin.raw[j] = std::max(0.f, std::min(bboxmin.raw[j], pts[i][j]));
			bboxmax.raw[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
		}
	}

	Vec3f P;
	for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++)
	{
		for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
		{
			Vec3f bc_screen = barycentric(pts[0], pts[1], pts[2], P);
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
			P.z = 0;
			for (int i = 0; i < 3; i++)
			{
				//由二维情形 int y = p0.y*(1.-t) + p1.y*t; 所以三维情形为  p0.z*(1-u-v) + p1.z*u + p2.z*v
				//三个顶点的坐标求出P点的坐标，取z的分量 P = (1-u-v)A + uB +vC
				P.z += pts[i][2] * bc_screen[i];
			}
			if (zbuffer[int(P.x + P.y*image.getWidth())] < P.z) 
			{
				zbuffer[int(P.x + P.y*image.getWidth())] = P.z;

				//计算纹理坐标插值
				Vec2f texCoord;
				for (int i = 0; i < 3; i++)
				{
					for (int j = 0; j < 2; j++)
					{
						texCoord.raw[j] += ptTexCoords[i][j]* bc_screen[i];
					}
				}
				ImageColor textureColor = textureImage.get(texCoord[0] * textureImage.getWidth(), (1-texCoord[1]) * textureImage.getHeight());

				//image.set(P.x, P.y, ImageColor(textureColor.r*intensity, textureColor.g*intensity, textureColor.b*intensity));

// 				//计算法向插值
// 				float normal = 0.0f;
// 				for (int i = 0; i < 3; i++)
// 				{
// 					normal += ptNormals[i][2]* bc_screen[i];
// 				}
// 				float intensity = normal;// normal.normalize() * Vec3f(0, 0, -1);
//  				
  				ImageColor color = ImageColor(textureColor.r*intensity, textureColor.g * intensity, textureColor.b * intensity,255);
// 				if (P.x == 350 &&P.y == 200)
// 				{
// 					std::cout << "intensity = " << intensity << std::endl;
// 					std::cout << "ptNormals0 x= " << ptNormals[0].x << " y=" << ptNormals[0].y << " z=" << ptNormals[0].z << std::endl;
// 					std::cout << "ptNormals1 x= " << ptNormals[1].x << " y=" << ptNormals[1].y << " z=" << ptNormals[1].z << std::endl;
// 					std::cout << "ptNormals2 x= " << ptNormals[2].x << " y=" << ptNormals[2].y << " z=" << ptNormals[2].z << std::endl;
// 					std::cout << "normal  x= " << normal << std::endl;
// 
// 					std::cout << "textureColor  b= " << (int)textureColor.raw[0] << " g=" << (int)textureColor.raw[1] << " r=" << (int)textureColor.raw[2] << " a=" << (int)textureColor.raw[3]<<std::endl;
// 					//std::cout << "texCoord x= " << texCoord[0] * textureImage.getWidth() << " y=" << texCoord[1] * textureImage.getHeight() << std::endl;
// 				}
 				image.set(P.x, P.y, color);
			}
		}
	}

}

void CDraw::drawTriangle(Vec3f *pts, float* intensitys, int *zbuffer, CImage &image)
{
	//重心判断
	Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
	Vec2f clamp(image.getWidth() - 1, image.getHeight() - 1);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			bboxmin.raw[j] = std::max(0.f, std::min(bboxmin.raw[j], pts[i][j]));
			bboxmax.raw[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
		}
	}

	//Vec3f P;
	Vec3i P1;
	for (P1.x = bboxmin.x; P1.x <= bboxmax.x; P1.x++)
	{
		for (P1.y = bboxmin.y; P1.y <= bboxmax.y; P1.y++)
		{

			Vec3f bc_screen1 = pointInTriangleVec3(pts[0], pts[1], pts[2], P1);


			if (bc_screen1.x < 0 || bc_screen1.y < 0 || bc_screen1.z < 0) continue;
			P1.z = 0;
			for (int i = 0; i < 3; i++)
			{
				//由二维情形 int y = p0.y*(1.-t) + p1.y*t; 所以三维情形为  p0.z*(1-u-v) + p1.z*u + p2.z*v
				//三个顶点的坐标求出P点的坐标，取z的分量 P = (1-u-v)A + uB +vC
				P1.z += pts[i][2] * bc_screen1[i];
			}

// 			P1.x = int(P.x + 0.5f);
// 			P1.y = int(P.y + 0.5f);

			//int idx = int(P.x + P.y*image.getWidth());
			int idx = P1.x + P1.y*image.getWidth();//终于找到原因了，就是上面这里，如果使用带小数的P.y计算，最后id由于乘了image width，引起突变

			if (idx >= 0 && idx < image.getWidth() * image.getHeight())
			{
				if (P1.z < zbuffer[idx]) {
					zbuffer[idx] = P1.z;

					//计算法向插值
					float normal = 0.0f;
					for (int i = 0; i < 3; i++)
					{
						normal += intensitys[i] * bc_screen1[i];
					}
					//float intensity = normal;// normal.normalize() * Vec3f(0, 0, -1);



// 					if (P1.x == 270 && P1.y == 277)
// 					{
// 						//std::cout << "intensity = " << intensity << std::endl;
// 						std::cout << "pts0 x= " << pts[0].x << " y=" << pts[0].y << " z=" << pts[0].z << std::endl;
// 						std::cout << "pts1 x= " << pts[1].x << " y=" << pts[1].y << " z=" << pts[1].z << std::endl;
// 						std::cout << "pts2 x= " << pts[2].x << " y=" << pts[2].y << " z=" << pts[2].z << std::endl;
// 					}
					if (normal > 0.f)
					{
						if (normal > 1.f)
						{
							normal = 1.f;
						}
						ImageColor color = ImageColor(255 * normal, 255 * normal, 255 * normal, 255);
						image.set(P1.x, P1.y, color);
					}
				}
			}


		}
	}

// 	//扫描线向量法 todo 
// 	Vec3i t0 = pts[0];
// 	Vec3i t1 = pts[1];
// 	Vec3i t2 = pts[2];
// 
// 	float ity0 = intensitys[0];
// 	float ity1 = intensitys[1];
// 	float ity2 = intensitys[2];
// 
// 	if (t0.y == t1.y && t0.y == t2.y)
// 	{
// // 		ImageColor color = ImageColor(255, 255, 255, 255);
// // 		image.set(t1.x, t1.y, color);
// 		return;
// 	}// i dont care about degenerate triangles
// 		 
// 	if (t0.y > t1.y) { std::swap(t0, t1); std::swap(ity0, ity1); }
// 	if (t0.y > t2.y) { std::swap(t0, t2); std::swap(ity0, ity2); }
// 	if (t1.y > t2.y) { std::swap(t1, t2); std::swap(ity1, ity2); }
// 
// 
// 	//1.扫描线分开两半
// // 	int total_height = t2.y - t0.y;
// // 	int segment_height = t1.y - t0.y + 1;
// // 	for (int y = t0.y; y <= t1.y; y++)
// // 	{
// // 		float alpha = (float)(y - t0.y) / total_height;
// // 		float beta = (float)(y - t0.y) / segment_height; // be careful with divisions by zero 
// // 		Vec3i A = t0 + (t2 - t0)*alpha;
// // 		Vec3i B = t0 + (t1 - t0)*beta;
// // 
// // 		float ityA = ity0 + (ity2 - ity0)*alpha;
// // 		float ityB = ity0 + (ity1 - ity0)*beta;
// // 		//画一条线从A到B
// // 		if (A.x > B.x) { std::swap(A, B); std::swap(ityA, ityB); }
// // 		for (int j = A.x; j <= B.x; j++)
// // 		{
// // 			float phi = B.x == A.x ? 1. : (float)(j - A.x) / (float)(B.x - A.x);
// // 			int   Z = A.z + (B.z - A.z)*phi;
// // 			float ityP = ityA + (ityB - ityA)*phi;
// // 			int idx = j + y*image.getWidth();
// // 
// // 			if (idx >= 0 && idx < image.getWidth() * image.getHeight())
// // 			{
// // 				if (Z < zbuffer[idx])
// // 				{
// // 					zbuffer[idx] = Z;
// // 					ImageColor color = ImageColor(255 * ityP, 255 * ityP, 255 * ityP, 255);
// // 					image.set(j, y, color);
// // 				}
// // 			}
// // 		}
// // 	}
// // 	segment_height = t2.y - t1.y + 1;
// // 	for (int y = t1.y; y <= t2.y; y++)
// // 	{
// // 		float alpha = (float)(y - t0.y) / total_height;
// // 		float beta = (float)(y - t1.y) / segment_height; // be careful with divisions by zero 
// // 		Vec3i A = t0 + (t2 - t0)*alpha;
// // 		Vec3i B = t1 + (t2 - t1)*beta;
// // 
// // 		float ityA = ity0 + (ity2 - ity0)*alpha;
// // 		float ityB = ity1 + (ity2 - ity1)*beta;
// // 		if (A.x > B.x) { std::swap(A, B); std::swap(ityA, ityB); }
// // 		for (int j = A.x; j <= B.x; j++)
// // 		{
// // 			float phi = B.x == A.x ? 1. : (float)(j - A.x) / (float)(B.x - A.x);
// // 			int   Z = A.z + (B.z - A.z)*phi;
// // 			float ityP = ityA + (ityB - ityA)*phi;
// // 			int idx = j + y * image.getWidth();
// // 
// // 			if (idx >= 0 && idx < image.getWidth() * image.getHeight())
// // 			{
// // 				if (Z < zbuffer[idx])
// // 				{
// // 					zbuffer[idx] = Z;
// // 					ImageColor color = ImageColor(255 * ityP, 255 * ityP, 255 * ityP, 255);
// // 					image.set(j, y, color);
// // 				}
// // 			}
// // 		}
// // 	}
// 	//扫描线合并一起循环
// 	int total_height = t2.y - t0.y;
// 	for (int i = 0; i < total_height; i++) {
// 		bool second_half = i > t1.y - t0.y || t1.y == t0.y;
// 		int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
// 		float alpha = (float)i / total_height;
// 		float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height; // be careful: with above conditions no division by zero here
// 		Vec3i A = t0 + (t2 - t0)*alpha;
// 		Vec3i B = second_half ? t1 + (t2 - t1)*beta : t0 + (t1 - t0)*beta;
// 		float ityA = ity0 + (ity2 - ity0)*alpha;
// 		float ityB = second_half ? ity1 + (ity2 - ity1)*beta : ity0 + (ity1 - ity0)*beta;
// 		if (A.x > B.x) { std::swap(A, B); std::swap(ityA, ityB); }
// 		for (int j = A.x; j <= B.x; j++) {
// 			float phi = B.x == A.x ? 1. : (float)(j - A.x) / (float)(B.x - A.x);
// 			//Vec3i   P = A + (B - A)*phi;//计算顶点坐标插值,狗日的就是这里插值计算出了问题，才有黑边，黑点
// 			int   Z = A.z + (B.z - A.z)*phi; //只插值Z就完事了
// 			float ityP = ityA + (ityB - ityA)*phi;				//计算纹理坐标插值
// 			int idx = j + (i+t0.y)*image.getWidth();
// 
// 			if (idx >= 0 && idx < image.getWidth() * image.getHeight())
// 			{
// 				if (Z < zbuffer[idx] ) {
// 					zbuffer[idx] = Z;
// 
// 
// 					//ImageColor textureColor = textureImage.get(uvP[0] * textureImage.getWidth(), (1 - uvP[1]) * textureImage.getHeight());
// 					ImageColor color = ImageColor(255 * ityP, 255 * ityP, 255 * ityP, 255);
// 					//ImageColor color = ImageColor(255, 255, 255, 255);
// 					image.set(j, i + t0.y, color);
// 				}
// 			}
// 
// 		}
// 	}
}

