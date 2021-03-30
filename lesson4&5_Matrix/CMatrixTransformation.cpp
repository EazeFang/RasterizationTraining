#include "CMatrixTransformation.h"



CMatrixTransformation::CMatrixTransformation()
{
}


CMatrixTransformation::~CMatrixTransformation()
{
}

Vec3f CMatrixTransformation::m2v(CMatrix m)
{
	return Vec3f(m[0][0] / m[3][0], m[1][0] / m[3][0], m[2][0] / m[3][0]);
}

CMatrix CMatrixTransformation::translation(Vec3f v)
{
	CMatrix Tr = CMatrix::identity(4);
	Tr[0][3] = v.x;
	Tr[1][3] = v.y;
	Tr[2][3] = v.z;
	return Tr;
}

CMatrix CMatrixTransformation::scale(float factor)
{
	CMatrix Z = CMatrix::identity(4);
	Z[0][0] = Z[1][1] = Z[2][2] = factor;
	return Z;
}

CMatrix CMatrixTransformation::rotateX(float cosx, float sinx)
{
	CMatrix rotatex = CMatrix::identity(4);
	rotatex[1][1] = cosx;
	rotatex[1][2] = -sinx;
	rotatex[2][1] = sinx;
	rotatex[2][2] = cosx;

	return rotatex;
}

CMatrix CMatrixTransformation::rotateY(float cosx, float sinx)
{
	CMatrix rotatey = CMatrix::identity(4);
	rotatey[0][0] = cosx;
	rotatey[0][2] = -sinx;
	rotatey[2][0] = sinx;
	rotatey[2][2] = cosx;

	return rotatey;
}

CMatrix CMatrixTransformation::rotateZ(float cosx, float sinx)
{
	CMatrix rotatez = CMatrix::identity(4);
	rotatez[0][0] = cosx;
	rotatez[0][1] = -sinx;
	rotatez[1][0] = sinx;
	rotatez[1][1] = cosx;
	return rotatez;
}

//其实这个坐标的变换
CMatrix CMatrixTransformation::lookat(Vec3f eye, Vec3f center, Vec3f up)
{
	//从center指向eye的方向为z的正方形
	Vec3f z = (eye - center).normalize();
	Vec3f x = cross(up, z).normalize();
	Vec3f y = cross(z, x).normalize();

	CMatrix translateM = CMatrix::identity(4);
	for (int i = 0; i < 3; i++)
	{
		translateM[i][3] = -center[i];
	}
	CMatrix rotateM = CMatrix::identity(4);
	for (int i = 0; i < 3; i++)
	{
		rotateM[0][i] = x[i];
		rotateM[1][i] = y[i];
		rotateM[2][i] = z[i];
	}

	return rotateM * translateM;

}

CMatrix CMatrixTransformation::lookatOpenGL(Vec3f eye, Vec3f center, Vec3f up)
{
	//从center指向eye的方向为z的正方形
	Vec3f z = (eye - center).normalize();
	Vec3f x = cross(up, z).normalize();
	Vec3f y = cross(z, x).normalize();

	CMatrix translateM = CMatrix::identity(4);
	for (int i = 0; i < 3; i++)
	{
		translateM[i][3] = -eye[i];
	}
	CMatrix rotateM = CMatrix::identity(4);
	for (int i = 0; i < 3; i++)
	{
		rotateM[0][i] = x[i];
		rotateM[1][i] = y[i];
		rotateM[2][i] = z[i];
	}

	return rotateM * translateM;
}

CMatrix CMatrixTransformation::perspectiveOpenGL(float ZNear, float ZFar, float fovAngle, float aspect)
{

	//计算坐标
	float n = -ZNear;											//near
	float f = -ZFar;											//far
	
	float PI = std::acosf(-1);
	float t = ZNear * std::tanf(fovAngle * 0.5f / 180.f * PI);  //top
	float b = -t;												//bottom
	//w / h = aspect
	float r = t * aspect;										//right
	float l = -r;												//left
	 


	//把视锥体变为长方体
	CMatrix perspectiveM = CMatrix::identity(4);
	perspectiveM[0][0] = n;
	perspectiveM[1][1] = n;
	perspectiveM[2][2] = n + f;
	perspectiveM[2][3] = -n*f;
	perspectiveM[3][2] = 1;

	//[l, r]*[b, t]*[n, f]缩放到边长为2的立方体[-1,1]*[-1, 1]*[-1, 1]
	CMatrix interpolateM = CMatrix::identity(4);
	interpolateM[0][0] = 2 / (r - l);
	interpolateM[0][3] = - (r + l) / (r - l);
	interpolateM[1][1] = 2 / (t - b);
	interpolateM[1][3] = -(t + b) / (t - b);
	interpolateM[2][2] = 2 / (f - n);
	interpolateM[2][3] = -(f + n) / (f - n);

	return interpolateM * perspectiveM;
}

CMatrix CMatrixTransformation::viewport(int x, int y, int width, int height, int depth)
{
	//[-1,1]*[-1, 1]*[-1, 1] 缩放到[x,x+width]*[y, y+height]*[0, depth]
	CMatrix m = CMatrix::identity(4);
	m[0][0] = width * 0.5f;
	m[0][3] = x + width * 0.5f;
	m[1][1] = height * 0.5f;
	m[1][3] = y + height * 0.5f;
	m[2][2] = depth * 0.5f;
	m[2][3] = depth * 0.5f;
	return m;
}
