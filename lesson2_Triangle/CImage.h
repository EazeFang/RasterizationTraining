#ifndef CIMAGE
#define CIMAGE

#include <fstream>

//#pragma pack 指定编译器的内存对齐方式
//#pragma pack(n) 是最常见的用法 默认n==8,16 取值可以是1，2，4，8，16
//#pragma pack(show) //查看默认的对其内存 测试后值为16
//简单来说一个存储单位是8个字节，也就是说包含的结构体中 char类型 也是占用了8个字节 int类型也是占用8个字节
//整个结构体的占用内存自然也是8的倍数

//将当前的对齐字节数压入栈顶，并设置1为新的对齐字节数。
#pragma pack(push,1)
struct ROTKHeader {
	char idlength;
	char colormaptype;
	char datatypecode;
	short colormaporigin;
	short colormaplength;
	char colormapdepth;
	short x_origin;
	short y_origin;
	short width;
	short height;
	char  bitsperpixel;
	char  imagedescriptor;
};
#pragma pack(pop)


struct ImageColor {
	union {
		struct {
			unsigned char b, g, r, a;
		};
		unsigned char raw[4];
		unsigned int val;
	};
	int bytespp;

	ImageColor() : val(0), bytespp(1) {
	}

	ImageColor(unsigned char R, unsigned char G, unsigned char B, unsigned char A) : b(B), g(G), r(R), a(A), bytespp(4) {
	}

	ImageColor(int v, int bpp) : val(v), bytespp(bpp) {
	}

	ImageColor(const ImageColor &c) : val(c.val), bytespp(c.bytespp) {
	}

	ImageColor(const unsigned char *p, int bpp) : val(0), bytespp(bpp) {
		for (int i = 0; i < bpp; i++) {
			raw[i] = p[i];
		}
	}

	ImageColor & operator =(const ImageColor &c) {
		if (this != &c) {
			bytespp = c.bytespp;
			val = c.val;
		}
		return *this;
	}

};


class CImage
{
public:
	enum Format {
		GRAYSCALE = 1, RGB = 3, RGBA = 4
	};
	CImage();
	CImage(int w, int h, int bpp);//bpp bits per piexel
	CImage(const CImage &img);
	~CImage();

	bool readImage(const char *filename);
	bool writeImage(const char *filename, bool rle = true);
	//水平翻转
	bool flipHorizontally();
	//垂直翻转
	bool flipVertically();
	bool scale(int w, int h);

	//
	bool resample(int w, int h);


	ImageColor get(int x, int y);
	bool set(int x, int y, ImageColor c);
	CImage & operator =(const CImage &img);
	int getWidth();
	int getHeight();
	int getBytespp();
	unsigned char *buffer();
	void clear();
private:
	unsigned char* data;
	int width;
	int height;
	int bytesPerPixel;
	//run-length encoding rle编码，是一种压缩方式
	bool   load_rle_data(std::ifstream &in);
	bool unload_rle_data(std::ofstream &out);

};

#endif // 



