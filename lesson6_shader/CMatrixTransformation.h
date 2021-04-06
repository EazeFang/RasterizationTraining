#ifndef __CMATRIXTRANSFORMATION_H__
#define __CMATRIXTRANSFORMATION_H__

#include "CMatrix.h"

//const float PI = std::acosf(-1);

class CMatrixTransformation
{
public:
	CMatrixTransformation();
	~CMatrixTransformation();

	//4x1矩阵转三维向量
	Vec3f m2v(CMatrix m);
	//三维向量转4x1矩阵
	CMatrix v2m(Vec3f v) {
		CMatrix m(4, 1);
		m[0][0] = v.x;
		m[1][0] = v.y;
		m[2][0] = v.z;
		m[3][0] = 1.f;
		return m;
	}
	//模型在全局坐标系下，空间变换矩阵 modelTransformation Mat
	//平移
	CMatrix translation(Vec3f v);
	//缩放
	CMatrix scale(float factor);
	//旋转
	CMatrix rotateX(float cosx, float sinx);
	CMatrix rotateY(float cosx, float sinx);
	CMatrix rotateZ(float cosx, float sinx);

	//相机变换(变换到相机坐标系的变换矩阵)viewMat，变换后的坐标在观察点center
	CMatrix lookat(Vec3f eye, Vec3f center, Vec3f up);
	//变换后的坐标原点在相机eye
	CMatrix lookatOpenGL(Vec3f eye, Vec3f center, Vec3f up);

	//perspective 视锥投影变换
	CMatrix perspectiveOpenGL(float ZNear, float ZFar, float fovAngle, float aspect);
	//视口变换 把投影变换后的立方体[1-,1]*[1-,1]*[1-,1] , 映射到[x, x+width]*[y, y+height]*[0,d]
	CMatrix viewport(int x, int y, int width, int height, int depth = 255);

};

#endif

