#include<iostream>
#include<fstream>
#include"image.h"
#include<map>
#include<math.h>

using namespace std;

std::map<unsigned char, int> genPdf(unsigned char* data, int size) {
	map<unsigned char, int> pdf;
	for (int i = 0; i < size; i++) {
		unsigned char temp = data[i];
		std::map<unsigned char, int>::iterator it;
		it = pdf.find(temp);
		if (it != pdf.end()) {
			pdf[temp] ++;
		}
		else {
			pdf.insert(std::pair<unsigned char, int>(temp, 1));
		}
	}
	return pdf;
}

void description(map<unsigned char, int> table) {
	for (map<unsigned char, int>::iterator it = table.begin(); it != table.end(); it++) {
		cout << (int)(it->first) << ":" << it->second << endl;
	}
}

void description(unsigned char* data, int size) {
	for (int i = 0; i < size; i++) {
		cout << (int)(data[i]) << " ";
	}	
}

unsigned char* hisEqua(unsigned char* data,int size) {
	//generate the pdf	
	std::map<unsigned char, int> pdf = genPdf(data, size);
	int cdf[256] = {};
	cdf[0] = pdf[(unsigned int)0];
	for (unsigned int i = 1; i < 256; i++) {
		cdf[i] = pdf[(unsigned int)i] + cdf[i - 1];
	}

	 //transform the intensity
	unsigned int transtable[256];
	for (unsigned int i = 0; i < 255; i++) {
		transtable[i] = (unsigned int)(round((double)(cdf[i] - cdf[0]) / (400.0 * 300.0 - cdf[0]) * 255.0));
	}

	for (int i = 0; i < size; i++) {
		data[i] = transtable[data[i]];
	}
	cout << "histogram equalization finished"<< endl;
	return data;
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

unsigned char * reconstruct(unsigned char* redCom, unsigned char* greenCom, unsigned char* blueCom, int frameSize) {
	unsigned char* image = (unsigned char*)malloc(sizeof(unsigned char)*frameSize);
	for (int i = 0; i < frameSize; i = i + 3) {
		image[i] = redCom[i / 3];
		image[i + 1] = greenCom[i / 3];
		image[i + 2] = blueCom[i / 3];
	}
	return image;
}

unsigned char transfer1(unsigned char data) {
	double result;
	if (data <= 50) {
		result = (double)data * 220.0 / 50.0;
	}
	else {
		result = 220.0 + ((double)data - 50.0) *35.5 / 205.0;
	}
	return (unsigned char)result;
}

unsigned char transfer2(unsigned char data) {
	double result = 255.0 / (log10(255.0))*log10((double)data);
	return (unsigned char)result;
}



int main(int argc,char* argv[]) {

	//retrieve the data..
	Image desk = Image(400, 300, 3, "Desk.raw");
	int size = 400 * 300 ;
	unsigned char* deskData = desk.imageData();
	unsigned char* deskRed = desk.getRedCom();
	unsigned char* deskGreen = desk.getGreenCom();
	unsigned char* deskBlue = desk.getBlueCom();

	//hist equalization
	unsigned char* equRed = hisEqua(deskRed, size);
	unsigned char* equGreen = hisEqua(deskGreen, size);
	unsigned char* equBlue = hisEqua(deskBlue, size);

	//construct a new RGB picture
	//int size = desk.getWidth()*desk.getHeight() * 3;
	unsigned char* equDesk = reconstruct(equRed, equGreen, equBlue, size*3);
	//writing file
	writeFile(equDesk, 400, 300, 3, "equDesk.raw");

	//transfer function1
	//transfer funciton2
	unsigned char* transfer1Image = (unsigned char*)malloc(sizeof(unsigned char)*size*3);
	unsigned char* transfer2Image = (unsigned char*)malloc(sizeof(unsigned char)*size*3);
	for (int i = 0; i < size*3; i++) {
		transfer1Image[i] = transfer1(deskData[i]);
		transfer2Image[i] = transfer2(deskData[i]);
	}

	//writing to file
	writeFile(transfer1Image, desk.getWidth(), desk.getHeight(), 3, "transfer1.raw");
	writeFile(transfer2Image, desk.getWidth(), desk.getHeight(), 3, "transfer2.raw");


	return -1;


}

