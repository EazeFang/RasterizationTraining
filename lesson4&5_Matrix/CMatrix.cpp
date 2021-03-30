#include "CMatrix.h"
#include <cassert>


CMatrix::CMatrix(int rows, int cols)
{
	rows_ = rows;
	cols_ = cols;

	m_ = std::vector<std::vector<float>>(rows_, std::vector<float>(cols_, 0.f));
}


CMatrix::~CMatrix()
{
}

CMatrix CMatrix::identity(int dimensions)
{
	CMatrix matrix(dimensions, dimensions);
	for (int i =0; i< dimensions; i++)
	{
		for (int j = 0; j < dimensions; j++)
		{
			matrix[i][j] = (i==j ? 1.0f : 0.f);
		}
	}
	return matrix;
}

CMatrix CMatrix::transpose()
{
	CMatrix trans(cols_, rows_);
	for (int i = 0; i < cols_; i++)
	{
		for (int j = 0; j < rows_; j++)
		{
			trans[i][j] = m_[j][i];
		}
	}
	return trans;
}

CMatrix CMatrix::inverse()
{
	assert(rows_ == cols_);
	CMatrix result(rows_, cols_ * 2);
	for (int i = 0; i < rows_; i++)
		for (int j = 0; j < cols_; j++)
			result[i][j] = m_[i][j];

	for (int i = 0; i < rows_; i++)
		result[i][i + cols_] = 1;

	// first pass 行消元算法
	for (int i = 0; i < rows_ - 1; i++) 
	{
		// normalize the first row
		for (int j = result.cols() - 1; j >= 0; j--)
			result[i][j] /= result[i][i];
		for (int k = i + 1; k < rows_; k++) {
			float coeff = result[k][i];
			for (int j = 0; j < result.cols(); j++) {
				result[k][j] -= result[i][j] * coeff;
			}
		}
	}

	// normalize the last row
	for (int j = result.cols() - 1; j >= rows_ - 1; j--)
		result[rows_ - 1][j] /= result[rows_ - 1][rows_ - 1];
	// second pass
	for (int i = rows_ - 1; i > 0; i--) {
		for (int k = i - 1; k >= 0; k--) {
			float coeff = result[k][i];
			for (int j = 0; j < result.cols(); j++) {
				result[k][j] -= result[i][j] * coeff;
			}
		}
	}
	//把右边的矩阵取出来就是逆矩阵
	CMatrix result1(rows_, cols_);
	for (int i = 0; i < rows_; i++)
		for (int j = 0; j < cols_; j++)
			result1[i][j] = result[i][j + cols_];

	return result1;
}

CMatrix CMatrix::operator*(CMatrix & a)
{
	assert(cols_ == a.rows());
	CMatrix result(rows_, cols_);
	for (int i = 0; i < rows_; i++) {
		for (int j = 0; j < a.cols_; j++) {
			result[i][j] = 0.f;
			for (int k = 0; k < cols_; k++) {
				result[i][j] += m_[i][k] * a[k][j];
			}
		}
	}
	return result;
}

std::vector<float>& CMatrix::operator[](const int i)
{
	// TODO: 在此处插入 return 语句
	return m_[i];
}

std::ostream & operator<<(std::ostream & s, CMatrix & m)
{
	for (int i = 0; i < m.rows(); i++) {
		for (int j = 0; j < m.cols(); j++) {
			s << m[i][j];
			if (j < m.cols() - 1) s << "\t";
		}
		s << "\n";
	}
	return s;
}
