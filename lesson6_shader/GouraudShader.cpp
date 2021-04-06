#include "GouraudShader.h"
#include <cmath>
#include <algorithm>

GouraudShader::GouraudShader()
{
}


GouraudShader::~GouraudShader()
{
}


Vec4f GouraudShader::vertex(CMatrix vertM, CMatrix modelview, CMatrix projection, CMatrix viewport, Vec3f lightDir)
{

// 	varying_intensity[nthvert] = std::max(0.f, model_->getNormal(iface, nthvert)*lightDir_);
// 	Vec3f vert = model_->getVert(iface, nthvert);
// 	CMatrix vertm = matrixTool_.v2m(vert);
// 
	Vec3f glvertex = matrixTool_.m2v(viewport * projection* modelview *vertM);

 	return Vec4f(glvertex.x, glvertex.y, glvertex.z, 1.0f);
}

bool GouraudShader::fragment(Vec3f bar, ImageColor & color)
{
	float intensity = varying_intensity * bar;
	color = ImageColor(255*intensity, 255*intensity, 255*intensity);
	return false;
}




