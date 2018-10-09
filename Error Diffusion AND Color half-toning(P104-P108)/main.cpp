#include<iostream>
#include"Image.h"
#include<vector>
#include"Dithering.h"
#define CMYW 0
#define MYGC 1
#define RGMY 2
#define KRGB 3
#define RGBM 4
#define CMGB 5

vector<vector<vector<int>>> colorVs = { { { 0,1,0 },{1,0,0},{0,0,1},{0,0,0} },
										{ { 1,0,0 },{0,0,1},{0,1,1},{0,1,0} },
										{ { 1,0,1 },{0,1,1},{1,0,0},{0,0,1}},
										{ { 1,1,1 },{1,0,1},{0,1,1},{1,1,0} },
										{ { 1,0,1 },{0,1,1},{1,1,0},{1,0,0} },
										{ { 0,1,0 },{1,0,0},{0,1,1},{1,1,0} },
									  };

vector<vector<double>> floydm = { { 0,0,7.0 / 16.0 },{ 3.0 / 16.0,5.0 / 16.0,1.0 / 16.0 } };

int cVIndex( unsigned char red,unsigned char green,unsigned char blue ){
	int r = (int)red;
	int g = (int)green;
	int b = (int)blue;
	if ((r + g) > 255) {
		if ((g + b) > 255) {
			if ((r + g + b) > 510) {
				return CMYW;
			}
			else {
				return MYGC;
			}
		}
		else
		{
			return RGMY;
		}
	}
	else {
		if ((g + b) <= 255) {
			if ((r + g + b) <= 255) {
				return KRGB;
			}
			else {
				return RGBM;
			}
		}
		else {
			return CMGB;
		}
	}
}

double constraintv(double value) {
	if (value < 0) {
		return
			0.0;
	}
	else if (value > 255) {
		return 255.0;
	}
	return value;
}

vector<int> findVertex(vector<int> pixel) {
	int index = cVIndex(pixel[0], pixel[1], pixel[2]);
	vector<vector<int>> vertexs = colorVs.at(index);
	double minError = 0.0;
	int vIndex = 0;
	for (int i = 0; i < 3; i++) {
		vector<int> vertex = vertexs[0];
		minError += pow((double)((255-pixel[i]) - vertex[i]*255), 2.0);
	}
	for (int i = 1; i < 4; i++) {
		double error = 0.0;
		vector<int> vertex = vertexs.at(i);
		for (int j = 0; j < 3; j++) {
			error += pow((double)((255-pixel[j]) - vertex[j] * 255), 2.0);
		}
		if (error < minError) {
			minError = error;
			vIndex = i;
		}
	}
	return vertexs.at(vIndex);
}

unsigned char * colorMBVQ(unsigned char* cdata, unsigned char* mdata, unsigned char* ydata, int width, int height,int wSize) {
	unsigned char* newCCom = (unsigned char*)malloc(sizeof(unsigned char)*width*height);
	unsigned char* newMCom = (unsigned char*)malloc(sizeof(unsigned char)*width*height);
	unsigned char* newYCom = (unsigned char*)malloc(sizeof(unsigned char)*width*height);

	vector<double> cCom;
	vector<double> mCom;
	vector<double> yCom;
	for (int i = 0; i < width * height; i++) {
		cCom.push_back((double)cdata[i]);
	}
	for (int i = 0; i < width * height; i++) {
		mCom.push_back((double)mdata[i]);
	}
	for (int i = 0; i < width * height; i++) {
		yCom.push_back((double)ydata[i]);
	}


	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			vector<int> pixel;
			pixel.push_back(255-(int)cCom[i*width + j]);
			pixel.push_back(255-(int)mCom[i*width + j]);
			pixel.push_back(255-(int)yCom[i*width + j]);
			vector<int> nearestVertex = findVertex(pixel);
			newCCom[i*width + j] = nearestVertex[0]*255;
			newMCom[i*width + j] = nearestVertex[1]*255;
			newYCom[i*width + j] = nearestVertex[2]*255;
			int p = i * width + j;
			int errC = (255 - pixel[0])- newCCom[i*width+j];
			int errM = (255 - pixel[1]) - newMCom[i*width + j];
			int errY = (255 - pixel[2]) - newYCom[i*width + j];
			for (int k = wSize / 2 + 1; k < wSize; k++) {
				if ((j + k) < width) {
					cCom[p + k - wSize / 2] += (floydm[0][k] * (double)errC);
					cCom[p + k - wSize / 2] = constraintv(cCom[p + k - wSize / 2]);
					mCom[p + k - wSize / 2] += (floydm[0][k] * (double)errM);
					mCom[p + k - wSize / 2] = constraintv(mCom[p + k - wSize / 2]);
					yCom[p + k - wSize / 2] += (floydm[0][k] * (double)errY);
					yCom[p + k - wSize / 2] = constraintv(yCom[p + k - wSize / 2]);
				}
			}
			for (int jj = 1; jj < wSize / 2 + 1; jj++) {
				for (int k = -wSize / 2; k < wSize / 2 + 1; k++) {
					if ((i + jj) < height && (j + k) < width &&(j+k)>0) {
						cCom[p + jj*width + k] += (floydm[jj][k + wSize / 2] * (double)errC);
						cCom[p + jj*width + k] = constraintv(cCom[p + jj*width + k]);
						mCom[p + jj*width + k] += (floydm[jj][k + wSize / 2] * (double)errM);
						mCom[p + jj*width + k] = constraintv(mCom[p + jj*width + k]);
						yCom[p + jj*width + k] += (floydm[jj][k + wSize / 2] * (double)errY);
						yCom[p + jj*width + k] = constraintv(yCom[p + jj*width + k]);
					}
				}
			}
		}
		if (i % 10 == 0) cout << "#" ;
	}
	unsigned char * result = Image::reconstruct(newCCom, newMCom, newYCom, height*width * 3);
	return result;
}
int main() {
	//======================2b=======================//

	//gather image informate
	//Image image = Image(512, 512, 1, "colorchecker.raw");
	//unsigned char * data = image.imageData();
	//int width = image.getWidth();
	//int height = image.getHeight();

	//0:floyd
	//1:javrvis
	//2:Stucki
	//unsigned char* result = Dithering::errorDif(data, width, height, 0, 3);
	//Image::writeFile(result, width, height, 1, "diffusionFloyd.raw");
	//unsigned char* result2 = Dithering::errorDif(data, width, height, 1, 5);
	//Image::writeFile(result2, width, height, 1, "diffusionjavrvis.raw");
	//unsigned char* result3 = Dithering::errorDif(data, width, height, 2, 5);
	//Image::writeFile(result3, width, height, 1, "diffusionstucki.raw");

	//=======================2c========================//
	//gather information
	Image imageflower = Image(700, 700, 3,"flower.raw");
	unsigned char *flowerdata = imageflower.imageData();
	int fwidth = imageflower.getWidth();
	int fheight = imageflower.getHeight();
	unsigned char * redCom = imageflower.getRedCom();
	unsigned char * greenCom = imageflower.getGreenCom();
	unsigned char * blueCom = imageflower.getBlueCom();

	//gather CMY data
	unsigned char * cCom = Image::inverse(redCom, fwidth, fheight, 1);
	unsigned char * mCom = Image::inverse(greenCom, fwidth, fheight, 1);
	unsigned char * yCom = Image::inverse(blueCom, fwidth, fheight, 1);

	////error diffusion
	//unsigned char * cComED = Dithering::errorDif(cCom, fwidth, fheight, 0, 3);
	//unsigned char * mComED = Dithering::errorDif(mCom, fwidth, fheight, 0, 3);
	//unsigned char * yComED = Dithering::errorDif(yCom, fwidth, fheight, 0, 3);

	////Combine channel
	//unsigned char* colorErrDis = Image::reconstruct(cComED, mComED, yComED, fwidth*fheight * 3);
	////inverse color
	//colorErrDis = Image::inverse(colorErrDis,fwidth,fheight,3);
	////write file
	//Image::writeFile(colorErrDis, fwidth, fheight, 3, "colorErrDif.raw");


	//==============MBVQ=========================//
	unsigned char* mbvqMethod = colorMBVQ(cCom, mCom, yCom, fwidth, fheight, 3);
	unsigned char* result = Image::inverse(mbvqMethod, fwidth, fheight, 3);
	Image::writeFile(result, fwidth, fheight, 3, "mbvqResult.raw");


	return 0;

}