#pragma once
#include "parameters.h"
#include "GouraudShader.h"
#include "TextureShader.h"
//#include "PhongShader.h"
#include "NormalMapShader.h"
#include "CImage.h"
#include "CModel.h"
#include "CMatrixTransformation.h"

class CRender
{
public:
	CRender();
	~CRender();

	bool init();

	void draw();
private:
	Vec3f pointInTriangleVec3(Vec3f pt0, Vec3f pt1, Vec3f pt2, Vec3f P);
	void drawTriangel(Vec3f* pts, CShader &shader,  CImage& image, int *zbuffer);
	//void drawTriangel(Vec3f* pts, Vec3f ndc, CShader &shader, CImage& image, int *zbuffer);

	void drawTriangle(Vec3f *pts, float* intensitys, int *zbuffer, CImage &image);
private:
	int width_;
	int height_;

	sceneParameters parameters_;

	CMatrixTransformation matrixTools_;

	GouraudShader gShader;
	TextureShader texShader;
	//PhongShader phongShader;
	NormalMapShader normalShader;

	CModel * model = NULL;

	CImage frameImage_;
	CImage zbuffer_;

	int* zbuffer = NULL;
};

