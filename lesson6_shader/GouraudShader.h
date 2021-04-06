#ifndef __GOURAUDSHADER_H__
#define __GOURAUDSHADER_H__

#include "CShader.h"
#include "CMatrix.h"
#include "CMatrixTransformation.h"
#include "CModel.h"
#include <vector>

class GouraudShader : public CShader
{
public:
	GouraudShader();
	~GouraudShader();


	// Í¨¹ý CShader ¼Ì³Ð
	virtual Vec4f vertex(CMatrix vertM, CMatrix modelview, CMatrix projection, CMatrix viewport, Vec3f lightDir) override;

	virtual bool fragment(Vec3f bar, ImageColor & color) override;

	

	Vec3f varying_intensity; // written by vertex shader, read by fragment shader
	CMatrixTransformation matrixTool_;
};

#endif

