#include "NormalMapShader.h"

#include <algorithm>

NormalMapShader::NormalMapShader()
{
}


NormalMapShader::~NormalMapShader()
{
}

Vec4f NormalMapShader::vertex(CMatrix vertM, CMatrix modelview, CMatrix projection, CMatrix viewport, Vec3f lightDir)
{
	return Vec4f();
}

bool NormalMapShader::fragment(Vec3f bar, ImageColor & color)
{
	Vec3f bn = Vec3f(normalTrans[0] * bar, normalTrans[1] * bar, normalTrans[2] * bar).normalize();

	float u = varying_u * bar;
	float v = varying_v * bar;

	CMatrix A = CMatrix(3, 3);
	for (int i = 0; i < 3; i++)
	{
		A[0][i] = ndccoords_[1][i] - ndccoords_[0][i];
		A[1][i] = ndccoords_[2][i] - ndccoords_[0][i];
		A[2][i] = bn[i];
	}
	CMatrix AI = A.inverse();

	CMatrix tu = CMatrix(3, 1);
	tu[0][0] = varying_u[1] - varying_u[0];
	tu[1][0] = varying_u[2] - varying_u[0];
	tu[2][0] = 0;

	CMatrix tM = AI * tu;
	Vec3f t = Vec3f(tM[0][0], tM[1][0], tM[2][0]).normalize();

	CMatrix tv = CMatrix(3, 1);
	tv[0][0] = varying_v[1] - varying_v[0];
	tv[1][0] = varying_v[2] - varying_v[0];
	tv[2][0] = 0;

	CMatrix bM = AI * tv;
	Vec3f b = Vec3f(bM[0][0], bM[1][0], bM[2][0]).normalize();

	//获取normal map 图片上的颜色值，转换为坐标
// 	Vec3f normal1  = getNormalUV(u, v);

	Vec3f normal = getNormalTangent(u, v);
 	CMatrix normalM = CMatrix(3, 1);
	normalM[0][0] = normal[0];
	normalM[1][0] = normal[1];
	normalM[2][0] = normal[2];

	//
	CMatrix B = CMatrix(3, 3);
	for (int i = 0; i < 3; i++)
	{
		B[0][i] = t[i];
		B[1][i] = b[i];
		B[2][i] = bn[i];
	}
	CMatrix nM = B * normalM;//
	Vec3f n = Vec3f(nM[0][0], nM[1][0], nM[2][0]).normalize();
	//std::cout << "normalmap res =" << n << std::endl;


	float intensity = std::max(0.f, n * lightdir_);//这个平行光坐标，是dnc呢 还是世界坐标呢

	ImageColor colortemp = getTextureUV(u, v);
	color = ImageColor(colortemp.r*intensity, colortemp.g*intensity, colortemp.b*intensity);

	return false;
}

ImageColor NormalMapShader::getTextureUV(float u, float v)
{
	return texture_.get(u * texture_.getWidth(), (1.f - v) * texture_.getHeight());
}

Vec3f NormalMapShader::getNormalUV(float u, float v)//直接读取非切线坐标系下的normal map 这么个换算法
{
	ImageColor c =  normalmap_.get(u * normalmap_.getWidth(), (1.f - v) * normalmap_.getHeight());
	Vec3f res;
	res[0] = (float)c.r / 255.f;
	res[1] = (float)c.g / 255.f;
	res[2] = (float)c.b / 255.f;
	//std::cout << "normalmap =" <<res << std::endl;
	return res;
}


Vec3f NormalMapShader::getNormalTangent(float u, float v)
{
	ImageColor c = normalmapTangent_.get(u * normalmapTangent_.getWidth(), (1.f - v) * normalmapTangent_.getHeight());
	Vec3f res;
	res[0] = (float)c.r/ 255.f*2.f - 1.f;
	res[1] = (float)c.g / 255.f*2.f - 1.f;
	res[2] = (float)c.b / 255.f*2.f - 1.f;
	//std::cout << "normalmapTangent = " <<res << std::endl;
	return res;
}
