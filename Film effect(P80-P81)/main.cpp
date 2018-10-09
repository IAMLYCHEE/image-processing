#include<iostream>
#include<fstream>
#include"Image.h"
#include<vector>
#include<cmath>
#include<stack>

using namespace std;

unsigned char * reconstruct(unsigned char* redCom, unsigned char* greenCom, unsigned char* blueCom,int frameSize) {
	unsigned char* image = (unsigned char*)malloc(sizeof(unsigned char)*frameSize);
	for (int i = 0; i < frameSize; i = i + 3) {
		image[i] = redCom[i / 3];
		image[i + 1] = greenCom[i / 3];
		image[i + 2] = blueCom[i / 3];
	}
	return image;
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

unsigned char hisTrans(unsigned char oValue,double start,double end) {
	double result = start + (end - start) / 255.0*(255.0 - oValue);
	return (unsigned char)result;
}

int main() {
	Image image = Image(256, 256, 3, "Girl.raw");
	unsigned char * data = image.imageData();
	unsigned char * redCom = image.getRedCom();
	unsigned char * greenCom = image.getGreenCom();
	unsigned char * blueCom = image.getBlueCom();

	int size = image.getHeight()*image.getWidth();
	unsigned char* redComMir = (unsigned char*)malloc(sizeof(unsigned char)*size);
	unsigned char* greenComMir = (unsigned char*)malloc(sizeof(unsigned char)*size);
	unsigned char* blueComMir = (unsigned char*)malloc(sizeof(unsigned char)*size);

	for (int i = 0; i < image.getHeight(); i++) {
		stack<unsigned char> memory;
		for (int j = 0; j < image.getWidth(); j++) {
			memory.push(redCom[j + i*image.getWidth()]);
		}
		for (int j = 0; j < image.getWidth(); j++) {
			redComMir[j+i*image.getWidth()]=hisTrans(memory.top(),70.0,255.0);
			memory.pop();
		}

		for (int j = 0; j < image.getWidth(); j++) {
			memory.push(greenCom[j + i*image.getWidth()]);
		}
		for (int j = 0; j < image.getWidth(); j++) {
			greenComMir[j + i*image.getWidth()] = hisTrans(memory.top(),20.0,205.0);
			memory.pop();
		}

		for (int j = 0; j < image.getWidth(); j++) {
			memory.push(blueCom[j + i*image.getWidth()]);
		}
		for (int j = 0; j < image.getWidth(); j++) {
			blueComMir[j + i*image.getWidth()] = hisTrans(memory.top(),0.0,190.0);
			memory.pop();
		}
	}

	unsigned char * photoEffect = reconstruct(redComMir, greenComMir, blueComMir, image.getFrameSize());
	writeFile(photoEffect, image.getWidth(), image.getHeight(), 3, "GirlPhoto.raw");
	getchar();
}