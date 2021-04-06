#pragma once
#include "CShader.h"
#include "CMatrixTransformation.h"
#include <vector>
class NormalMapShader :public CShader
{
public:
	NormalMapShader();
	~NormalMapShader();

	// Í¨¹ý CShader ¼Ì³Ð
	virtual Vec4f vertex(CMatrix vertM, CMatrix modelview, CMatrix projection, CMatrix viewport, Vec3f lightDir) override;
	virtual bool fragment(Vec3f bar, ImageColor & color) override;

private:
	ImageColor getTextureUV(float u, float v);
	Vec3f getNormalUV(float u, float v);

	Vec3f getNormalTangent(float u, float v);

public:
	CImage texture_;
	CImage normalmap_;
	CImage normalmapTangent_;

	Vec3f varying_u;
	Vec3f varying_v;

	Vec3f normalTrans[3];

	Vec3f ndccoords_[3];

	Vec3f lightdir_;
};

