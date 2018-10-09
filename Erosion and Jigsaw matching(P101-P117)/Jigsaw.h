#pragma once
//author Li Yicheng 1/17/2018
//no distribution without permission
#include"Jigsaw.h"	
#ifndef  JIGSAW_H_
#define JIGSAW_H_

#include<iostream>

class Jigsaw {
private:
	int width;
	int height;
	int label;
	unsigned char* data;
public:
	Jigsaw::Jigsaw(int width, int height, unsigned char*data, int label);
	void setLabel(int label);
	unsigned char * getdata();
	int getWidth();
	int getHeight();
	int getLabel();
};



#endif // ! JIGSAW_H_