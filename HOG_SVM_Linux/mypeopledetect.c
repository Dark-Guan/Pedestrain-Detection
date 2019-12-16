#include "mypeopledetect.h"
#include <stdio.h>
#include <stdlib.h>

#include "readbmp.h"

myMat *readPGM(const char *filename)
{
	char buff[16];
	myMat *img;
	FILE *fp;
	int c, rgb_comp_color;
	fp = fopen(filename, "rb");
	if (!fp) {
		fprintf(stderr, "Unable to open file '%s'\n", filename);
		exit(1);
	}

	//read image format
	if (!fgets(buff, sizeof(buff), fp)) {
		perror(filename);
		exit(1);
	}

	//check the image format
	if (buff[0] != 'P' || buff[1] != '5') {
		fprintf(stderr, "Invalid PGM image format (must be 'P5')\n");
		exit(1);
	}

	//alloc memory form image
	img = (myMat*)malloc(sizeof(myMat));
	if (!img) {
		fprintf(stderr, "Unable to allocate memory\n");
		exit(1);
	}

	//check for comments
	c = getc(fp);
	while (c == '#') {
		while (getc(fp) != '\n');
		c = getc(fp);
	}

	ungetc(c, fp);
	//read image size information
	if (fscanf(fp, "%d %d", &img->x, &img->y) != 2) {
		fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
		exit(1);
	}

	//read rgb component
	if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
		fprintf(stderr, "Invalid rgb component (error loading '%s')\n", filename);
		exit(1);
	}

	//check rgb component depth
	if (rgb_comp_color != RGB_COMPONENT_COLOR) {
		fprintf(stderr, "'%s' does not have 8-bits components\n", filename);
		exit(1);
	}
	img->type = myCV_8U;
	img->dims = 2;
	img->channels = 1;
	img->step = img->channels*img->cols;
	img->totalsize = (img->step*img->rows);
	while (fgetc(fp) != '\n');
	img->data = (uchar*)malloc(img->totalsize);
	img->dataend = img->datalimit = (uchar*)img->data + img->totalsize;

	if (!img) {
		fprintf(stderr, "Unable to allocate memory\n");
		exit(1);
	}
	//read pixel data from file
	if (fread(img->data, img->channels * img->x, img->y, fp) != img->y) {
		fprintf(stderr, "Error loading image '%s'\n", filename);
		exit(1);
	}

	fclose(fp);
	return img;   
}

unsigned char networkData[800*400*3];

void ImgInverse(myMat * img, uchar * reverse_image)
{
	int height = img->height;
	int sig_width = img->step;
	int i = 0, j = 0;
	int pos = 0;
	for (i = 0; i < height; i++)
	{
		pos = i*sig_width;
		for (j = 0; j < sig_width; j++)
		{
			reverse_image[pos + j] = img->data[(height - 1 - i)*sig_width + j];
		}
	}
}

void DrawImageEdgeCorner8(myMat *img,int x,int y,int w,int h)
{
	int i, j;

	U8 *p_image = img->data;
	//����4����Ե�ߣ���Ϊ��ɫ
	//���±�Ե��
	for (i = 0; i < w;i++)
	{
		p_image[y*img->width + x + i] = 255;
		p_image[(y + h)*img->width + x + i] = 255;
	}
	for (j = 0; j < h;j++)
	{
		p_image[(y + j)*img->width + x] = 255;
		p_image[(y + j)*img->width + x + w] = 255;
	}

	//SaveGrayBitmap_("E:\\Detect_result.bmp", p_image, img->width, img->height); 

	//free(p_image);
}

void DrawImageEdgeCorner24(myMat *img, int x, int y, int w, int h)
{
	int i, j;

	U8 *p_image = img->data;
	//����4����Ե�ߣ���Ϊ��ɫ
	//���º���
	for (i = 0; i < w*3; i++)
	{
		p_image[y*img->step + x*3 + i] = 255;
		p_image[y*img->step + x*3 + i+1] = 255;
		p_image[y*img->step + x*3 + i+2] = 255;

		p_image[(y + h)*img->step + x * 3 + i] = 255;
		p_image[(y + h)*img->step + x * 3 + i + 1] = 255;
		p_image[(y + h)*img->step + x * 3 + i + 2] = 255;
	}
	for (j = 0; j < h; j++)  //���Һ���
	{
		p_image[(y + j)*img->step + x * 3] = 255;
		p_image[(y + j)*img->step + x * 3 + 1] = 255;
		p_image[(y + j)*img->step + x * 3 + 2] = 255;

		p_image[(y + j)*img->step + x * 3 + w*3] = 255;
		p_image[(y + j)*img->step + x * 3 + w*3 + 1] = 255;
		p_image[(y + j)*img->step + x * 3 + w*3 + 2] = 255;
	}

	//SaveGrayBitmap_("E:\\Detect_result.bmp", p_image, img->width, img->height); 

	//free(p_image);
}

U8 p_data[800 * 400 * 4];

void SliceImage(myMat *img, int p)
{
	U8 *p_image = img->data;
	int w = img->width;
	int h = img->height;

	int wn = w / p;   //�����Ƚ�ͼƬ�ֳ�4����
	
	int offset = 800 * 400;
	U8 *p1_img = p_data;
	U8 *p2_img = p_data + offset;
	U8 *p3_img = p_data + 2*offset;
	U8 *p4_img = p_data + 3*offset;
	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < wn; i++)
		{
			p1_img[j*wn + i] = p_image[j*w + i];
			p2_img[j*wn + i] = p_image[j*w + wn + i];
			p3_img[j*wn + i] = p_image[j*w + 2 * wn + i];
			p4_img[j*wn + i] = p_image[j*w + 3 * wn + i];
		}
	}
	SaveGrayBitmap8("E:\\Slice1.bmp", p1_img, wn, h);
	SaveGrayBitmap8("E:\\Slice2.bmp", p2_img, wn, h);
	SaveGrayBitmap8("E:\\Slice3.bmp", p3_img, wn, h);
	SaveGrayBitmap8("E:\\Slice4.bmp", p4_img, wn, h);
}


//���ļ�����
int main(int argc, char** argv)
{	
	printf("peopledetect is running....\n");
	myMat *img;

	int i;
	clock_t t;
	hog_ hog;
	rect_ *found = NULL, *found_filtered = NULL;
	size_ w, p;
	char str[128];

	char filename[128] = "G:\\Share_code\\10-test\\9-pedestrain-detection\\Pedestrain-Detection\\HOG_SVM_master\\000001.bmp"; //001IRB.bmp  //000001.bmp---24 // ---8  test_8.bmp
	strcpy(str, filename);	
	//����bmp
	uchar *image_data = NULL;
	img = loadBitmapFromFile24(filename, &image_data); //8λͼ

	uchar *reverse_image = networkData;
	ImgInverse(img, reverse_image);  //��תͼ��
	img->data = reverse_image;
	SaveGrayBitmap24("G:\\hog_bmp.bmp", img->data, img->width, img->height);


	////����pgm
	//img = readPGM(filename);
	//SaveGrayBitmap8("E:\\hog_pgm.bmp", img->data, img->width, img->height); //����pgm,���bmp,���ɱ궨����

	printf("input bmp name %s:\n", filename);
	if (!img->data)
		return -1;
	/************************************************************************/

	hogalc(&hog);
	sizealc(&w, 8, 8);
	sizealc(&p, 32, 32);
	t = clock();

	//��߶ȼ��
	//�������ͼƬimg���ж�߶����˼��
	//imgΪ���������ͼƬ��foundΪ��⵽Ŀ�������б�������?Ϊ�����ڲ�����Ϊ����Ŀ�����ֵ��Ҳ���Ǽ�⵽��������SVM���೬ƽ��ľ���?
	//����4Ϊ��������ÿ���ƶ��ľ��롣�������ǿ��ƶ���������������5Ϊͼ������Ĵ�С������?Ϊ����ϵ����������ͼƬÿ�γߴ��������ӵı�����
	//����7Ϊ����ֵ����У��ϵ������һ��Ŀ�걻������ڼ�����ʱ���ò�����ʱ�����˵������ã��?ʱ��ʾ����������á�?
	detectMultiScale(&hog, img, &found, 0, w, p, 1.05, 2, 0);
	t = clock() - t;
	printf("\nDetection Time= %fsecs\n", ((float)t) / CLOCKS_PER_SEC);
	if (sbcount(found))
	{
		for (i = 0; i < sbcount(found); i++)
		{
			printf("%s : x=%d y=%d w=%d h=%d\n", str, found[i].x, found[i].y, found[i].width, found[i].height);
			
			DrawImageEdgeCorner24(img, found[i].x, found[i].y, found[i].width, found[i].height);
		}
	}
	else
	{
		printf("%s : x=%d y=%d w=%d h=%d\n", str, 0, 0, 0, 0);
	}
	printf("try to save result\n");
	SaveGrayBitmap24("G:\\Detect_result.bmp", img->data, img->width, img->height);
	printf("save result finished\n");
	sbfree(found);
	sbfree(found_filtered);
	free(img->data);
	free(img);

	return 0;
}




////���������ļ�
//#define  FILEPATH "E:\\CLQ_C_code\\ppm\\FileNameList.txt" //�����ļ�·�����ļ����ƣ����ɷ�ʽ˫������03_ReadFileNames.bat�ļ������Զ�����
//int main(int argc, char** argv)
//{
//	myMat *img;
//
//	int i;
//	clock_t t;
//	hog_ hog;
//	rect_ *found = NULL, *found_filtered = NULL;
//	size_ w, p;
//	char str[128];
//
//	FILE* f = 0;
//	char _filename[1024];
//	// ��ȡ����ͼƬ�ļ�·��
//	f = fopen(FILEPATH, "rt");
//	if (!f)
//	{
//		fprintf(stderr, "ERROR: the specified file could not be loaded\n");
//		return 0;
//	}
//	FILE* f_save = NULL;
//	if (!(f_save=fopen("E:\\CLQ_C_code\\FileNameList.txt", "wt+")))  //���ر�����·�����Լ��趨
//	{
//		fprintf(stderr, "ERROR: the specified file could not be loaded\n");
//		return 0;
//	}
//
//	// ���ͼ�?//	for (;;)
//	{
//		// ��ȡ�ļ���
//		char* filename = _filename;
//		if (f)
//		{
//			if (!fgets(filename, (int)sizeof(_filename) - 2, f))
//				break;
//		
//			if (filename[0] == '#')
//				continue;
//
//			//ȥ���ո�
//			int l = (int)strlen(filename);
//			while (l > 0 && isspace(filename[l - 1]))
//				--l;
//			filename[l] = '\0';
//			strcpy(str, filename);
//		
//			img = readPGM(filename);
//		}
//		//printf("%s:\n", filename);
//		if (!img->data)
//			continue;
//		/************************************************************************/
//
//		hogalc(&hog);
//		sizealc(&w, 8, 8);
//		sizealc(&p, 32, 32);
//		t = clock();
//		detectMultiScale(&hog, img, &found, 0, w, p, 1.05, 2, 0);
//		t = clock() - t;
//		printf("\nDetection Time= %fsecs\n", ((float)t) / CLOCKS_PER_SEC);
//		if (sbcount(found))
//		{
//			for (i = 0; i < sbcount(found); i++)
//			{
//				printf("%s : x=%d y=%d w=%d h=%d\n", str, found[i].x, found[i].y, found[i].width, found[i].height);
//				fprintf(f_save,"%s %d %d %d %d\n", str, found[i].x, found[i].y, found[i].width, found[i].height);
//			}
//		}	
//		else
//		{
//			printf("%s : x=%d y=%d w=%d h=%d\n", str, 0, 0, 0, 0);
//			//fprintf(f_save,"%s %d %d %d\n", str,0, 0,0,0);  //���ļ�
//		}
//	} //end for
//	if(f_save)
//	{
//		fclose(f_save);
//	}
//	if (f)
//	{
//		fclose(f);
//	}	
//	sbfree(found);
//	sbfree(found_filtered);
//	free(img->data);
//	free(img);
//
//	return 0;
//}

