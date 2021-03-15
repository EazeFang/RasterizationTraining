#include <iostream>
#include "CImage.h"
#include "CDraw.h"
#include "CModel.h"

const ImageColor white = ImageColor(255, 255, 255, 255);
const ImageColor red = ImageColor(255, 0, 0, 255);

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

	CDraw drawfun;

	CImage image(width, height, CImage::RGB);
	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		for (int j = 0; j < 3; j++) {
			Vec3f v0 = model->vert(face[j]);
			Vec3f v1 = model->vert(face[(j + 1) % 3]);
			int x0 = (v0.x + 1.)*width / 2.;
			int y0 = (v0.y + 1.)*height / 2.;
			int x1 = (v1.x + 1.)*width / 2.;
			int y1 = (v1.y + 1.)*height / 2.;
			drawfun.drawLine(x0, y0, x1, y1, image, white);
		}
	}

// 	image.set(52, 41, red);
 	image.flipVertically(); // i want to have the origin at the left bottom corner of the image 左下角为图像原点
	image.writeImage("output.tga");

	if (model)
	{
		delete model;
	}
	system("pause");
	return 0;
}