#pragma once
//author Li Yicheng 2/11/2018
//no distribution without permission

#ifndef DITHERING_H_
#define DITHERING_H_

#include<iostream>
#include<random>
#include<cmath>
using namespace std;

class Dithering {

public:


	static void showKernel();

	static unsigned char * fixThre(unsigned char * data, int width, int height, int channel, int Thre);

	static unsigned char * fixThreT4(unsigned char * data, int width, int height, int channel);

	static unsigned char * randThre(unsigned char* data, int width, int height, int channel);

	static unsigned char * matrixN2(unsigned char * data, int width, int height, int channel);

	static unsigned char * matrixN4(unsigned char * data, int width, int height, int channel);

	static unsigned char * matrixN4keep(unsigned char * data, int width, int height, int channel);

	static unsigned char * matrixN8(unsigned char * data, int width, int height, int channel);

	static unsigned char * matrixN4T4(unsigned char * data, int width, int height, int channel);
};


#endif