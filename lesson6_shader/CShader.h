#ifndef __CSHADER_H__
#define __CSHADER_H__

#include "geometry.h"
#include "CImage.h"
#include "CMatrix.h"

class CShader
{
public:
	CShader();
	~CShader();

	virtual Vec4f vertex(CMatrix vertM, CMatrix modelview, CMatrix projection, CMatrix viewport, Vec3f lightDir) = 0;
	virtual bool fragment(Vec3f bar, ImageColor &color) = 0;


};

#endif

