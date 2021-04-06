#pragma once
#include "CShader.h"
#include "CMatrix.h"
#include "CMatrixTransformation.h"
#include <vector>
class PhongShader :public CShader
{
public:
	PhongShader();
	~PhongShader();

	// 通过 CShader 继承
	virtual Vec4f vertex(CMatrix vertM, CMatrix modelview, CMatrix projection, CMatrix viewport, Vec3f lightDir) override;
	virtual bool fragment(Vec3f bar, ImageColor & color) override;
private:
	ImageColor getUV(float u, float v);

public:
	Vec3f varying_u;
	Vec3f varying_v;
	//把xyz分别存在在一个vec3里，方便插值计算
	Vec3f normalTrans[3];
	CMatrixTransformation matrixTool_;

	Vec3f lightdir_;

	CImage texture_;
};

