#ifndef __CMATRIXTRANSFORMATION_H__
#define __CMATRIXTRANSFORMATION_H__

#include "CMatrix.h"

//const float PI = std::acosf(-1);

class CMatrixTransformation
{
public:
	CMatrixTransformation();
	~CMatrixTransformation();

	//4x1����ת��ά����
	Vec3f m2v(CMatrix m);
	//��ά����ת4x1����
	CMatrix v2m(Vec3f v) {
		CMatrix m(4, 1);
		m[0][0] = v.x;
		m[1][0] = v.y;
		m[2][0] = v.z;
		m[3][0] = 1.f;
		return m;
	}
	//ģ����ȫ������ϵ�£��ռ�任���� modelTransformation Mat
	//ƽ��
	CMatrix translation(Vec3f v);
	//����
	CMatrix scale(float factor);
	//��ת
	CMatrix rotateX(float cosx, float sinx);
	CMatrix rotateY(float cosx, float sinx);
	CMatrix rotateZ(float cosx, float sinx);

	//����任(�任���������ϵ�ı任����)viewMat���任��������ڹ۲��center
	CMatrix lookat(Vec3f eye, Vec3f center, Vec3f up);
	//�任�������ԭ�������eye
	CMatrix lookatOpenGL(Vec3f eye, Vec3f center, Vec3f up);

	//perspective ��׶ͶӰ�任
	CMatrix perspectiveOpenGL(float ZNear, float ZFar, float fovAngle, float aspect);
	//�ӿڱ任 ��ͶӰ�任���������[1-,1]*[1-,1]*[1-,1] , ӳ�䵽[x, x+width]*[y, y+height]*[0,d]
	CMatrix viewport(int x, int y, int width, int height, int depth = 255);

};

#endif

