#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define BMPFILEHEADERLENGTH 14 //the bmp fileheader length is 14
#define MAXSIZE 10000

void HistogramEqualization_R();
void HistogramEqualization_G();
void HistogramEqualization_B();

void bmpHeaderPartLength(FILE* fpbmp);  //get the offset of header to data part
void bmpWidthHeight(FILE* fpbmp);  //get the width and height of the bmp
void bmpDataPart(FILE* fpbmp);  //get R, G, B data
void bmpoutput(FILE* fpout);  //output data to corresponding txt file

//rewrite the file
void Initialization();
FILE *openfile(char filename[]);  //open a picture
FILE *writefile(char filename[]);  //create a picture
FILE *AddHeader2File(FILE *input, FILE *output);  //add head to file

unsigned int Offset = 0;  //offset from Header part to Data part
long width;  //width of the Data part
long height;  //height of the Data part
unsigned char R[MAXSIZE][MAXSIZE], output_R[MAXSIZE][MAXSIZE];
unsigned char G[MAXSIZE][MAXSIZE], output_G[MAXSIZE][MAXSIZE];
unsigned char B[MAXSIZE][MAXSIZE], output_B[MAXSIZE][MAXSIZE];
unsigned char Y[MAXSIZE][MAXSIZE], output_Y[MAXSIZE][MAXSIZE];
unsigned char U[MAXSIZE][MAXSIZE], output_U[MAXSIZE][MAXSIZE];
unsigned char V[MAXSIZE][MAXSIZE], output_V[MAXSIZE][MAXSIZE];
unsigned char records[MAXSIZE][MAXSIZE];

int main(int argc, char* argv[])
{
	/*open bmp file*/
	unsigned char *fp_temp;

	FILE *fpbmp;
	FILE *fpout;
	Initialization();
	
	//-----------------------histogram equalization---------------

 	/*standard operations for file*/
	fpbmp = openfile("input2.bmp");  //open the source file 
	bmpDataPart(fpbmp);

	fpout = writefile("output2.bmp");  //output the file
	AddHeader2File(fpbmp, fpout);
	

	/*operations for the picture*/
	
	HistogramEqualization_R();
 	HistogramEqualization_G();
 	HistogramEqualization_B();

	bmpoutput(fpout);

	/*standard operations for close*/
	fclose(fpbmp);
	fclose(fpout);

	return 0;
}

void bmpoutput(FILE* fpout)
{
	long i, j = 0;
	long stride;
	unsigned char* pixout = NULL;

	stride = (24 * width + 31) / 8;
	stride = stride / 4 * 4;
	pixout = malloc(stride);

	fseek(fpout, Offset, SEEK_SET);

	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			pixout[i * 3 + 2] = output_R[height - 1 - j][i];
			pixout[i * 3 + 1] = output_G[height - 1 - j][i];
			pixout[i * 3] = output_B[height - 1 - j][i];
		}
		fwrite(pixout, 1, stride, fpout);
	}
}

void bmpDataPart(FILE* fpbmp)
{
	int i, j = 0;
	int stride;
	unsigned char* pix = NULL;

	fseek(fpbmp, Offset, SEEK_SET);
	stride = (24 * width + 31) / 8;
	stride = stride / 4 * 4;
	pix = malloc(stride);
	for (j = 0; j < height; j++) {
		fread(pix, 1, stride, fpbmp);

		for (i = 0; i < width; i++) {
			R[height - 1 - j][i] = pix[i * 3 + 2];
			G[height - 1 - j][i] = pix[i * 3 + 1];
			B[height - 1 - j][i] = pix[i * 3];

			output_R[height - 1 - j][i] = 255;
			output_G[height - 1 - j][i] = 255;
			output_B[height - 1 - j][i] = 255;
		}
	}
}


//get the offset of header to data part
void bmpHeaderPartLength(FILE* fpbmp)
{
	fseek(fpbmp, 10L, SEEK_SET);
	fread(&Offset, sizeof(char), 4, fpbmp);
	printf("Header Part is of length %d.\n", Offset);
}

//get the width and height of the bmp file
void bmpWidthHeight(FILE* fpbmp)
{
	int size;
	fseek(fpbmp, 18L, SEEK_SET);
	fread(&width, sizeof(char), 4, fpbmp);
	fseek(fpbmp, 2L, SEEK_SET);
	fread(&size, sizeof(char), 4, fpbmp);
	printf("The size of the bmp file is %ld.\n", size);
	fseek(fpbmp, 22L, SEEK_SET);
	fread(&height, sizeof(char), 4, fpbmp);
	printf("The width of the bmp file is %ld.\n", width);
	printf("The height of the bmp file is %ld.\n", height);
}

void HistogramEqualization_R() {
	int i, j;
	int min_number[256] = { 0 };
 	long n[256] = { 0.0 };
 	double p[256] = { 0.0 };
 	double s[256] = { 0.0 };
 	double min = 1.0;
 	double diff = 0.0;
	
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			n[R[i][j]]++;
		}
	}
		
	for (i = 0; i < 256; i++){
		p[i] = n[i] / (1.0*height*width);
	} 
	
	s[0] = p[0];
	
	for (i = 1; i < 256; i++){
		s[i] = s[i - 1] + p[i];
	}
	
	for (j = 0; j < 256; j++){
		diff = 0.0; 
		min = 1.0;
		for (i = 0; i < 256; i++){
			double pix = 1.0*i / 256;
			diff = fabs(s[j] - pix);
			if (diff < min) {
				min = diff;
				min_number[j] = i;
			}
		}
	}
	
	for (j = 0; j < height; j++){
		for (i = 0; i < width; i++){
			output_R[j][i] = min_number[R[j][i]];
		}
	}
}

void HistogramEqualization_G() {
	int i, j;
	int min_number[256] = { 0 };
 	long n[256] = { 0.0 };
 	double p[256] = { 0.0 };
 	double s[256] = { 0.0 };
 	double min = 1.0;
 	double diff = 0.0;
	
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			n[G[i][j]]++;
		}
	}
		
	for (i = 0; i < 256; i++){
		p[i] = n[i] / (1.0*height*width);
	} 
	
	s[0] = p[0];
	
	for (i = 1; i < 256; i++){
		s[i] = s[i - 1] + p[i];
	}
	
	for (j = 0; j < 256; j++){
		diff = 0.0; 
		min = 1.0;
		for (i = 0; i < 256; i++){
			double pix = 1.0*i / 256;
			diff = fabs(s[j] - pix);
			if (diff < min) {
				min = diff;
				min_number[j] = i;
			}
		}
	}
	
	for (j = 0; j < height; j++){
		for (i = 0; i < width; i++){
			output_G[j][i] = min_number[G[j][i]];
		}
	}
}
void HistogramEqualization_B() {
	int i, j;
	int min_number[256] = { 0 };
 	long n[256] = { 0.0 };
 	double p[256] = { 0.0 };
 	double s[256] = { 0.0 };
 	double min = 1.0;
 	double diff = 0.0;
	
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			n[B[i][j]]++;
		}
	}
		
	for (i = 0; i < 256; i++){
		p[i] = n[i] / (1.0*height*width);
	} 
	
	s[0] = p[0];
	
	for (i = 1; i < 256; i++){
		s[i] = s[i - 1] + p[i];
	}
	
	for (j = 0; j < 256; j++){
		diff = 0.0; 
		min = 1.0;
		for (i = 0; i < 256; i++){
			double pix = 1.0*i / 256;
			diff = fabs(s[j] - pix);
			if (diff < min) {
				min = diff;
				min_number[j] = i;
			}
		}
	}
	
	for (j = 0; j < height; j++){
		for (i = 0; i < width; i++){
			output_B[j][i] = min_number[B[j][i]];
		}
	}
}
//functions for files
void Initialization()
{
	int i = 0, j = 0;
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			output_Y[i][j] = 255;
			output_U[j][i] = 0;
			output_V[j][i] = 0;
			records[j][i] = 0;
		}
	}
}

FILE *openfile(char filename[])
{
	FILE *fpbmp;
	fpbmp = fopen("input2.bmp", "rb");
	if (fpbmp == NULL) {
		printf("Open bmp failed!\n");
		exit(-1);
	}


	bmpHeaderPartLength(fpbmp);
	bmpWidthHeight(fpbmp);

	fseek(fpbmp, 0L, SEEK_SET);
	return fpbmp;
}
FILE *writefile(char filename[])
{
	FILE *fpout;
	fpout = fopen("output2.bmp", "wb+");

	if (fpout == NULL) {
		printf("Open out bmp failed.\n");
		exit(-1);
	}
	fseek(fpout, 0L, SEEK_SET);
	return fpout;
}

FILE *AddHeader2File(FILE *input, FILE *output)
{
	unsigned char *fp_temp;

	fseek(input, 0L, SEEK_SET);
	fseek(output, 0L, SEEK_SET);

	fp_temp = malloc(Offset);
	fread(fp_temp, 1, Offset, input);  //read the head file
	fp_temp[18] = (int)width;
	fp_temp[22] = (int)height;
	fp_temp[2] = (int)(Offset + height * width * 3);
	fp_temp[34] = (int)(height*((24 * width / 8 + 3) / 4 * 4));

	fwrite(fp_temp, 1, Offset, output);
}
