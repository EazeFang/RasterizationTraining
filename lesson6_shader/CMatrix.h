#ifndef __CMATRIX_H__
#define __CMATRIX_H__
#include "geometry.h"
#include <vector>

class CMatrix
{
public:
	CMatrix(int rows = 4, int cols = 4);

	~CMatrix();

	int rows() { return rows_; }
	int cols() { return cols_; }

	static CMatrix identity(int dimensions);
	CMatrix transpose();
	CMatrix inverse();

	CMatrix operator*(CMatrix& a);
	std::vector<float>& operator[](const int i);

	// 4x1矩阵转化为向量，丢失w分量
	Vec3f ToVec3f() { return Vec3f(m_[0][0], m_[1][0], m_[2][0]); }
	// 4x1矩阵转化为向量，不丢失w分量
	//Vec3f ToVec4f() { return vec4f(m[0][0], m[1][0], m[2][0], m[3][0]); }

	friend std::ostream& operator<<(std::ostream& s, CMatrix& m);
private:
	int rows_;
	int cols_;
	std::vector<std::vector<float>> m_;
};

#endif //

