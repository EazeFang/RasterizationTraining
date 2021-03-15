#ifndef CIMAGE
#define CIMAGE

#include <fstream>

//#pragma pack ָ�����������ڴ���뷽ʽ
//#pragma pack(n) ��������÷� Ĭ��n==8,16 ȡֵ������1��2��4��8��16
//#pragma pack(show) //�鿴Ĭ�ϵĶ����ڴ� ���Ժ�ֵΪ16
//����˵һ���洢��λ��8���ֽڣ�Ҳ����˵�����Ľṹ���� char���� Ҳ��ռ����8���ֽ� int����Ҳ��ռ��8���ֽ�
//�����ṹ���ռ���ڴ���ȻҲ��8�ı���

//����ǰ�Ķ����ֽ���ѹ��ջ����������1Ϊ�µĶ����ֽ�����
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
	//ˮƽ��ת
	bool flipHorizontally();
	//��ֱ��ת
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
	//run-length encoding rle���룬��һ��ѹ����ʽ
	bool   load_rle_data(std::ifstream &in);
	bool unload_rle_data(std::ofstream &out);

};

#endif // 



