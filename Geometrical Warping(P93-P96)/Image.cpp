#include "Image.h"
#include<iostream>	
#include<fstream>

using namespace std;

Image::Image(int w, int h, int channel, const char* path) {
	this->width = w;
	this->height = h;
	this->channel = channel;
	this->filename = path;
	this->frameSize = width * height * channel;
}

unsigned char* Image::imageData() {
	FILE *fp = NULL;
	fp = fopen(filename, "rb");
	this->imagedata = (unsigned char*)malloc(sizeof(unsigned char)*frameSize);
	if (!fp) {
		cout << "Fail to open file" << endl;
		return NULL;
	}
	fread(imagedata, sizeof(unsigned char), frameSize, fp);
	fclose(fp);
	return imagedata;
}

int Image::getWidth() {
	return width;
}

int Image::getHeight() {
	return height;
}

long Image::getFrameSize() {
	return frameSize;
}

unsigned char* Image::getRedCom() {
	int size = height * width * channel;
	unsigned char * redCom = (unsigned char*)malloc(sizeof(unsigned char)* size);
	if (imagedata != NULL) {
		for (int i = 0; i<size; i = i + 3) {
			redCom[i / 3] = imagedata[i];
		}
		return redCom;
	}
	else {
		cout << "initiate image data first" << endl;
		return NULL;
	}
}

unsigned char* Image::getGreenCom() {
	int size = height * width * channel;
	unsigned char * redCom = (unsigned char*)malloc(sizeof(unsigned char)* size);
	if (imagedata != NULL) {
		for (int i = 1; i<size; i = i + 3) {
			redCom[i / 3] = imagedata[i];
		}
		return redCom;
	}
	else {
		cout << "initiate image data first" << endl;
		return NULL;
	}
}

unsigned char* Image::getBlueCom() {
	int size = height * width * channel;
	unsigned char * redCom = (unsigned char*)malloc(sizeof(unsigned char)* size);
	if (imagedata != NULL) {
		for (int i = 2; i<size; i = i + 3) {
			redCom[i / 3] = imagedata[i];
		}
		return redCom;
	}
	else {
		cout << "initiate image data first" << endl;
		return NULL;
	}
}

unsigned char * Image::reconstruct(unsigned char * redCom, unsigned char * greenCom, unsigned char * blueCom, int frameSize)
{
	unsigned char* image = (unsigned char*)malloc(sizeof(unsigned char)*frameSize);
	for (int i = 0; i < frameSize; i = i + 3) {
		image[i] = redCom[i / 3];
		image[i + 1] = greenCom[i / 3];
		image[i + 2] = blueCom[i / 3];
	}
	return image;
}

vector<double> Image::genGaussianFilter(int size, double sigma)
{
	double pi = 3.14159265;
	int center = size / 2;
	vector<double> filter;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			double denominator = 2 * pi * sigma * sigma;
			double nominator = exp(-((double)(pow(i - center, 2) + pow(j - center, 2))) / ((double)(2 * pow(sigma, 2))));
			double value = nominator / denominator;
			filter.push_back(value);
		}
	}

	//normalize
	double sum = 0;
	for (auto i : filter) {
		sum = sum + i;
	}
	for (int i = 0; i < filter.size(); i++) {
		filter[i] = filter[i] / sum;
	}
	return filter;
}

vector<double> Image::genAverFilter(int size)
{
	vector<double> filterData;
	int length = size*size;
	for (int i = 0; i < size*size; i++) {
		filterData.push_back((double)1.0 / ((double)length));
	}
	return filterData;
}

double innerProduct(vector<double> vector1, vector<double> vector2, int length) {
	double sum = 0;
	for (int i = 0; i < length; i++) {
		sum = sum + vector1[i] * vector2[i];
	}
	return sum;
}


unsigned char * Image::filter2d(unsigned char * imageData, vector<double> kernel, int size, int width, int height)
{
	int imageSize = width*height;// the size of the image
	int length = size * size;//the length of the filter vector is size^2
							 //assign the result
	unsigned char * filteredImage = (unsigned char*)malloc(imageSize * sizeof(unsigned char));
	//copy the origin image to filtered image, for border consideration
	memcpy(filteredImage, imageData, imageSize);
	// get the first to do the filter
	int startPosition = width*(size / 2) + size / 2;
	for (int i = startPosition; i < imageSize - startPosition; i++) {
		vector<double> blockData;
		for (int j = -size / 2; j < size / 2 + 1; j++) {
			for (int k = -size / 2; k < size / 2 + 1; k++) {
				int position = i + j * width + k;
				blockData.push_back((double)imageData[position]);
			}
		}
		double productResult = innerProduct(blockData, kernel, length);
		filteredImage[i] = (unsigned char)(productResult);
		blockData.clear();
		if (i % 1000 == 0) { cout << "#"; }
	}
	cout << endl;
	cout << "filter finished" << endl;
	return filteredImage;
}

double Image::calPSNR(unsigned char * distort, unsigned char * origin, int imageSize)
{
	double squareError = 0.0;
	for (int i = 0; i < imageSize; i++) {
		squareError += pow((double)distort[i] - (double)origin[i], 2);
	}
	double MSE = squareError / imageSize;
	return (10.0*log10(255.0*255.0 / MSE));
}


bool mySortLogic(double i, double j) { return (i < j); }

unsigned char * Image::medianFilter(unsigned char * imageData, int width, int height, int windowSize) {
	int imageSize = width*height;// the size of the image
	int length = windowSize * windowSize;//the length of the filter vector is size^2
										 //assign the result
	unsigned char * filteredImage = (unsigned char*)malloc(imageSize * sizeof(unsigned char));
	//copy the origin image to filtered image, for border consideration
	memcpy(filteredImage, imageData, imageSize);
	// get the first to do the filter
	int startPosition = width*(windowSize / 2) + windowSize / 2;
	for (int i = startPosition; i < imageSize - startPosition; i++) {
		vector<double> blockData;
		for (int j = -windowSize / 2; j < windowSize / 2 + 1; j++) {
			for (int k = -windowSize / 2; k < windowSize / 2 + 1; k++) {
				int position = i + j * width + k;
				blockData.push_back((double)imageData[position]);
			}
		}
		std::sort(blockData.begin(), blockData.end(), mySortLogic);
		//test
		//if (i % 10000 == 0) {
		//	for (auto e : blockData) {
		//		cout << e << " ";
		//	}
		//	cout << endl;
		//}
		filteredImage[i] = (unsigned char)(blockData.at(blockData.size() / 2));
		blockData.clear();
		if (i % 1000 == 0) { cout << "#"; }
	}
	cout << endl;
	cout << "median filter finished" << endl;
	return filteredImage;
}


int Image::writeFile(unsigned char * data, int width, int height, int channal, const char* path) {
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
