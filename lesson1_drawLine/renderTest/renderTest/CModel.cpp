#include "CModel.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>


CModel::CModel(const char *filename) : verts_(), faces_()
{
	//��ȡobj�ļ�
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail()) return;
	std::string line;
	while (!in.eof())
	{
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v ")) //ƥ���Ƿ����v
		{
			iss >> trash;
			Vec3f v;
			for (int i = 0; i < 3; i++) iss >> v.raw[i];
			verts_.push_back(v);
		}
		else if (!line.compare(0, 2, "f "))
		{
			std::vector<int> f;
			int itrash, idx;
			iss >> trash;
			while (iss >> idx >> trash >> itrash >> trash >> itrash) //����û����
			{
				idx--; // in wavefront obj all indices start at 1, not zero //��Ϊ��ĵĶ�Ӧ������Ǵ�1��ʼ�ģ����Ǵ�0��ʼ�ģ�����Ҫͳһ��һ
				f.push_back(idx);
			}
			faces_.push_back(f);
		}
	}
	std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << std::endl;
}


CModel::~CModel()
{
}

int CModel::nverts()
{
	return (int)verts_.size();
}

int CModel::nfaces()
{
	return (int)faces_.size();
}

Vec3f CModel::vert(int i)
{
	return verts_[i];
}

std::vector<int> CModel::face(int idx)
{
	return faces_[idx];
}
