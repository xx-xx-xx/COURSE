#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define BMPFILEHEADERLENGTH 14 //the bmp fileheader length is 14
#define MAXSIZE 10000

unsigned char Y[MAXSIZE][MAXSIZE], output_Y[MAXSIZE][MAXSIZE];
unsigned char U[MAXSIZE][MAXSIZE], output_U[MAXSIZE][MAXSIZE];
unsigned char V[MAXSIZE][MAXSIZE], output_V[MAXSIZE][MAXSIZE];

void RGB2YUV();
void YUV2Gray();
void YUV2RGB();
void Bilateralfiltering(int parameter);

double Gaussian(int radial, double lamda);
void RBF(unsigned char temp[], int i, int j, int parameter, double lamda_range, double lamda_color);

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
unsigned char records[MAXSIZE][MAXSIZE];

int main(int argc, char* argv[])
{
	/*open bmp file*/
	unsigned char *fp_temp;

	FILE *fpbmp;
	FILE *fpout;
	Initialization();

	//----------------Mean Filtering-----------------
	/*standard operations for file*/
	fpbmp = openfile("input.bmp");  //open the source file 
	bmpDataPart(fpbmp);

	fpout = writefile("output.bmp");  //output mean filtering image
	AddHeader2File(fpbmp, fpout);

	/*operations for the picture*/
	//RGB2YUV();
	//YUV2Gray();
	Bilateralfiltering(5);
	//YUV2RGB();

	bmpoutput(fpout);

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

void RGB2YUV()
{
	//convert
	int i = 0, j = 0;

	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			Y[j][i] = 0.299*R[j][i] + 0.587*G[j][i] + 0.114*B[j][i];
			U[j][i] = -0.147*R[j][i] - 0.289*G[j][i] + 0.436*B[j][i];
			V[j][i] = 0.615*R[j][i] - 0.515*G[j][i] + 0.100*B[j][i];
		}
	}
}

void YUV2Gray()
{
	int i = 0, j = 0;
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			output_Y[j][i] = Y[j][i];
			output_U[j][i] = 0;
			output_V[j][i] = 0;
		}
	}
}
void YUV2RGB()
{
	int i = 0, j = 0;
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			output_R[j][i] = output_Y[j][i] + 1.140*output_V[j][i];
			output_G[j][i] = output_Y[j][i] - 0.394*output_U[j][i] - 0.581*output_V[j][i];
			output_B[j][i] = output_Y[j][i] + 2.032*output_U[j][i];
		}
	}
}

void Bilateralfiltering(int parameter)
{
	float wr, wg, wb;	
	float d;
	float rr, rg, rb;	
	int i, j, i0, j0;

	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			if (i <= parameter / 2 || j <= parameter / 2 || i >= width - 1 - parameter / 2 || j == height - 1 - parameter / 2) {
				output_R[j][i] = R[j][i];
				output_G[j][i] = G[j][i];
				output_B[j][i] = B[j][i];
			}
			else {
				float fw_r, fw_g, fw_b;
				float totalw_r, totalw_g, totalw_b;
				fw_r = 0; fw_g = 0; fw_b = 0;
				totalw_r = 0; totalw_g = 0; totalw_b = 0;
				for (j0 = j - parameter / 2; j0 <= j + parameter / 2; j0++) {
					for (i0 = i - parameter / 2; i0 <= i - parameter / 2; i0++) {
						d = exp(-((j0 - j) ^ 2 + (i0 - i) ^ 2)*1.0 / (2 * parameter*parameter));
						rr = exp(-((R[j][i] - R[j0][i0]) ^ 2)*1.0 / (2 * parameter*parameter));
						rg = exp(-((G[j][i] - G[j0][i0]) ^ 2)*1.0 / (2 * parameter*parameter));
						rb = exp(-((B[j][i] - B[j0][i0]) ^ 2)*1.0 / (2 * parameter*parameter));
						wr = d * rr;
						wg = d * rg;
						wb = d * rb;
						fw_r = fw_r + R[j0][i0] * wr;
						fw_g = fw_g + G[j0][i0] * wg;
						fw_b = fw_b + B[j0][i0] * wb;
						totalw_r = totalw_r + wr;
						totalw_g = totalw_g + wg;
						totalw_b = totalw_b + wb;
					}
				}
				output_R[j][i] = (int)(fw_r / totalw_r);
				output_G[j][i] = (int)(fw_g / totalw_g);
				output_B[j][i] = (int)(fw_b / totalw_b);
			}
		}
	}
	
	printf("Bilateral filtering success.\n");
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
	fpbmp = fopen("input.bmp", "rb");
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
	fpout = fopen("output.bmp", "wb+");  //
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


