#include<iostream>
#include<cmath>
#include"Image.h"
#include<random>
#include"Dithering.h"
#include<vector>
#include<iomanip>
using namespace std;

vector<vector<int>> C1 = { { 0,0 },{ 0,0 } };
vector<vector<int>> C2 = { { 0,0 },{ 0,85 } };
vector<vector<int>> C3 = { { 85,0 },{ 0,85 } };
vector<vector<int>> C4 = { { 85,85 },{ 0,85 } };
vector<vector<int>> C5 = { { 85,85 },{ 85,85 } };
vector<vector<int>> C6 = { { 85,85 },{ 85,170 } };
vector<vector<int>> C7 = { { 170,85 },{ 0,170 } };
vector<vector<int>> C8 = { { 170,85 },{ 85,170 } };
vector<vector<int>> C9 = { { 170,170 },{ 0,170 } };
vector<vector<int>> C10 = { { 170,170 },{ 85,170 } };
vector<vector<int>> C11 = { { 170,170 },{ 170,170 } };
vector<vector<int>> C12 = { { 170,170 },{ 85,255 } };
vector<vector<int>> C13 = { { 170,170 },{ 170,255 } };
vector<vector<int>> C14 = { { 255,170 },{ 85,255 } };
vector<vector<int>> C15 = { { 255,170 },{ 170,255 } };
vector<vector<int>> C16 = { { 255,255 },{ 85,255 } };
vector<vector<int>> C17 = { { 255,255 },{ 170,255 } };
vector<vector<int>> C18 = { { 255,255 },{ 255,255 } };

vector<vector<vector<int>>> array3d(18, vector<vector<int>>(2, vector<int>(2)));



unsigned char * matrixT4lyc(unsigned char * data, int width, int height, int channel)
{
	array3d.at(0) = C1;
	array3d.at(1) = C2;
	array3d.at(2) = C3;
	array3d.at(3) = C4;
	array3d.at(4) = C5;
	array3d.at(5) = C6;
	array3d.at(6) = C7;
	array3d.at(7) = C8;
	array3d.at(8) = C9;
	array3d.at(9) = C10;
	array3d.at(10) = C11;
	array3d.at(11) = C12;
	array3d.at(12) = C13;
	array3d.at(13) = C14;
	array3d.at(14) = C15;
	array3d.at(15) = C16;
	array3d.at(16) = C17;
	array3d.at(17) = C18;
	int dataSize = width * height*channel;
	unsigned char * lycT4 = (unsigned char*)malloc(sizeof(unsigned char)*dataSize);
	int N = 4;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int value = (int)data[i*width + j];
			int index;
			if (value < 85) {
				index = round((double)value / (85.0 / 4.0));
			}
			else if (value < 170) {
				index = round((double)(value-85) / (85.0 / 6.0))+4;
			}
			else {
				index = round((double)(value - 170) / (85.0 / 7.0)) + 10;
			}
			lycT4[i*width + j] = array3d[index][i % 2][j % 2];
		}
	}
	return lycT4;
}


int main() {
	//gather image informate
	Image image = Image(512, 512, 1, "colorchecker.raw");
	unsigned char * data = image.imageData();
	int width = image.getWidth();
	int height = image.getHeight();


	////fixed Threshold
	//unsigned char* fixedResult = Dithering::fixThre(data, width, height, 1, 127);
	//Image::writeFile(fixedResult, width, height, 1, "fixedThre.raw");

	////random Threshold
	//unsigned char* randResult = Dithering::randThre(data, width, height, 1);
	//Image::writeFile(randResult, width, height, 1, "randomThre.raw");

	//Dithering Matrix
	//I2
	//unsigned char* matrixI2 = Dithering::matrixN2(data, width, height, 1);
	//Image::writeFile(matrixI2, width, height, 1, "matrixI2.raw");
	//I4
	//unsigned char* matrixI4 = Dithering::matrixN4(data, width, height, 1);
	//Image::writeFile(matrixI4, width, height, 1, "matrixI4.raw");
	////I8
	//unsigned char* matrixI8 = Dithering::matrixN8(data, width, height, 1);
	//Image::writeFile(matrixI8, width, height, 1, "matrixI8.raw");


	//Matrix N = 2 and 4 threshold
	//unsigned char* matrixI4T4 = Dithering::matrixN4T4(data, width, height, 1);
	//Image::writeFile(matrixI4T4, width, height, 1, "matrixI4T4.raw");

	////try combine I4T2 and I4T4
	//unsigned char* matrixI4T4cI4T2 = Dithering::matrixN4T4(data, width, height, 1);
	//for (int i = 0; i < height; i++) {
	//	for (int j = 0; j < height; j++) {
	//		if ((i*width + j) % 3 != 0) {
	//			matrixI4T4cI4T2[i*width + j] = matrixI4[i*width + j];
	//		}
	//		else {
	//			matrixI4T4cI4T2[i*width + j] = matrixI4T4[i*width + j];
	//		}
	//	}
	//}
	//Image::writeFile(matrixI4T4cI4T2, width, height, 1, "matrixI4T4cI4T2m.raw");
	//result not good

/*

	unsigned char * fixedT4Result = Dithering::fixThreT4(data, width, height, 1);
	Image::writeFile(fixedT4Result, width, height, 1, "fixed4threshold.raw");
	unsigned char * matrixI4afterT4 = Dithering::matrixN4keep(fixedT4Result, width, height, 1);
	Image::writeFile(matrixI4afterT4, width, height, 1, "matrixI4afterT4.raw");*/
	//result not good

	//unsigned char* lycalgorithm = matrixT4lyc(data, width, height, 1);
	//Image::writeFile(lycalgorithm, width, height, 1, "miraclelyc.raw");

	Dithering::showKernel();

	getchar();
	return 0;
}
