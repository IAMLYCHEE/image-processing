#include"Image.h"
#include<iostream>
#include<iomanip>
#include"Utility.h"
int main() {
	Image image = Image(321, 481, 3, "Boat_noisy.raw");
	unsigned char* imageData = image.imageData();
	int width = image.getWidth();
	int height = image.getHeight();
	int size = width*height;
	//convert to gray
	unsigned char* grayImage = Image::rgb2gray(imageData, width, height);
	//LoG kernel show 
	int windowSize = 9;
	double sigma = 8.0;
	vector<double> LoGKernel = Image::genLoGKernel(windowSize, sigma);
	//for (int i = 0; i < windowSize*windowSize; i++) {
	//	cout << setw(12) << LoGKernel[i] << " ";
	//	if ((i + 1) % windowSize == 0) cout << endl;
	//	//sum = sum + LoGKernel[i];
	//}
	//cout << sum << endl;
	//LoG response
	vector<double> loGResponse = Image::filter2d(grayImage, LoGKernel, width, height, true);
	Utility::saveData(loGResponse, "logResponses8_noisy.dat");
	//get ABS response
	//loGResponse = Image::getABSResponse(loGResponse);
	//normalize
	vector<double> loGNormResponse = Image::normalize2UCRange(loGResponse);

	//check result
	unsigned char* responseImage = Image::castV2UC(loGNormResponse);
	Image::writeFile(responseImage, width, height, 1, "loGResponsew9s8_noisy.raw");

	//ternary map -1 -> 0  ;   0 -> 127   ;  1 -> 255
	double mean = 130.677;
	double var = 15.821;
	double threshold1 = mean - var/2;
	double threshold2 = mean + var/2;
	unsigned char* ternaryMap = (unsigned char*)malloc(sizeof(unsigned char)*width * height);
	for (int i = 0; i < size; i++) {
		if (loGNormResponse[i] < threshold1) {
			ternaryMap[i] = 0;
		}
		else if (loGNormResponse[i] < threshold2) {
			ternaryMap[i] = 127;
		}
		else {
			ternaryMap[i] = 255;
		}
	}
	Image::writeFile(ternaryMap, width, height, 1, "ternarys8_noisy.raw");
	cout << "ternay map write finished" << endl;
	//zero-crossing
	unsigned char* edgeMap = (unsigned char*)malloc(sizeof(unsigned char)*width*height);
	for (int i = width+1; i < size-width-1; i++) {
		edgeMap[i] = 255;// background
		if (ternaryMap[i - 1] == 0 || ternaryMap[i - width - 1] == 0 || ternaryMap[i + width - 1] == 0) {
			if (ternaryMap[i + 1] == 255 || ternaryMap[i - width + 1] == 255 || ternaryMap[i + width + 1] == 255) {
				edgeMap[i] = 0;
			}
		}else if (ternaryMap[i - 1] == 255 || ternaryMap[i - width - 1] == 255 || ternaryMap[i + width - 1] == 255) {
			if (ternaryMap[i + 1] == 0 || ternaryMap[i - width + 1] == 0 || ternaryMap[i + width + 1] == 0) {
				edgeMap[i] = 0;
			}
		}
		if (ternaryMap[i - -width -1] == 0 || ternaryMap[i - width ] == 0 || ternaryMap[i - width + 1] == 0) {
			if (ternaryMap[i + width -1] == 255 || ternaryMap[i + width ] == 255 || ternaryMap[i + width + 1] == 255) {
				edgeMap[i] = 0;
			}
		}else if (ternaryMap[i - -width - 1] == 255 || ternaryMap[i - width] == 255 || ternaryMap[i - width + 1] == 255) {
			if (ternaryMap[i + width - 1] == 0 || ternaryMap[i + width] == 0 || ternaryMap[i + width + 1] == 0) {
				edgeMap[i] = 0;
			}
		}
	}
	Image::writeFile(edgeMap, width, height, 1, "edgeMaps8_noisy.raw");
	cout << "edge Map write finished" << endl;
	getchar();
	return 0;

}
