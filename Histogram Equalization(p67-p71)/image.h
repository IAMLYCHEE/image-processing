#pragma once
//author Li Yicheng 1/17/2018
#ifndef IMAGE_H_
#define IMAGE_H_

#include<iostream>
#include<fstream>	
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
};

#endif // !IMAGE_H_
