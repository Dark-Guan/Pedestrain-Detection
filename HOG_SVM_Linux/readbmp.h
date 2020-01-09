
#ifndef MYDEF_INCLUDED
    #include "mydef.h"

#endif

#define U8 unsigned char

//图像数组结构体
typedef struct IplImage_
{
	int width;
	int height;
	int width_step;
	unsigned char *image_data;
	int valid;
}IplImage_;

myMat *loadBitmapFromFile32(const char *filePath, U8 **bits) ; //32 Bitmap

myMat *loadBitmapFromFile24(const char *filePath, U8 **bits); //24 Bitmap

myMat *loadBitmapFromFile8(const char *filePath, U8 **bits); //8 Bitmap

int CreateImage(IplImage_ *_img, int _width, int _height, U8 *_image_data);
void ReleaseImage(IplImage_ *_img);

void SaveGrayBitmap32(const char *fileName, const U8 *imageData, int width, int height); //32 bitmap

void SaveGrayBitmap24(const char *fileName, const U8 *imageData, int width, int height); //24 bitmap

void SaveGrayBitmap8(const char *fileName, const U8 *imageData, int width, int height);//8 bitmap

int GetBitCountOfBmpFile(const char *filePath);