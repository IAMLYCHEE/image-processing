#include<iostream>
#include<fstream>
#include"image.h"
#include<map>
#include<cmath>
#include <vector>
#include <algorithm>
#include <cstdlib>
using namespace std;

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

int main(int argc, char * argv[]) {

	if (argc !=  2) {
		cout << "Problem3a.exe windowsize" << endl;
		cout << "exmaple : Problem3a.exe 3" << endl;
		return -1;
	}

	Image image = Image(512, 512, 3, "Lena_mixed.raw");
	int size = image.getWidth() * image.getHeight();

	unsigned char* imageData = image.imageData();
	unsigned char* imageRed = image.getRedCom();
	unsigned char* imageGreen = image.getGreenCom();
	unsigned char* imageBlue = image.getBlueCom();

	int windowSize = atoi(argv[1]);
	int sigma = 10;
	//average filter
	unsigned char * avgFilteredImageRed = Image::filter2d(imageRed, Image::genAverFilter(windowSize), windowSize, image.getWidth(), image.getHeight());
	unsigned char * avgFilteredImageGreen = Image::filter2d(imageGreen, Image::genAverFilter(windowSize), windowSize, image.getWidth(), image.getHeight());
	unsigned char * avgFilteredImageBlue = Image::filter2d(imageBlue, Image::genAverFilter(windowSize), windowSize, image.getWidth(), image.getHeight());
	unsigned char* avgFilteredImage = Image::reconstruct(avgFilteredImageRed, avgFilteredImageGreen, avgFilteredImageBlue,size*3);
	writeFile(avgFilteredImage, image.getWidth(), image.getHeight(), 3, "avgFilter.raw");

	//Gaussian filter
	unsigned char * gauFilteredImageRed = Image::filter2d(imageRed, Image::genGaussianFilter(windowSize,sigma), windowSize, image.getWidth(), image.getHeight());
	unsigned char * gauFilteredImageGreen = Image::filter2d(imageGreen, Image::genGaussianFilter(windowSize,sigma), windowSize, image.getWidth(), image.getHeight());
	unsigned char * gauFilteredImageBlue = Image::filter2d(imageBlue, Image::genGaussianFilter(windowSize,sigma), windowSize, image.getWidth(), image.getHeight());
	unsigned char* gauFilteredImage = Image::reconstruct(gauFilteredImageRed, gauFilteredImageGreen, gauFilteredImageBlue, size * 3);
	writeFile(gauFilteredImage, image.getWidth(), image.getHeight(), 3, "gauFilter.raw");
	//windowSize = 7;
	////cascade system
	//unsigned char* medFilterImageRed = Image::medianFilter(gauFilteredImageRed, image.getWidth(), image.getHeight(), windowSize);
	//unsigned char* medFilterImageGreen = Image::medianFilter(gauFilteredImageGreen, image.getWidth(), image.getHeight(), windowSize);
	//unsigned char* medFilterImageBlue = Image::medianFilter(gauFilteredImageBlue, image.getWidth(), image.getHeight(), windowSize);
	//unsigned char* medFilteredImage = Image::reconstruct(medFilterImageRed, medFilterImageGreen, medFilterImageBlue, size * 3);
	//writeFile(medFilteredImage, image.getWidth(), image.getHeight(), 3, "lena_filtered.raw");

	////median filter
	unsigned char* medFilterImageRed = Image::medianFilter(imageRed, image.getWidth(), image.getHeight(), windowSize);
	unsigned char* medFilterImageGreen = Image::medianFilter(imageGreen, image.getWidth(), image.getHeight(), windowSize);
	unsigned char* medFilterImageBlue = Image::medianFilter(imageBlue, image.getWidth(), image.getHeight(), windowSize);
	unsigned char* medFilteredImage = Image::reconstruct(medFilterImageRed, medFilterImageGreen, medFilterImageBlue, size * 3);
	writeFile(medFilteredImage, image.getWidth(), image.getHeight(), 3, "medianFilter.raw");

	////get the PSNR
	////average filter
	////get the data , avoid doing filter again
	Image imageOrigin = Image(512, 512, 3, "Lena.raw");
	Image imageAvg = Image(512, 512, 3, "avgFilter.raw");
	Image imageGau = Image(512, 512, 3, "gauFilter.raw");
	Image imageMed = Image(512, 512, 3, "medianFilter.raw");
	//Image imageNLM = Image(512, 512, 3, "NLM.raw");
	//Image imageNLM20 = Image(512, 512, 3, "NLM20.raw");
	//Image imageNLM25 = Image(512, 512, 3, "NLM25.raw");


	cout <<"Median filter PSNR:" <<Image::calPSNR(imageMed.imageData(), imageOrigin.imageData(), size*3)<< endl;
	cout << "Average filter PSNR:" << Image::calPSNR(imageAvg.imageData(), imageOrigin.imageData(), size*3) << endl;
	cout << "Gaussian filter PSNR:" << Image::calPSNR(imageGau.imageData(), imageOrigin.imageData(), size*3) << endl;
	//cout << " Non local mean PSNR:" << Image::calPSNR(imageNLM.imageData(), imageOrigin.imageData(), size * 3) << endl;
	//cout << " Non local mean PSNR:" << Image::calPSNR(imageNLM20.imageData(), imageOrigin.imageData(), size * 3) << endl;
	//cout << " Non local mean PSNR:" << Image::calPSNR(imageNLM25.imageData(), imageOrigin.imageData(), size * 3) << endl;

}
