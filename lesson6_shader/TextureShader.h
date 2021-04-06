#pragma once

#include "CShader.h"
#include "CMatrix.h"
#include "CMatrixTransformation.h"
#include <vector>
class TextureShader : public CShader
{
public:
	TextureShader();
	~TextureShader();
	// Í¨¹ý CShader ¼Ì³Ð
	virtual Vec4f vertex(CMatrix vertM, CMatrix modelview, CMatrix projection, CMatrix viewport, Vec3f lightDir) override;

	virtual bool fragment(Vec3f bar, ImageColor & color) override;

private:
	ImageColor getUV(float u, float v);

public:
	Vec3f varying_intensity; // written by vertex shader, read by fragment shader
	Vec3f varying_u;
	Vec3f varying_v;
	CMatrixTransformation matrixTool_;

	CImage texture_;

};

