#include<iostream>	
#include<fstream>	
#include"Image.h"

//author Li Yicheng 1/17/2018

using namespace std;

int writeFile(unsigned char * data, int width, int height, int channal,const char* path) {
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


unsigned char max(unsigned char a, unsigned char b, unsigned char c) {
	unsigned char maxc;
	a > b ? maxc = a : maxc = b;
	if (c > maxc) maxc = c;
	return maxc;
}

unsigned char min(unsigned char a, unsigned char b, unsigned char c) {
	unsigned char minc;
	a < b ? minc = a : minc = b;
	if (c < minc) minc = c;
	return minc;
}
int main() {
	//get image data
	Image tiffany = Image(512, 512, 3, "Tiffany.raw");
	unsigned char * imagetiff = tiffany.imageData();
	
	//========================rgb to gray====================================================//
	//average method
	unsigned char *gray1 = (unsigned char*)malloc((sizeof(unsigned char) * 512 * 512));
	for (int i = 0; i < 512 * 512*3; i=i+3) {
		gray1[i/3] = (unsigned char)((int)(imagetiff[i] + imagetiff[i + 1] + imagetiff[i + 2]) / 3);
	}
	//(max + min)/2
	unsigned char *gray2 = (unsigned char*)malloc((sizeof(unsigned char) * 512 * 512));
	for (int i = 0; i < 512 * 512 * 3; i = i + 3) {
		gray2[i / 3] = (unsigned char)((int)((max(imagetiff[i] , imagetiff[i + 1] , imagetiff[i + 2]) + min(imagetiff[i], imagetiff[i + 1], imagetiff[i + 2]) )/ 2));
	}
	//weighted sum
	unsigned char *gray3 = (unsigned char*)malloc((sizeof(unsigned char) * 512 * 512));
	for (int i = 0; i < 512 * 512 * 3; i = i + 3) {
		gray3[i / 3] = (unsigned char)((int)(0.21*imagetiff[i] + 0.72*imagetiff[i + 1] + 0.07*imagetiff[i + 2]));
	}
	cout << "three toGray method complete" << endl;

	//write files
	writeFile(gray1, 512, 512, 1, "gray1.raw");
	writeFile(gray2, 512, 512, 1, "gray2.raw");
	writeFile(gray3, 512, 512, 1, "gray3.raw");
	//===========================================================================================//


	//=========================rgb to cmk========================================================//
	//get image data
	Image dance = Image(854, 480, 3, "Dance.raw");
	unsigned char * danceData = dance.imageData();
	unsigned char* cyanComd = (unsigned char*)malloc((sizeof(unsigned char) * 854 * 480));
	unsigned char* mageComd = (unsigned char*)malloc((sizeof(unsigned char) * 854 * 480));
	unsigned char* yellComd = (unsigned char*)malloc((sizeof(unsigned char) * 854 * 480));
	for (int i = 0; i <dance.getFrameSize(); i=i+3)
	{
		// double mincNorm = ((double)min(bearData[i], bearData[i + 1], bearData[i + 2]))/255;
		// cyanCom[i / 3] = (unsigned char)((double)(255 - bearData[i]-mincNorm*255) / (1 - mincNorm));
		// mageCom[i / 3] = (unsigned char)((double)(255 - bearData[i+1]-mincNorm*255) / (1 - mincNorm));
		// yellCom[i / 3] = (unsigned char)((double)(255 - bearData[i+2]-mincNorm*255) / (1 - mincNorm));
		cyanComd[i / 3] = (unsigned char)((double)(255 - danceData[i]));
		mageComd[i / 3] = (unsigned char)((double)(255 - danceData[i+1]));
		yellComd[i / 3] = (unsigned char)((double)(255 - danceData[i+2]));
	}
	cout << "RGB to CMYK complete" << endl;

	//write files
	writeFile(cyanComd, 854, 480, 1, "cyanDance.raw");
	writeFile(mageComd, 854, 480, 1, "mageDance.raw");
	writeFile(yellComd, 854, 480, 1, "yellDance.raw");

	Image bear = Image(854, 480, 3, "Bear.raw");
	unsigned char * bearData = bear.imageData();
	unsigned char* cyanCom = (unsigned char*)malloc((sizeof(unsigned char) * 854 * 480));
	unsigned char* mageCom = (unsigned char*)malloc((sizeof(unsigned char) * 854 * 480));
	unsigned char* yellCom = (unsigned char*)malloc((sizeof(unsigned char) * 854 * 480));
	for (int i = 0; i <bear.getFrameSize(); i = i + 3)
	{
		// double mincNorm = ((double)min(bearData[i], bearData[i + 1], bearData[i + 2]))/255;
		// cyanCom[i / 3] = (unsigned char)((double)(255 - bearData[i]-mincNorm*255) / (1 - mincNorm));
		// mageCom[i / 3] = (unsigned char)((double)(255 - bearData[i+1]-mincNorm*255) / (1 - mincNorm));
		// yellCom[i / 3] = (unsigned char)((double)(255 - bearData[i+2]-mincNorm*255) / (1 - mincNorm));
		cyanCom[i / 3] = (unsigned char)((double)(255 - bearData[i]));
		mageCom[i / 3] = (unsigned char)((double)(255 - bearData[i + 1]));
		yellCom[i / 3] = (unsigned char)((double)(255 - bearData[i + 2]));
	}
	cout << "RGB to CMYK complete" << endl;

	//write files
	writeFile(cyanCom, 854, 480, 1, "cyanBear.raw");
	writeFile(mageCom, 854, 480, 1, "mageBear.raw");
	writeFile(yellCom, 854, 480, 1, "yellBear.raw");

}
