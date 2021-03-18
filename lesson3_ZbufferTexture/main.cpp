#include <iostream>
#include "CImage.h"
#include "CDraw.h"
#include "CModel.h"
#include <vector>


const int width = 800;
const int height = 800;
Vec3f world2screen(Vec3f v) {
	return Vec3f(int((v.x + 1.)*width / 2. + .5), int((v.y + 1.)*height / 2. + .5), v.z);
}

int main(int argc, char** argv)
{
	std::cout << "wdnmd" << std::endl;

	CModel *model = NULL;

	if (2 == argc) {
		model = new CModel(argv[1]);
	}
	else {
		model = new CModel("obj/african_head.obj");
	}

	CImage textureObj;
	textureObj.readImage("obj/african_head_diffuse.tga");


 	CDraw drawfun;
// 
 	CImage image(width, height, CImage::RGB);


#pragma region textureAndNormal
	float *zbuffer = new float[width*height];
	for (int i = width * height; i--; zbuffer[i] = -std::numeric_limits<float>::max());
	//std::cout << std::numeric_limits<float>::min() <<std::endl;
 	Vec3f light_dir = Vec3f(0, 0, -1);
	for (int i = 0; i < model->nfaces(); i++) {
		//面属性，获取点坐标
		std::vector<int> face = model->face(i);
		Vec3f screen_coords[3];
		Vec3f world_coords[3];
		for (int j = 0; j < 3; j++) {
			Vec3f v = model->vert(face[j]);
			screen_coords[j] = world2screen(v);
			world_coords[j] = v;
		}

		Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
		n.normalize();
		float intensity = n * light_dir;

		if (intensity > 0) 
		{
			//纹理坐标
			std::vector<int> vtidx = model->vtIdx(i);
			Vec3f vertTextures[3];
			for (int j = 0; j < 3; j++)
			{
				vertTextures[j] = model->verttexture(vtidx[j]);
			}
// 			//法向
// 			std::vector<int> vnidx = model->vnIdx(i);
// 			Vec3f vertNormals[3];
// 			for (int j = 0; j < 3; j++)
// 			{
// 				vertNormals[j] = model->vertnormal(vnidx[j]);
// 			}

			drawfun.drawTriangle(screen_coords, vertTextures, intensity, zbuffer, image, textureObj);
		}

	}

	delete[] zbuffer;

#pragma endregion textureAndNormal


#pragma region z-buffer
// 	float *zbuffer = new float[width*height];
// 	for (int i = width * height; i--; zbuffer[i] = -std::numeric_limits<float>::max());
// 	//std::cout << std::numeric_limits<float>::min() <<std::endl;
//  	Vec3f light_dir = Vec3f(0, 0, -1);
// 	for (int i = 0; i < model->nfaces(); i++) {
// 		std::vector<int> face = model->face(i);
// 		Vec3f screen_coords[3];
// 		Vec3f world_coords[3];
// 		for (int j = 0; j < 3; j++) {
// 			Vec3f v = model->vert(face[j]);
// 			screen_coords[j] = world2screen(v);
// 			world_coords[j] = v;
// 		}
// 
// 		Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
// 		n.normalize();
// 		float intensity = n * light_dir;
// 		if (intensity > 0) 
// 		{
//  			drawfun.drawTriangle(screen_coords, zbuffer, width, height, image, ImageColor(intensity*255, intensity* 255, intensity *255, 255));
// 		}
// 	}

// 	for (int i = 0; i < model->nfaces(); i++) {
// 		std::vector<int> face = model->face(i);
// 		Vec3f pts[3];
// 		for (int i = 0; i < 3; i++) pts[i] = world2screen(model->vert(face[i]));
// 		drawfun.drawTriangle(pts, zbuffer, width, height, image, ImageColor(rand() % 255, rand() % 255, rand() % 255, 255));
// 	}
// 
// 
// 	delete[] zbuffer;
#pragma endregion z-buffer

#pragma region renderModelTriangles
// 	//法向
// 	Vec3f light_dir = Vec3f(0, 0, -1);
// 	for (int i = 0; i < model->nfaces(); i++) {
// 		std::vector<int> face = model->face(i);
// 		Vec2i screen_coords[3];
// 		Vec3f world_coords[3];
// 		for (int j = 0; j < 3; j++) {
// 			Vec3f v = model->vert(face[j]);
// 			screen_coords[j] = Vec2i((v.x + 1.)*width / 2., (v.y + 1.)*height / 2.);
// 			world_coords[j] = v;
// 		}
// 		Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);//自定义重载x积，计算法向
// 		n.normalize();
// 		float intensity = n * light_dir;
// 
// 		if (intensity > 0) {
// 			drawfun.drawTriangle(screen_coords,image, ImageColor(intensity * 255, intensity * 255, intensity * 255, 255));
// 		}
// 	}

//	//随机颜色
// 	for (int i = 0; i < model->nfaces(); i++) {
// 		std::vector<int> face = model->face(i);
// 		Vec2i screen_coords[3];
// 		for (int j = 0; j < 3; j++) {
// 			Vec3f world_coords = model->vert(face[j]);
// 			screen_coords[j] = Vec2i((world_coords.x + 1.)*width / 2., (world_coords.y + 1.)*height / 2.);
// 		}
// 		//std::cout << "screen_coords = " << screen_coords[0].x << screen_coords[1].x << screen_coords[2].x << std::endl;
// 		drawfun.drawTriangle(screen_coords, image, ImageColor(rand() % 255, rand() % 255, rand() % 255, 255));
// 	}
#pragma endregion renderModelTriangles

#pragma region renderModleLines
	//模型画线
// 	for (int i = 0; i < model->nfaces(); i++) {
// 		std::vector<int> face = model->face(i);
// 		for (int j = 0; j < 3; j++) {
// 			Vec3f v0 = model->vert(face[j]);
// 			Vec3f v1 = model->vert(face[(j + 1) % 3]);
// 			int x0 = (v0.x + 1.)*width / 2.;
// 			int y0 = (v0.y + 1.)*height / 2.;
// 			int x1 = (v1.x + 1.)*width / 2.;
// 			int y1 = (v1.y + 1.)*height / 2.;
// 			drawfun.drawLine(x0, y0, x1, y1, image, white);
// 		}
// 	}
#pragma endregion renderModleLines
	//draw triangles


	image.flipVertically(); // i want to have the origin at the left bottom corner of the image
	image.writeImage("texture2.tga");

	if (model)
	{
		delete model;
	}
	
	system("pause");
	return 0;
}