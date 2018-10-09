#pragma once
//author Li Yicheng 1/17/2018
//no distribution without permission
#ifndef IMAGE_H_
#define IMAGE_H_

#include<iostream>
#include<fstream>	
#include<vector>
#include<algorithm>
#include<cmath>
using namespace std;
class Image {
private:
	int width;
	int height;
	int channel;
	const char* filename;
	long frameSize;
	unsigned char* imagedata = NULL;
public:
	Image(int w, int h, int channel, const char* path);

	unsigned char* imageData();

	int getWidth();

	int getHeight();

	long getFrameSize();

	unsigned char* getRedCom();

	unsigned char* getGreenCom();

	unsigned char* getBlueCom();

	static unsigned char* rgb2gray(unsigned char* image, int width, int height);

	static unsigned char*  reconstruct(unsigned char* redCom, unsigned char* greenCom, unsigned char* blueCom, int frameSize);

	static vector<double> genGaussianFilter(int size, double sigma);

	static vector<double> genAverFilter(int size);

	static unsigned char * filter2d(unsigned char* imageData, vector<double> kernel, int size, int width, int height);

	static double calPSNR(unsigned char* distort, unsigned char* origin, int imageSize);

	static unsigned char * medianFilter(unsigned char * imageData, int width, int height, int windowSize);

	static int writeFile(unsigned char * data, int width, int height, int channel, const char* path);

	static unsigned char* inverse(unsigned char* data, int width, int height, int channel);

	static vector<double> filter2d(unsigned char* imageData, vector <vector<double>> kernel, int width, int height);

	static vector<double> filter2d(unsigned char* imageData, vector <vector<double>> kernel, int width, int height,double dataMean);

	static double getmean(unsigned char* data, int width, int height);

	static double getABSmean(vector<double> data);

	static vector<double> filter2d(unsigned char * imageData, vector<double> kernel, int width, int height, bool edge);

	static unsigned char* castV2UC(vector<double> imageData);

	static vector<double> normalize2UCRange(vector<double> imageData);

	static vector<double> getABSResponse(vector<double> response);

};
#endif // !IMAGE_H_

