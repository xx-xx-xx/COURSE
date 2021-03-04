#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define BMPFILEHEADERLENGTH 14 //the bmp fileheader length is 14
#define MAXSIZE 10000
#define PAI 3.14159265358979

unsigned char Y[MAXSIZE][MAXSIZE], output_Y[MAXSIZE][MAXSIZE];
unsigned char U[MAXSIZE][MAXSIZE], output_U[MAXSIZE][MAXSIZE];
unsigned char V[MAXSIZE][MAXSIZE], output_V[MAXSIZE][MAXSIZE];

void Translation(int dx, int dy);
void Rotation(double angle);
void Scale(float ratio_x, float ratio_y);
void Shear(float dx, float dy);
void Mirror(char character);
double Gaussian(int radial, double lamda);
void RBF_interpolation(unsigned char temp[], int i, int j, int parameter);

void bmpHeaderPartLength(FILE* fpbmp);  //get the offset of header to data part
void bmpWidthHeight(FILE* fpbmp);  //get the width and height of the bmp
void bmpDataPart(FILE* fpbmp);  //get R, G, B data
void bmpoutput(FILE* fpout);  //output data to corresponding txt file

//rewrite the file
void Initialization();
FILE *openfile(char filename[]);  //open a picture
FILE *writefile1(char filename[]);  //create picture 1
FILE *writefile2(char filename[]);  //create picture 2
FILE *writefile3(char filename[]);  //create picture 3
FILE *writefile4(char filename[]);  //create picture 4
FILE *writefile5(char filename[]);  //create picture 5
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

	//--------------------1.translation------------------

	/*standard operations for file*/
	fpbmp = openfile("input.bmp");  //open the source file 
	bmpDataPart(fpbmp);

	fpout = writefile1("translation1.bmp");  //output the file
	AddHeader2File(fpbmp, fpout);

	/*operations for the picture*/
	Translation(10, 20);
	
	bmpoutput(fpout);

	/*standard operations for close*/
	fclose(fpbmp);
	fclose(fpout);
	
	//-----------------------2.rotation---------------

	/*standard operations for file*/
	fpbmp = openfile("input.bmp");  //open the source file 
	bmpDataPart(fpbmp);

	fpout = writefile2("rotation2.bmp");  //output the file
	AddHeader2File(fpbmp, fpout);

	/*operations for the picture*/
	Rotation(45.00);

	bmpoutput(fpout);

	/*standard operations for close*/
	fclose(fpbmp);
	fclose(fpout);

	//-----------------------3.scale---------------

	/*standard operations for file*/
	fpbmp = openfile("input.bmp");  //open the source file 
	bmpDataPart(fpbmp);

	fpout = writefile3("scale3.bmp");  //output the file
	AddHeader2File(fpbmp, fpout);

	/*operations for the picture*/
	Scale(2, 1);

	bmpoutput(fpout);

	/*standard operations for close*/
	fclose(fpbmp);
	fclose(fpout);

	//-----------------------4.shear---------------

	/*standard operations for file*/
	fpbmp = openfile("input.bmp");  //open the source file 
	bmpDataPart(fpbmp);

	fpout = writefile4("shear4.bmp");  //output the file
	AddHeader2File(fpbmp, fpout);

	/*operations for the picture*/
	Shear(20.0, 0);

	bmpoutput(fpout);

	/*standard operations for close*/
	fclose(fpbmp);
	fclose(fpout);
	
	//-----------------------5.mirror---------------

	/*standard operations for file*/
	fpbmp = openfile("input.bmp");  //open the source file 
	bmpDataPart(fpbmp);

	fpout = writefile5("mirror5.bmp");  //output the file
	AddHeader2File(fpbmp, fpout);

	/*operations for the picture*/
	Mirror('y');

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

void Translation(int dx, int dy)  //turn right if x is positive, up if y is positive
{
	int i, j;

	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			output_R[j][i] = R[j+dy][i-dx];
			output_G[j][i] = G[j+dy][i-dx];
			output_B[j][i] = B[j+dy][i-dx];
		}
	}
}

void Rotation(double angle)
{
	int i, j, i1, j1;
	angle = (angle - 90) / 180 * PAI;
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			i1 = sin(angle)*(width / 2 - i) + cos(angle)*(height / 2 - j);
			j1 = sin(angle)*(height / 2 - j) - cos(angle)*(width / 2 - i);
			i1 += width / 2;
			j1 += height / 2;
			records[j1][i1] = 1;

			output_R[j1][i1] = R[j][i];
			output_G[j1][i1] = G[j][i];
			output_B[j1][i1] = B[j][i];
		}
	}

	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			if (records[j][i] != 1) {
				unsigned char temp[3];
				RBF_interpolation(temp, i, j, 3);

				output_R[j][i] = temp[0];
				output_G[j][i] = temp[1];
				output_B[j][i] = temp[2];
			}
		}
	}

}
void Scale(float ratio_x, float ratio_y)
{
	int i, j, i1, j1;
	if (ratio_x <= 0 || ratio_y <= 0) {
		printf("Scaling parameter wrong!\n");
		return;
	}
	if (ratio_x <= 1 && ratio_y <= 1) {
		for (j = 0; j < height; j++) {
			for (i = 0; i < width; i++) {
				i1 = ratio_x * i;
				j1 = ratio_y * j;

				output_R[j1][i1] = R[j][i];
				output_G[j1][i1] = G[j][i];
				output_B[j1][i1] = B[j][i];
			}
		}
	}
	else if (ratio_x > 1 || ratio_y > 1) {
		for (j = 0; j < height; j++) {
			for (i = 0; i < width; i++) {
				i1 = ratio_x * i;
				j1 = ratio_y * j;

				output_R[j1][i1] = R[j][i];
				output_G[j1][i1] = G[j][i];
				output_B[j1][i1] = B[j][i];
				records[j1][i1] = 1;
			}
		}
	}

	for (j = 0; j < height*ratio_y; j++) {
		for (i = 0; i < width*ratio_x; i++) {
			if (records[j][i] == 0) {
				if (ratio_x > 1 && ratio_y > 1) {
					output_R[j][i] = R[(int)(j / ratio_y)][(int)(i / ratio_x)];
					output_G[j][i] = G[(int)(j / ratio_y)][(int)(i / ratio_x)];
					output_B[j][i] = B[(int)(j / ratio_y)][(int)(i / ratio_x)];
				}
				else if (ratio_x <= 1 && ratio_y > 1) {
					output_R[j][i] = R[(int)(j / ratio_y)][i];
					output_G[j][i] = G[(int)(j / ratio_y)][i];
					output_B[j][i] = B[(int)(j / ratio_y)][i];
				}
				else if (ratio_x > 1 && ratio_y <= 1) {
					output_R[j][i] = R[j][(int)(i / ratio_x)];
					output_G[j][i] = G[j][(int)(i / ratio_x)];
					output_B[j][i] = B[j][(int)(i / ratio_x)];
				}
			}
		}
	}
	
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			if (records[j][i] != 1) {
				unsigned char temp[3];
				RBF_interpolation(temp, i, j, 3);

				output_R[j][i] = temp[0];
				output_G[j][i] = temp[1];
				output_B[j][i] = temp[2];
			}
		}
	}
}

void Shear(float dx, float dy)
{
	int i, j, i1, j1;
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			i1 = 0;
			j1 = 0;

			i1 = i + dx * j;
			j1 = j + dy * i;			
			
			output_R[j1][i1] = R[j][i];
			output_G[j1][i1] = G[j][i];
			output_B[j1][i1] = B[j][i];	
		}
	}
	printf("Shearing completed.");
}

void Mirror(char character)
{
	int i, j;
	if (character == 'x')
		for (j = 0; j < height; j++) {
			for (i = 0; i < width; i++) {
				output_R[j][i] = R[height - j][i];
				output_G[j][i] = G[height - j][i];
				output_B[j][i] = B[height - j][i];
			}
		}
	else if(character=='y')
		for (j = 0; j < height; j++) {
			for (i = 0; i < height; i++) {
				output_R[j][i] = R[j][width-i];
				output_G[j][i] = G[j][width-i];
				output_B[j][i] = B[j][width-i];
			}
		}
	else
		printf("Input wrong!");
		
}

double Gaussian(int radial, double lamda)
{
	return exp(-pow(radial, 2) / (2 * pow(lamda, 2)));
}

void RBF_interpolation(unsigned char temp[], int i, int j, int parameter)
{
	int i1, j1;
	double w = 0.0, lamda = 0.4;  //parameter_x
	double radial, w1;
	double pixout[3] = { 0.0 };
	for (j1 = j - parameter; j1 <= j + parameter; j1++) {
		for (i1 = i - parameter; i1 <= i + parameter; i1++) {
			if (i1 == i && j1 == j)
				continue;
			radial = abs(i - i1) + abs(j - j1);
			w += Gaussian(radial, lamda);
		}
	}

	for (j1 = j - parameter; j1 <= j + parameter; j1++) {
		for (i1 = i - parameter; i1 <= i + parameter; i1++) {
			if (i1 == i && j1 == j)
				continue;
			radial = abs(i - i1) + abs(j - j1);
			w1 = Gaussian(radial, lamda) / w;

			pixout[0] += w1 * output_R[j][i];
			pixout[1] += w1 * output_G[j][i];
			pixout[2] += w1 * output_B[j][i];
		}
	}

	for (i = 0; i < 3; i++) {
		temp[i] = (unsigned char)pixout[i];  //output
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

FILE *writefile1(char filename[])
{
	FILE *fpout;
	fpout = fopen("translation1.bmp", "wb+");
	if (fpout == NULL) {
		printf("Open out bmp failed.\n");
		exit(-1);
	}
	fseek(fpout, 0L, SEEK_SET);
	return fpout;
}

FILE *writefile2(char filename[])
{
	FILE *fpout;
	fpout = fopen("rotation2.bmp", "wb+");
	if (fpout == NULL) {
		printf("Open out bmp failed.\n");
		exit(-1);
	}
	fseek(fpout, 0L, SEEK_SET);
	return fpout;
}

FILE *writefile3(char filename[])
{
	FILE *fpout;
	fpout = fopen("scale3.bmp", "wb+");
	if (fpout == NULL) {
		printf("Open out bmp failed.\n");
		exit(-1);
	}
	fseek(fpout, 0L, SEEK_SET);
	return fpout;
}

FILE *writefile4(char filename[])
{
	FILE *fpout;
	fpout = fopen("shear4.bmp", "wb+");
	if (fpout == NULL) {
		printf("Open out bmp failed.\n");
		exit(-1);
	}
	fseek(fpout, 0L, SEEK_SET);
	return fpout;
}

FILE *writefile5(char filename[])
{
	FILE *fpout;
	fpout = fopen("mirror5.bmp", "wb+");
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
