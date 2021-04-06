#include "CRender.h"
#include <algorithm>


CRender::CRender()
{
}


CRender::~CRender()
{
	if (model)
	{
		delete model;
		model = NULL;
	}

	if (zbuffer)
	{
		delete[] zbuffer;
		zbuffer = NULL;
	}
}

bool CRender::init()
{
	width_ = 800;
	height_ = 800;

	frameImage_ = CImage(width_, height_, CImage::RGB);
	zbuffer_ = CImage(width_, height_, CImage::GRAYSCALE);
	

	parameters_.directLight = Vec3f(1, 1, 1);// .normalize();

	parameters_.eyepos = Vec3f(1, 1, 3);
	parameters_.lookatcenter = Vec3f(0, 0, 0);
	parameters_.topdir = Vec3f(0, 1, 0);

	parameters_.znear = 2.f;
	parameters_.zfar = 10.f;
	parameters_.fov = 60.f;
	parameters_.aspect = 1.f;
	//load model
	model = new CModel("obj/african_head.obj");

	zbuffer = new int[width_*height_];
	for (int i = 0; i < width_*height_; i++) {
		//zbuffer[i] = std::numeric_limits<int>::min();
		zbuffer[i] = std::numeric_limits<int>::max();
	}

	texShader.texture_.readImage("obj/african_head_diffuse.tga");

	//phongShader.texture_ = texShader.texture_;

	normalShader.texture_.readImage("obj/african_head_diffuse.tga");
	normalShader.normalmap_.readImage("obj/african_head_nm.tga");
	normalShader.normalmapTangent_.readImage("obj/african_head_nm_tangent.tga");

	return true;
}

void CRender::draw()
{
	if (!model)
	{
		return;
	}
	CMatrix modelviewMatrix = matrixTools_.lookatOpenGL(parameters_.eyepos, parameters_.lookatcenter, parameters_.topdir);
	CMatrix projectionMatrix = matrixTools_.perspectiveOpenGL(parameters_.znear, parameters_.zfar, parameters_.fov, parameters_.aspect);
	CMatrix viewportMatrix = matrixTools_.viewport(0, 0, width_, height_);

	CMatrix M = viewportMatrix * projectionMatrix *modelviewMatrix;

	CMatrix mvpinverseTrans = (projectionMatrix * modelviewMatrix).inverse().transpose();

	CMatrix ligitM = matrixTools_.v2m(parameters_.directLight);
	//phongShader.lightdir_ = matrixTools_.m2v(projectionMatrix*modelviewMatrix*ligitM);

	//normalShader.lightdir_ = matrixTools_.m2v(projectionMatrix*modelviewMatrix*ligitM);
	normalShader.lightdir_ = parameters_.directLight;
	std::cout << modelviewMatrix<<std::endl;
	std::cout << projectionMatrix << std::endl;
	std::cout << viewportMatrix << std::endl;
	std::cout << M << std::endl;
	for (int i = 0; i < model->nfaces(); i++) 
	{
		std::vector<int> face = model->face(i);
		Vec3f screen_coords[3];
		//float intensity[3];
		Vec3f uv;
		Vec3f normal;
		//Vec3f ndccoords[3];
		for (int j = 0; j < 3; j++)
		{
			//这里的逐点操作其实都是vertex shader里的内容
			Vec3f v = model->getVert(i, j);
			CMatrix vM = matrixTools_.v2m(v);
			//Vec4f trv = phongShader.vertex(vM, modelviewMatrix, projectionMatrix, viewportMatrix, parameters_.directLight);
			screen_coords[j] = matrixTools_.m2v(M *vM);
			//screen_coords[j] = Vec3f(trv.x, trv.y, trv.z);
			normalShader.ndccoords_[j] = matrixTools_.m2v(projectionMatrix*modelviewMatrix*vM);
			
// 			//GouraudShader
// 			gShader.varying_intensity[j] = model->getNormal(i, j)*parameters_.directLight;
// 			//TextureShader
// 			texShader.varying_intensity[j] = model->getNormal(i, j)*parameters_.directLight;
// 			uv = model->getTexUV(i, j);
// 			texShader.varying_u[j] = uv.x;
// 			texShader.varying_v[j] = uv.y;
			//PhongShader
// 			uv = model->getTexUV(i, j);
// 			phongShader.varying_u[j] = uv.x;
// 			phongShader.varying_v[j] = uv.y;
// 
// 			normal = model->getNormal(i, j);
// 			phongShader.normalTrans[0][j] = normal.x;
// 			phongShader.normalTrans[1][j] = normal.y;
// 			phongShader.normalTrans[2][j] = normal.z;

			//normal map
			uv = model->getTexUV(i, j);
			normalShader.varying_u[j] = uv.x;
			normalShader.varying_v[j] = uv.y;

			normal = model->getNormal(i, j);
			//这里需要统一转换到ndc坐标系
			CMatrix normalM = matrixTools_.v2m(normal);
			normalM[3][0] = 0.f;
			CMatrix ndcnormal = mvpinverseTrans*normalM;
			normalShader.normalTrans[0][j] = ndcnormal[0][0];
			normalShader.normalTrans[1][j] = ndcnormal[1][0];
			normalShader.normalTrans[2][j] = ndcnormal[2][0];
			
		}
		
		drawTriangel(screen_coords, normalShader, frameImage_, zbuffer);
		//drawTriangle(screen_coords, intensity, zbuffer, frameImage_);
	}

	frameImage_.flipVertically(); // i want to have the origin at the left bottom corner of the image
	frameImage_.writeImage("frame72.tga");

// 	zbuffer_.flipVertically();
// 	zbuffer_.writeImage("zbuffer66.tga");
}

Vec3f CRender::pointInTriangleVec3(Vec3f pt0, Vec3f pt1, Vec3f pt2, Vec3f P)
{
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

	return Vec3f(1.f - (u + v), u, v);
}


void CRender::drawTriangel(Vec3f* pts, CShader &shader, CImage& image, int *zbuffer)
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
	ImageColor color;
	Vec3i P1;
	for (P1.x = bboxmin.x; P1.x <= bboxmax.x; P1.x++)
	{
		for (P1.y = bboxmin.y; P1.y <= bboxmax.y; P1.y++)
		{

			Vec3f bc_screen1 = pointInTriangleVec3(pts[0], pts[1], pts[2], P1);


			if (bc_screen1.x < 0 || bc_screen1.y < 0 || bc_screen1.z < 0) continue;
			float z = 0.f;
			for (int i = 0; i < 3; i++)
			{
				//由二维情形 int y = p0.y*(1.-t) + p1.y*t; 所以三维情形为  p0.z*(1-u-v) + p1.z*u + p2.z*v
				//三个顶点的坐标求出P点的坐标，取z的分量 P = (1-u-v)A + uB +vC
				z += pts[i][2] * bc_screen1[i];
			}
			int frag_depth = std::max(0, std::min(255, int(z + .5)));
// 			std::cout <<"z =" <<z << "frag_depth ="<<frag_depth <<std::endl;
// 			std::cout << (int)zbuffer_.get(P1.x, P1.y).b << std::endl;
			int idx = P1.x + P1.y*image.getWidth();
			if (idx >= 0 && idx < image.getWidth() * image.getHeight())
			{
				if (frag_depth < zbuffer[idx]) {
					zbuffer[idx] = frag_depth;
					bool discard = shader.fragment(bc_screen1, color);
					if (!discard) {
						//zbuffer.set(P1.x, P1.y, ImageColor(frag_depth, frag_depth, frag_depth));
						image.set(P1.x, P1.y, color);
					}
				}
			}

// 			if ((int)zbuffer_.get(P1.x, P1.y).b == 0  || frag_depth < (int)zbuffer_.get(P1.x, P1.y).b)
// 			{
// 				float normal = 0.0f;
// 				for (int i = 0; i < 3; i++)
// 				{
// 					normal += intensitys[i] * bc_screen1[i];
// 				}
// 
// 				if (normal > 0.f)
// 				{
// 					if (normal > 1.f)
// 					{
// 						normal = 1.f;
// 					}
// 					ImageColor color = ImageColor(255 * normal, 255 * normal, 255 * normal, 255);
// 					image.set(P1.x, P1.y, color);
// 				}
// 			}
		}
	}

}

// void CRender::drawTriangel(Vec3f* pts, Vec3f ndc, CShader &shader, CImage& image, int *zbuffer)
// {
// 	//重心判断
// 	Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
// 	Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
// 	Vec2f clamp(image.getWidth() - 1, image.getHeight() - 1);
// 	for (int i = 0; i < 3; i++)
// 	{
// 		for (int j = 0; j < 2; j++)
// 		{
// 			bboxmin.raw[j] = std::max(0.f, std::min(bboxmin.raw[j], pts[i][j]));
// 			bboxmax.raw[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
// 		}
// 	}
// 
// 	ImageColor color;
// 	Vec3i P1;
// 
// 	for (P1.x = bboxmin.x; P1.x <= bboxmax.x; P1.x++)
// 	{
// 		for (P1.y = bboxmin.y; P1.y <= bboxmax.y; P1.y++)
// 		{
// 
// 			Vec3f bc_screen1 = pointInTriangleVec3(pts[0], pts[1], pts[2], P1);
// 
// 
// 			if (bc_screen1.x < 0 || bc_screen1.y < 0 || bc_screen1.z < 0) continue;
// 			float z = 0.f;
// 			for (int i = 0; i < 3; i++)
// 			{
// 				//由二维情形 int y = p0.y*(1.-t) + p1.y*t; 所以三维情形为  p0.z*(1-u-v) + p1.z*u + p2.z*v
// 				//三个顶点的坐标求出P点的坐标，取z的分量 P = (1-u-v)A + uB +vC
// 				z += pts[i][2] * bc_screen1[i];
// 			}
// 			int frag_depth = std::max(0, std::min(255, int(z + .5)));
// 
// 			int idx = P1.x + P1.y*image.getWidth();
// 			if (idx >= 0 && idx < image.getWidth() * image.getHeight())
// 			{
// 				if (frag_depth < zbuffer[idx]) {
// 					zbuffer[idx] = frag_depth;
// 					bool discard = shader.fragment(bc_screen1, color);
// 					if (!discard) {
// 						//zbuffer.set(P1.x, P1.y, ImageColor(frag_depth, frag_depth, frag_depth));
// 						image.set(P1.x, P1.y, color);
// 					}
// 				}
// 			}
// 		}
// 	}
//}

void CRender::drawTriangle(Vec3f *pts, float* intensitys, int *zbuffer, CImage &image)
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
}

