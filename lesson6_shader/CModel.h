#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"
class CModel
{
public:
	CModel(const char *filename);
	~CModel();
	int nverts();
	int nfaces();

	int nvts();
	int nvtIds();
	int nvns();
	int nvnIds();


	Vec3f vert(int i);
	std::vector<int> face(int idx);
	Vec3f verttexture(int i);
	std::vector<int> vtIdx(int idx);
	Vec3f vertnormal(int i);
	std::vector<int> vnIdx(int idx);

	//面id， 面中的三点id
	Vec3f getVert(int faceId, int vertInFaceId);

	Vec3f getNormal(int nfaceId, int normalInFaceId);

	Vec3f getTexUV(int uvfaceId, int uvInFaceId);

private:
	std::vector<Vec3f> verts_;
	std::vector<std::vector<int> > faces_;

	//vert texture coordination
	std::vector<Vec3f> vertTextureCoords_;
	std::vector<std::vector<int> > VTIds_;
	//vert normal
	std::vector<Vec3f> vertNoraml_;
	std::vector<std::vector<int> > VNIds_;
};

#endif //__MODEL_H__