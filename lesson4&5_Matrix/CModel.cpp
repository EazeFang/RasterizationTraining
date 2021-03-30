#include "CModel.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
//#include <vector>


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
		std::string trashh;
		if (!line.compare(0, 2, "v ")) //匹配是否相等v
		{
			iss >> trash;
			Vec3f v;
			for (int i = 0; i < 3; i++) iss >> v[i];
			verts_.push_back(v);
			//std::cout << "v x= " << v.x << " y=" << v.y << " z=" << v.z << std::endl;
		}
		else if (!line.compare(0, 2, "vt"))
		{
			iss >> trashh;
			Vec3f v;
			for (int i = 0; i < 3; i++) iss >> v[i];
			vertTextureCoords_.push_back(v);
// 			std::cout << "trashh " << trashh << std::endl;
// 			std::cout << "vt x= " << v.x << " y=" << v.y << " z=" << v.z << std::endl;
		}
		else if (!line.compare(0, 2, "vn"))
		{
			iss >> trashh;
			Vec3f v;
			for (int i = 0; i < 3; i++) iss >> v[i];
			vertNoraml_.push_back(v);
//			std::cout << "vn x= " << v.x << " y=" << v.y << " z=" << v.z << std::endl;
		}
		else if (!line.compare(0, 2, "f "))
		{
			std::vector<int> f;
			std::vector<int> vt;
			std::vector<int> vn;
			int idx, vtIdx, vnIdx;
			iss >> trash;
			while (iss >> idx >> trash >> vtIdx >> trash >> vnIdx) 
			{
				idx--; // in wavefront obj all indices start at 1, not zero //因为面的的对应点序号是从1开始的，不是从0开始的，所以要统一减一
				f.push_back(idx);
				vtIdx--;
				vt.push_back(vtIdx);
				vnIdx--;
				vn.push_back(vnIdx);
			}
			faces_.push_back(f);
// 			std::cout << "vt x= " << vt[0] << " y=" << vt[1] << " z=" << vt[2] << std::endl;
// 			std::cout << "vn x= " << vn[0] << " y=" << vn[1] << " z=" << vn[2] << std::endl;
			VTIds_.push_back(vt);
			VNIds_.push_back(vn);
		}
	}
	std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << std::endl;
	std::cerr << "# vt# " << vertTextureCoords_.size() << " vn# " << vertNoraml_.size() << std::endl;
	std::cerr << "# vtids# " << VTIds_.size() << " vnids# " << VNIds_.size() << std::endl;
	//为什么纹理坐标数 比 顶点坐标数 多，不应该是一一对应的嘛。
	//事实上一个顶点可能在同时属于几个三角面，可能三角面会发生突变，那么这个顶点在几个三角面中的纹理坐标可能就不同了
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

int CModel::nvts()
{
	return (int)vertTextureCoords_.size();
}

int CModel::nvtIds()
{
	return (int)VTIds_.size();
}

int CModel::nvns()
{
	return (int)vertNoraml_.size();
}

int CModel::nvnIds()
{
	return (int)VNIds_.size();
}

Vec3f CModel::vert(int i)
{
	return verts_[i];
}

std::vector<int> CModel::face(int idx)
{
	return faces_[idx];
}

Vec3f CModel::verttexture(int i)
{
	return vertTextureCoords_[i];
}

std::vector<int> CModel::vtIdx(int idx)
{
	return VTIds_[idx];
}

Vec3f CModel::vertnormal(int i)
{
	return vertNoraml_[i];
}

std::vector<int> CModel::vnIdx(int idx)
{
	return VNIds_[idx];
}
