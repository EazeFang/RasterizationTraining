#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <math.h>
#include "CImage.h"


CImage::CImage():data(NULL), width(0), height(0), bytesPerPixel(0)
{
}


CImage::CImage(int w, int h, int bpp) : data(NULL), width(w), height(h), bytesPerPixel(bpp)
{
	unsigned long nbytes = width * height * bytesPerPixel;
	data = new unsigned char[nbytes];
	memset(data, 0, nbytes);
}

CImage::CImage(const CImage &img)
{
	width = img.width;
	height = img.height;
	bytesPerPixel = img.bytesPerPixel;
	unsigned long nbytes = width * height * bytesPerPixel;
	data = new unsigned char[nbytes];
	memcpy(data, img.data, nbytes);
}

CImage::~CImage()
{
	if (data)
	{
		delete[] data;
	}
}

CImage & CImage::operator=(const CImage & img)
{
	if (this != &img) {
		if (data) delete[] data;
		width = img.width;
		height = img.height;
		bytesPerPixel = img.bytesPerPixel;
		unsigned long nbytes = width * height * bytesPerPixel;
		data = new unsigned char[nbytes];
		memcpy(data, img.data, nbytes);
	}
	return *this;
}

bool CImage::readImage(const char *filename)
{
	if (data) delete[] data;
	data = NULL;

	std::ifstream in;
	in.open(filename, std::ios::binary);
	if (!in.is_open()) {
		std::cerr << "can't open file " << filename << "\n";
		in.close();
		return false;
	}

	ROTKHeader header;
	in.read((char *)&header, sizeof(header));
	if (!in.good()) {
		in.close();
		std::cerr << "an error occured while reading the header\n";
		return false;
	}
	width = header.width;
	height = header.height;
	bytesPerPixel = header.bitsperpixel >> 3;//为什么要向右移动三位,相当于除以8取整
	if (width <= 0 || height <= 0 || (bytesPerPixel != GRAYSCALE && bytesPerPixel != RGB && bytesPerPixel != RGBA)) {
		in.close();
		std::cerr << "bad bpp (or width/height) value\n";
		return false;
	}
	unsigned long nbytes = bytesPerPixel * width * height;
	data = new unsigned char[nbytes];
	if (3 == header.datatypecode || 2 == header.datatypecode) {
		in.read((char *)data, nbytes);
		if (!in.good()) {
			in.close();
			std::cerr << "an error occured while reading the data\n";
			return false;
		}
	}
	else if (10 == header.datatypecode || 11 == header.datatypecode) {
		if (!load_rle_data(in)) {
			in.close();
			std::cerr << "an error occured while reading the data\n";
			return false;
		}
	}
	else {
		in.close();
		std::cerr << "unknown file format " << (int)header.datatypecode << "\n";
		return false;
	}
	//0x20 0x表示 16进制 换算成十进制 0x16^0 + 2x16^1 = 32
	if (!(header.imagedescriptor & 0x20)) {
		flipVertically();
	}
	//同理0x10等于16
	if (header.imagedescriptor & 0x10) {
		flipHorizontally();
	}
	std::cerr << width << "x" << height << "/" << bytesPerPixel * 8 << "\n";
	in.close();
	return true;
}

bool CImage::writeImage(const char *filename, bool rle)
{
	unsigned char developer_area_ref[4] = { 0, 0, 0, 0 };
	unsigned char extension_area_ref[4] = { 0, 0, 0, 0 };
	unsigned char footer[18] = { 'T','R','U','E','V','I','S','I','O','N','-','X','F','I','L','E','.','\0' };
	//open file
	std::ofstream out;
	out.open(filename, std::ios::binary);
	if (!out.is_open()) {
		std::cerr << "can't open file " << filename << "\n";
		out.close();
		return false;
	}

	ROTKHeader header;
	memset((void *)&header, 0, sizeof(header));
	header.bitsperpixel = bytesPerPixel << 3;//相当于乘以8
	header.width = width;
	header.height = height;
	header.datatypecode = (bytesPerPixel == GRAYSCALE ? (rle ? 11 : 3) : (rle ? 10 : 2));//是否压缩
	header.imagedescriptor = 0x20; // top-left origin 图片原点在左上角
	out.write((char *)&header, sizeof(header));
	if (!out.good()) {
		out.close();
		std::cerr << "can't dump the tga file\n";
		return false;
	}

	if (!rle) {
		out.write((char *)data, width*height*bytesPerPixel);
		if (!out.good()) {
			std::cerr << "can't unload raw data\n";
			out.close();
			return false;
		}
	}
	else {
		if (!unload_rle_data(out)) {
			out.close();
			std::cerr << "can't unload rle data\n";
			return false;
		}
	}
	out.write((char *)developer_area_ref, sizeof(developer_area_ref));
	if (!out.good()) {
		std::cerr << "can't dump the tga file\n";
		out.close();
		return false;
	}
	out.write((char *)extension_area_ref, sizeof(extension_area_ref));
	if (!out.good()) {
		std::cerr << "can't dump the tga file\n";
		out.close();
		return false;
	}
	out.write((char *)footer, sizeof(footer));
	if (!out.good()) {
		std::cerr << "can't dump the tga file\n";
		out.close();
		return false;
	}
	out.close();
	return true;
}

bool CImage::load_rle_data(std::ifstream & in)
{
	unsigned long pixelcount = width * height;
	unsigned long currentpixel = 0;
	unsigned long currentbyte = 0;
	ImageColor colorbuffer;
	do {
		unsigned char chunkheader = 0;
		chunkheader = in.get();
		if (!in.good()) {
			std::cerr << "an error occured while reading the data\n";
			return false;
		}
		if (chunkheader < 128) {
			chunkheader++;
			for (int i = 0; i < chunkheader; i++) {
				in.read((char *)colorbuffer.raw, bytesPerPixel);
				if (!in.good()) {
					std::cerr << "an error occured while reading the header\n";
					return false;
				}
				for (int t = 0; t < bytesPerPixel; t++)
					data[currentbyte++] = colorbuffer.raw[t];
				currentpixel++;
				if (currentpixel > pixelcount) {
					std::cerr << "Too many pixels read\n";
					return false;
				}
			}
		}
		else {
			chunkheader -= 127;
			in.read((char *)colorbuffer.raw, bytesPerPixel);
			if (!in.good()) {
				std::cerr << "an error occured while reading the header\n";
				return false;
			}
			for (int i = 0; i < chunkheader; i++) {
				for (int t = 0; t < bytesPerPixel; t++)
					data[currentbyte++] = colorbuffer.raw[t];
				currentpixel++;
				if (currentpixel > pixelcount) {
					std::cerr << "Too many pixels read\n";
					return false;
				}
			}
		}
	} while (currentpixel < pixelcount);
	return true;
}

bool CImage::unload_rle_data(std::ofstream & out)
{
	const unsigned char max_chunk_length = 128;
	unsigned long npixels = width * height;
	unsigned long curpix = 0;
	while (curpix < npixels) {
		unsigned long chunkstart = curpix * bytesPerPixel;
		unsigned long curbyte = curpix * bytesPerPixel;
		unsigned char run_length = 1;
		bool raw = true;
		while (curpix + run_length < npixels && run_length < max_chunk_length) {
			bool succ_eq = true;
			for (int t = 0; succ_eq && t < bytesPerPixel; t++) {
				succ_eq = (data[curbyte + t] == data[curbyte + t + bytesPerPixel]);
			}
			curbyte += bytesPerPixel;
			if (1 == run_length) {
				raw = !succ_eq;
			}
			if (raw && succ_eq) {
				run_length--;
				break;
			}
			if (!raw && !succ_eq) {
				break;
			}
			run_length++;
		}
		curpix += run_length;
		out.put(raw ? run_length - 1 : run_length + 127);
		if (!out.good()) {
			std::cerr << "can't dump the tga file\n";
			return false;
		}
		out.write((char *)(data + chunkstart), (raw ? run_length * bytesPerPixel : bytesPerPixel));
		if (!out.good()) {
			std::cerr << "can't dump the tga file\n";
			return false;
		}
	}
	return true;
}


bool CImage::flipHorizontally()
{
	if (!data) return false;
	int half = width >> 1;
	for (int i = 0; i < half; i++) {
		for (int j = 0; j < height; j++) {
			ImageColor c1 = get(i, j);
			ImageColor c2 = get(width - 1 - i, j);
			set(i, j, c2);
			set(width - 1 - i, j, c1);
		}
	}
	return true;
}

bool CImage::flipVertically()
{
// 	if (!data) return false;
// 	int half = height >> 1;
// 	for (int i = 0; i < half; i++) {
// 		for (int j = 0; j < width; j++) {
// 			ImageColor c1 = get(i, j);
// 			ImageColor c2 = get(height - 1 - i, j);
// 			set(i, j, c2);
// 			set(height - 1 - i, j, c1);
// 		}
// 	}
// 	return true;
	if (!data) return false;
	unsigned long bytes_per_line = width * bytesPerPixel;
	unsigned char *line = new unsigned char[bytes_per_line];//直接一排一排交换内存，真有你的哦
	int half = height >> 1;
	for (int j = 0; j < half; j++) {
		unsigned long l1 = j * bytes_per_line;
		unsigned long l2 = (height - 1 - j)*bytes_per_line;
		memmove((void *)line, (void *)(data + l1), bytes_per_line);
		memmove((void *)(data + l1), (void *)(data + l2), bytes_per_line);
		memmove((void *)(data + l2), (void *)line, bytes_per_line);
	}
	delete[] line;
	return true;
}

bool CImage::scale(int w, int h)
{
	if (w <= 0 || h <= 0 || !data) return false;
	unsigned char *tdata = new unsigned char[w*h*bytesPerPixel];
	int nscanline = 0;
	int oscanline = 0;
	int erry = 0;
	unsigned long nlinebytes = w * bytesPerPixel;
	unsigned long olinebytes = width * bytesPerPixel;
	for (int j = 0; j < height; j++) {
		int errx = width - w;
		int nx = -bytesPerPixel;
		int ox = -bytesPerPixel;
		for (int i = 0; i < width; i++) {
			ox += bytesPerPixel;
			errx += w;
			while (errx >= (int)width) {
				errx -= width;
				nx += bytesPerPixel;
				memcpy(tdata + nscanline + nx, data + oscanline + ox, bytesPerPixel);
			}
		}
		erry += h;
		oscanline += olinebytes;
		while (erry >= (int)height) {
			if (erry >= (int)height << 1) // it means we jump over a scanline
				memcpy(tdata + nscanline + nlinebytes, tdata + nscanline, nlinebytes);
			erry -= height;
			nscanline += nlinebytes;
		}
	}
	delete[] data;
	data = tdata;
	width = w;
	height = h;
	return true;
}

bool CImage::resample(int w, int h)
{
	if (w <= 0 || h <= 0 || !data) return false;
	unsigned char *tdata = new unsigned char[w*h*bytesPerPixel];
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			//todo线性插值
		}
	}
	double scaleWidth = (double)w / (double)width;
	double scaleHeight = (double)h / (double)height;

	for (int cy = 0; cy < h; cy++)
	{
		for (int cx = 0; cx < w; cx++)
		{
// 			int pixel = (cy * (w * 3)) + (cx * 3);
// 			int nearestMatch = (((int)(cy / scaleHeight) * (width * 3)) + ((int)(cx / scaleWidth) * 3));
// 
// 			newData[pixel] = _data[nearestMatch];
// 			newData[pixel + 1] = _data[nearestMatch + 1];
// 			newData[pixel + 2] = _data[nearestMatch + 2];
		}
	}

	delete[] data;
	data = tdata;
	width = w;
	height = h;
	return true;
}

ImageColor CImage::get(int x, int y)
{
	if (!data || x < 0 || y < 0 || x >= width || y >= height) {
		return ImageColor();
	}
	ImageColor color(data + (x + y * width)*bytesPerPixel, bytesPerPixel);//这个构造函数对吗

	return color;
}

bool CImage::set(int x, int y, ImageColor c)
{
	if (!data || x < 0 || y < 0 || x >= width || y >= height) {
		return false;
	}
	memcpy(data + (x + y * width)*bytesPerPixel, c.raw, bytesPerPixel);
	return true;
}



int CImage::getWidth()
{
	return width;
}

int CImage::getHeight()
{
	return height;
}

int CImage::getBytespp()
{
	return bytesPerPixel;
}

unsigned char * CImage::buffer()
{
	return data;
}

void CImage::clear()
{
	memset((void *)data, 0, width*height*bytesPerPixel);
}


