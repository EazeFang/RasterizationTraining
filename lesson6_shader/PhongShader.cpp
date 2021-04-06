#include "PhongShader.h"

#include <algorithm>

PhongShader::PhongShader()
{
}


PhongShader::~PhongShader()
{
}

Vec4f PhongShader::vertex(CMatrix vertM, CMatrix modelview, CMatrix projection, CMatrix viewport, Vec3f lightDir)
{
	Vec3f glvertex = matrixTool_.m2v(viewport * projection* modelview *vertM);
	return Vec4f(glvertex.x, glvertex.y, glvertex.z, 1.0f);
}

bool PhongShader::fragment(Vec3f bar, ImageColor & color)
{
	Vec3f bn = Vec3f(normalTrans[0] * bar, normalTrans[1] * bar, normalTrans[2] * bar).normalize();
	
 	float intensity = std::max(0.f, bn * lightdir_);
// 	intensity = std::min(1.f, std::max(0.f, intensity));
	float u = varying_u * bar;
	float v = varying_v * bar;
// 	//ImageColor(255 * intensity, 155 * intensity, 0 * intensity);
 	ImageColor colortemp = getUV(u, v);
 	color = ImageColor(colortemp.r*intensity, colortemp.g*intensity, colortemp.b*intensity);
	return false;
}

ImageColor PhongShader::getUV(float u, float v)
{
	return texture_.get(u * texture_.getWidth(), (1 - v) * texture_.getHeight());
}
