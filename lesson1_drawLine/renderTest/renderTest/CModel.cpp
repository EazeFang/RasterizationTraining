#include "CModel.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>


CModel::CModel(const char *filename) : verts_(), faces_()
{
	//读取obj文件
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail()) return;
	std::string line;
	while (!in.eof())
	{
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v ")) //匹配是否相等v
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
			while (iss >> idx >> trash >> itrash >> trash >> itrash) //这里没看懂
			{
				idx--; // in wavefront obj all indices start at 1, not zero //因为面的的对应点序号是从1开始的，不是从0开始的，所以要统一减一
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
