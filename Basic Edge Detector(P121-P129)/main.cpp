#include"Image.h"
#include"Utility.h"
#include<iostream>

vector<double> sobelX = { -1.0,0.0,1.0,-2.0,0.0,2.0,-1.0,0.0,1.0 };
vector<double> sobelY = { 1.0,2.0,1.0,0.0,0.0,0.0,-1.0,-2.0,-1.0 };

vector<double> magnitude(vector<double> gxRes, vector<double> gyRes) {
	vector<double> result;
	int size = gxRes.size();
	for (int i = 0; i < size; i++) {
		double magnitude = sqrt(gxRes[i] * gxRes[i] + gyRes[i] * gyRes[i]);
		result.push_back(magnitude);
	}
	return result;
}

int main() {
	// ============================clean image==============================//
	Image image = Image(481, 321, 3, "House.raw");
	unsigned char* imageData = image.imageData();
	int width = image.getWidth();
	int height = image.getHeight();
	//convert to gray
	unsigned char* grayImage = Image::rgb2gray(imageData,width,height);
	//check
	Image::writeFile(grayImage, width, height, 1, "boatGray.raw");

	//Gx
	vector<double> gxResponse = Image::filter2d(grayImage, sobelX, width, height, true);
	//get ABS response
    gxResponse = Image::getABSResponse(gxResponse);
	//normalize
	vector<double> gxNormResponse = Image::normalize2UCRange(gxResponse);
	//check result
	unsigned char* responseImage = Image::castV2UC(gxNormResponse);
	Image::writeFile(responseImage, width, height, 1, "gxResponse.raw");
	cout << "Gx write finished" << endl;

	//Gy
	vector<double> gyResponse = Image::filter2d(grayImage, sobelY, width, height, true);
	//get ABS response
	gyResponse = Image::getABSResponse(gyResponse);
	//normalize
	vector<double> gyNormResponse = Image::normalize2UCRange(gyResponse);
	//check result
	responseImage = Image::castV2UC(gyNormResponse);
	Image::writeFile(responseImage, width, height, 1, "gyResponse.raw");
	cout << "Gy write finished" << endl;

	//sqrt ( Gy * Gy + Gx * Gx)
	vector<double> magResponse = magnitude(gxResponse, gyResponse);
	//normalize
	vector<double> magNormResponse = Image::normalize2UCRange(magResponse);
	//check result
	responseImage = Image::castV2UC(magNormResponse);
	Image::writeFile(responseImage, width, height, 1, "magResponse.raw");
	cout << "mag write finished" << endl;
	Utility::saveData(magNormResponse, "magResponse.dat");

	//show the edge map
	int size = width * height;
	unsigned char* edgeMaptop5p = (unsigned char*)malloc(sizeof(unsigned char*)* width * height);
	unsigned char* edgeMaptop10p = (unsigned char*)malloc(sizeof(unsigned char*)*width * height);
	for (int i = 0; i < size; i++) {
		if (magNormResponse[i] > 37) {
			if (magNormResponse[i] > 71) {
				edgeMaptop5p[i] = 0;
			}
			else {
				edgeMaptop5p[i] = 255;
			}
			edgeMaptop10p[i] = 0;
		}
		else {
			edgeMaptop10p[i] = 255;
			edgeMaptop5p[i] = 255;
		}
	}
	Image::writeFile(edgeMaptop10p, width, height, 1, "top10pedge.raw");
	Image::writeFile(edgeMaptop5p, width, height, 1, "tep5pedge.raw");
	cout << "edge map generated!" << endl;
	////==========================noisy image ===================================//
	//Image image = Image(321, 481, 3, "Boat_noisy.raw");
	//unsigned char* imageData = image.imageData();
	//int width = image.getWidth();
	//int height = image.getHeight();
	////convert to gray
	//unsigned char* grayImage = Image::rgb2gray(imageData, width, height);
	////check
	//Image::writeFile(grayImage, width, height, 1, "boatGray_noisy.raw");

	////Gx
	//vector<double> gxResponse = Image::filter2d(grayImage, sobelX, width, height, true);
	////get ABS response
	//gxResponse = Image::getABSResponse(gxResponse);
	////normalize
	//vector<double> gxNormResponse = Image::normalize2UCRange(gxResponse);
	////check result
	//unsigned char* responseImage = Image::castV2UC(gxNormResponse);
	//Image::writeFile(responseImage, width, height, 1, "gxResponse_noisy.raw");
	//cout << "Gx write finished" << endl;

	////Gy
	//vector<double> gyResponse = Image::filter2d(grayImage, sobelY, width, height, true);
	////get ABS response
	//gyResponse = Image::getABSResponse(gyResponse);
	////normalize
	//vector<double> gyNormResponse = Image::normalize2UCRange(gyResponse);
	////check result
	//responseImage = Image::castV2UC(gyNormResponse);
	//Image::writeFile(responseImage, width, height, 1, "gyResponse_noisy.raw");
	//cout << "Gy write finished" << endl;

	////sqrt ( Gy * Gy + Gx * Gx)
	//vector<double> magResponse = magnitude(gxResponse, gyResponse);
	////normalize
	//vector<double> magNormResponse = Image::normalize2UCRange(magResponse);
	////check result
	//responseImage = Image::castV2UC(magNormResponse);
	//Image::writeFile(responseImage, width, height, 1, "magResponse_noisy.raw");
	//cout << "mag write finished" << endl;
	//Utility::saveData(magNormResponse, "magResponse_noisy.dat");

	//int size = width * height;
	//unsigned char* edgeMaptop5p = (unsigned char*)malloc(sizeof(unsigned char*)* width * height);
	//unsigned char* edgeMaptop10p = (unsigned char*)malloc(sizeof(unsigned char*)*width * height);
	//for (int i = 0; i < size; i++) {
	//	if (magNormResponse[i] > 37) {
	//		if (magNormResponse[i] > 71) {
	//			edgeMaptop5p[i] = 0;
	//		}
	//		else {
	//			edgeMaptop5p[i] = 255;
	//		}
	//		edgeMaptop10p[i] = 0;
	//	}
	//	else {
	//		edgeMaptop10p[i] = 255;
	//		edgeMaptop5p[i] = 255;
	//	}
	//}
	//Image::writeFile(edgeMaptop10p, width, height, 1, "top10pedge_noisy.raw");
	//Image::writeFile(edgeMaptop5p, width, height, 1, "tep5pedge_noisy.raw");
	//cout << "edge map generated!" << endl;
	getchar();
	return 0;
}


