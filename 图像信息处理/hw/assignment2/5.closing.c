#include <stdio.h> 
#include <stdlib.h>
#include <math.h>

#define BMPFILEHEADERLENGTH 14 //the bmp fileheader length is 14
#define MAXSIZE 10000
#define elementline 3

unsigned char Y[MAXSIZE][MAXSIZE], output_Y[MAXSIZE][MAXSIZE];
unsigned char U[MAXSIZE][MAXSIZE], output_U[MAXSIZE][MAXSIZE];
unsigned char V[MAXSIZE][MAXSIZE], output_V[MAXSIZE][MAXSIZE];

void RGB2YUV();
void YUV2RGB();
void Erosion();
void Dilation(char operation);
void Opening_Closing(char operation);
void In_BetweenDataProcessing();


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

//----------------5.closing---------------

	/*standard operations for file*/
	fpbmp = openfile("input1.bmp");  //open the source file 
	bmpDataPart(fpbmp);

	fpout = writefile("output5.bmp");  //output the file
	AddHeader2File(fpbmp, fpout);

	/*operations for the picture*/
	RGB2YUV();
	Opening_Closing('c');
	YUV2RGB();

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

void Erosion() {
	int i, i1, j, j1;
	int structure_element[elementline][elementline] = { {0} };  //suppose the structure is cross
	int center = (elementline - 1) / 2;

	//generate the cross element automatically
	for (j = 0; j < elementline; j++) {
		for (i = 0; i < elementline; i++) {
			if (j == center || i == center) {
				structure_element[j][i] = 1;
			}
			else {
				structure_element[j][i] = 0;
			}
		}
	}
	//overlay the original image
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			output_Y[j][i] = 255;
			output_U[j][i] = 0;
			output_V[j][i] = 0;
		}
	}
	//shrink a circle
	for (j = 0; j < height - elementline; j++) {
		for (i = 0; i < width - elementline; i++) {
			int flag = 0;
			for (j1 = 0; j1 < elementline; j1++) {
				for (i1 = 0; i1 < elementline; i1++) {
					if (structure_element[j1][i1] == 0) continue;
					else if (structure_element[j1][i1] == 1 && Y[j + j1][i + i1] != 0)  //not black at the same time
						flag = 1;  //not equal
				}
			}
			if (flag == 0) output_Y[j + center][i + center] = 0;
		}
	}
	printf("Erosion successed.\n");
}

void Dilation(char operation)
{
	int i, i1, j, j1;
	int structure_element[elementline][elementline] = { {0} };  //SE
	//suppose the structure is cross
	int center = (elementline - 1) / 2;

	//generate the cross element automatically
	for (j = 0; j < elementline; j++) {
		for (i = 0; i < elementline; i++) {
			if (j == center || i == center) structure_element[j][i] = 1;
			else structure_element[j][i] = 0;
		}
	}
	//overlay the original image
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			output_Y[j][i] = Y[j][i];
			output_U[j][i] = 0;
			output_V[j][i] = 0;
		}
	}
	//expand a circle
	for (j = 0; j < height - elementline; j++) {
		for (i = 0; i < width - elementline; i++) {
			int flag = 0;
			for (j1 = 0; j1 < elementline; j1++) {
				for (i1 = 0; i1 < elementline; i1++) {
					if (structure_element[j1][i1] == 1 && Y[j + j1][i + i1] == 0)  //not black at the same time
						flag = 1;  //not equal
				}
			}
			if (flag == 1) output_Y[j + center][i + center] = operation == 'd' ? 0 : 255;
		}
	}

	printf("Dilation successed.\n");
}

void Opening_Closing(char operation)
{
	if (operation == 'o') {
		Erosion();
		In_BetweenDataProcessing();
		Dilation('d');
		printf("Opening finished, now outer noise are removed.\n");
	}
	else if (operation == 'c') {
		Dilation('d');
		In_BetweenDataProcessing();
		Erosion();
		printf("Closing finished, now inner noise are removed.\n");
	}
	else {
		printf("Opening_Closing fail.\n");
	}
}

void In_BetweenDataProcessing()
{
	int i, j;
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			Y[j][i] = output_Y[j][i];
			U[j][i] = output_U[j][i];
			V[j][i] = output_V[j][i];
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
	fpbmp = fopen("output1.bmp", "rb");
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
	fpout = fopen("output5.bmp", "wb+");  //write the grayscale bmp
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
