#include"Jigsaw.h"

using namespace std;

Jigsaw::Jigsaw(int width, int height,unsigned char* data,int label) {
	this->width = width;
	this->height = height;
	this->data = data;
	this->label = label;
}

void Jigsaw::setLabel(int label) {
	this->label = label;
}

unsigned char* Jigsaw::getdata() {
	return data;
}

int Jigsaw::getWidth()
{
	return width;
}

int Jigsaw::getHeight()
{
	return height;
}

int Jigsaw::getLabel() {
	return label;
}


