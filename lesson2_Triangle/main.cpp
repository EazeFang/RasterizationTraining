#include <iostream>
#include "CImage.h"
#include "CDraw.h"
#include "CModel.h"
#include <vector>
#include "temptest.h"


const int width = 800;
const int height = 800;

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
// 
 	CDraw drawfun;
// 
 	CImage image(width, height, CImage::RGB);

	Vec3f light_dir(0, 0, -1);
	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		Vec2i screen_coords[3];
		Vec3f world_coords[3];
		for (int j = 0; j < 3; j++) {
			Vec3f v = model->vert(face[j]);
			screen_coords[j] = Vec2i((v.x + 1.)*width / 2., (v.y + 1.)*height / 2.);
			world_coords[j] = v;
		}
		Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);//自定义重载x积，计算法向
		n.normalize();
		float intensity = n * light_dir;

		if (intensity > 0) {
			drawfun.drawTriangle(screen_coords[0], screen_coords[1], screen_coords[2], image, ImageColor(intensity * 255, intensity * 255, intensity * 255, 255));
		}
		//drawfun.drawTriangle(screen_coords[0], screen_coords[1], screen_coords[2], image, ImageColor(rand() % 255, rand() % 255, rand() % 255, 255));
	}
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


 	image.flipVertically(); // i want to have the origin at the left bottom corner of the image 左下角为图像原点
	image.writeImage("framebuffer.tga");

	if (model)
	{
		delete model;
	}
	system("pause");
	return 0;
}