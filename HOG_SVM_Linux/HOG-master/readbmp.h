
#ifndef MYDEF_INCLUDED
    #include "mydef.h"

#endif

#define U8 unsigned char

//ͼ������ṹ��
typedef struct IplImage_
{
	int width;
	int height;
	int width_step;
	unsigned char *image_data;
	int valid;
}IplImage_;


myMat *loadBitmapFromFile24(const char *filePath, U8 **bits);    //24λͼ

myMat *loadBitmapFromFile8(const char *filePath, U8 **bits);    //8λͼ

int CreateImage(IplImage_ *_img, int _width, int _height, U8 *_image_data);
void ReleaseImage(IplImage_ *_img);

void SaveGrayBitmap24(const char *fileName, const U8 *imageData, int width, int height); //24λͼ

void SaveGrayBitmap8(const char *fileName, const U8 *imageData, int width, int height);//8λͼ