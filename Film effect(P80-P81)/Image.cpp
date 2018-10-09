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
