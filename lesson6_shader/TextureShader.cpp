#include "TextureShader.h"

#include <algorithm>

TextureShader::TextureShader()
{
}


TextureShader::~TextureShader()
{
}

Vec4f TextureShader::vertex(CMatrix vertM, CMatrix modelview, CMatrix projection, CMatrix viewport, Vec3f lightDir)
{
	Vec3f glvertex = matrixTool_.m2v(viewport * projection* modelview *vertM);
	return Vec4f(glvertex.x, glvertex.y, glvertex.z, 1.0f);
}

bool TextureShader::fragment(Vec3f bar, ImageColor & color)
{
	float intensity = varying_intensity * bar;
	intensity = std::min(1.f, std::max(0.f, intensity));
	float u = varying_u * bar;
	float v = varying_v * bar;
	
	ImageColor colortemp = getUV(u, v);
	color = ImageColor(colortemp.r*intensity, colortemp.g*intensity, colortemp.b*intensity);
	
	return false;
}

ImageColor TextureShader::getUV(float u, float v)
{
	return texture_.get(u * texture_.getWidth(), (1 - v) * texture_.getHeight());
}
