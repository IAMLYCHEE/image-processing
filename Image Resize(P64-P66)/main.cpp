#include<iostream>
#include<fstream>
#include"image.h"
#include<map>
#include<math.h>

using namespace std;


double cal(unsigned char a, unsigned char b,double k) {
	double result = (double)a + ((double)b - (double)a)*k;
	return result;
}

double cal(double a, double b, double k) {
	double result = (double)a + ((double)b - (double)a)*k;
	return result;
}

int writeFile(unsigned char * data, int width, int height, int channal, const char* path) {
	FILE *fp = NULL;
	if (!(fp = fopen(path, "wb"))) {
		cout << "can not write file" << endl;
		return -1;
	}
	else {
		fwrite(data, sizeof(unsigned char), width * height*channal, fp);
	}
	fclose(fp);
	return 0;
}

unsigned char* resize(int oldWidth, int oldHeight, int newWidth, int newHeight, unsigned char * data) {
	unsigned char *dataResize = (unsigned char*)malloc(sizeof(unsigned char)*newWidth*newHeight);

	double ratiow = (double)(oldWidth - 1) / ((double)(newWidth));
	double ratioh = (double)(oldHeight - 1) / (double)(newHeight);

	//for corner
	dataResize[0] = data[0];
	dataResize[(newHeight - 1)*newWidth] = data[(oldWidth - 1)*oldWidth];
	dataResize[newWidth - 1] = data[oldWidth - 1];
	dataResize[newWidth*newHeight - 1] = data[newWidth*newHeight - 1];

	for (int i = 0; i < newHeight - 1; i++) {
		for (int j = 0; j < newWidth - 1; j++) {
			//find the points for interpolation
			int lux = floor(j*ratiow);
			int luy = floor(i*ratioh);
			double kx = j*ratiow - lux;
			double ky = i*ratioh - luy;
			//retrieve the data of four corner
			unsigned char ulValue = data[luy*oldWidth + lux];
			unsigned char urValue = data[luy*oldWidth + lux + 1];
			unsigned char blValue = data[(luy + 1)*oldWidth + lux];
			unsigned char brValue = data[(luy + 1)*oldWidth + lux + 1];
			//calculation
			//direction x
			double x1 = cal(ulValue, urValue, kx);
			double x2 = cal(blValue, brValue, kx);
			double y = cal(x1, x2, ky);
			dataResize[i*newWidth + j] = (unsigned char)y;
		}
	}
	return dataResize;
}

unsigned char * reconstruct(unsigned char* redCom, unsigned char* greenCom, unsigned char* blueCom, int frameSize) {
	unsigned char* image = (unsigned char*)malloc(sizeof(unsigned char)*frameSize);
	for (int i = 0; i < frameSize; i = i + 3) {
		image[i] = redCom[i / 3];
		image[i + 1] = greenCom[i / 3];
		image[i + 2] = blueCom[i / 3];
	}
	return image;
}

int main() {
	Image image = Image(512, 512, 3, "Airplane.raw");
	int size = image.getWidth() * image.getHeight();

	unsigned char* imageData = image.imageData();
	unsigned char* imageRed = image.getRedCom();
	unsigned char* imageGreen = image.getGreenCom();
	unsigned char* imageBlue = image.getBlueCom();

	int oldWidth = image.getWidth();
	int oldHeight = image.getHeight();
	int newWidth = 650;
	int newHeight = 650;

	unsigned char* redResize = resize(oldWidth,oldHeight,newWidth,newHeight,imageRed);
	unsigned char* greenResize = resize(oldWidth, oldHeight, newWidth, newHeight, imageGreen);
	unsigned char* blueResize = resize(oldWidth, oldHeight, newWidth, newHeight, imageBlue);
	unsigned char* result = reconstruct(redResize, greenResize, blueResize,newWidth*newHeight*3);
	writeFile(result, newWidth, newHeight, 3, "resize.raw");
	getchar();
}
