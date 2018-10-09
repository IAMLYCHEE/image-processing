#include<iostream>
#include<fstream>
#include"Image.h"
#include"lloydmax.h"
#include<vector>
#include<cmath>
#include<cstdlib>

using namespace std;

double findInterval(int data, vector<double> center) {
	if (center.size() < 1) { return -1; }//for safety
	int interval = 0;
	double distance = data - center.at(0);
	for (int i = 1; i < center.size(); i++) {
		if (abs(data - center.at(i)) < distance) {
			distance = data - center.at(i);
			interval = i;
		}
	}
	return center.at(interval);
}

vector<int> preprocess(unsigned char* data, int size) {
	vector<int> result;
	for (int i = 0; i < size; i++) {
		result.push_back((int)data[i]);
	}
	return result;
}

unsigned char reassign(unsigned char data, vector<double> centroids) {
	double distance = abs((double)data - centroids.at(0));
	int interval = 0;
	for (int i = 1; i < centroids.size(); i++) {
		if (abs(data - centroids.at(i)) < distance) {
			distance = abs(data - centroids.at(i));
			interval = i;
		}
	}
	return (unsigned char)(centroids.at(interval));
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

unsigned char mostOccur(vector<int> data,vector<double> centroids) {
	int maxCount = count(data.begin(), data.end(), (int)(centroids.at(0)));
	unsigned char result=(int)centroids.at(0);
	for (int i = 1; i < centroids.size(); i++) {
		if (count(data.begin(), data.end(), (int)(centroids.at(i))) > maxCount) {
			result = centroids.at(i);
		}
	}
	return (unsigned char)result;
}

int main(int argc,char * argv[]) {

	if (argc != 4) {
		cout << "check read me" << endl;
		return -1;
	}

	Image barn = Image(720, 480, 3, argv[1]);
	unsigned char* barnData = barn.imageData();
	//data of three channel
	unsigned char * barnRed = barn.getRedCom();
	unsigned char * barnGreen = barn.getGreenCom();
	unsigned char * barnBlue = barn.getBlueCom();
	//prepare data for the k-mean
	int size = barn.getWidth() * barn.getHeight();
	vector<int> barnReddata = preprocess(barnRed, size);
	vector<int> barnGreendata = preprocess(barnGreen, size);
	vector<int> barnBluedata = preprocess(barnBlue, size);
	//do k-mean for the three channel
	int colorbit = atoi(argv[2]);
	lloydMax redTrainer = lloydMax(colorbit);
	redTrainer.classify(barnReddata);
	vector<double> centroidsRed = redTrainer.getCentroids();

	lloydMax greenTrainer = lloydMax(colorbit);
	greenTrainer.classify(barnGreendata);
	vector<double> centroidsGreen = greenTrainer.getCentroids();

	lloydMax blueTrainer = lloydMax(colorbit);
	blueTrainer.classify(barnBluedata);
	vector<double> centroidsBlue = blueTrainer.getCentroids();

	//reassign data to the pixels
	for (int i = 0; i < size; i++) {
		barnRed[i] = reassign(barnRed[i], centroidsRed);
		barnGreen[i] = reassign(barnGreen[i], centroidsGreen);
		barnBlue[i] = reassign(barnBlue[i], centroidsBlue);
		if (i % 1000 == 0) cout << "#";
	}
	cout << "requantization finished" << endl;
	//reconstruct image
	unsigned char* requanBarn = (unsigned char*)malloc(sizeof(unsigned char)*(barn.getFrameSize()));
	for (int i = 0; i < barn.getFrameSize(); i = i + 3) {
		requanBarn[i] = barnRed[i / 3];
		requanBarn[i + 1] = barnGreen[i / 3];
		requanBarn[i + 2] = barnBlue[i / 3];
	}

	//reconstruct image with N
	cout << "grouping" << endl;
	int N = atoi(argv[2]);
	int startposition = barn.getWidth()*(N / 2) + (N / 2);
	unsigned char* barnRedG = (unsigned char*)malloc(sizeof(unsigned char)*size);
	memcpy(barnRedG, barnRed, size);
	unsigned char* barnGreenG = (unsigned char*)malloc(sizeof(unsigned char)*size);
	memcpy(barnGreenG, barnGreen, size);
	unsigned char* barnBlueG = (unsigned char*)malloc(sizeof(unsigned char)*size);
	memcpy(barnBlueG, barnBlue, size);
	for (int i = startposition; i < size - startposition; i++) {
		vector<int> pixelsRed;
		vector<int> pixelsGreen;
		vector<int> pixelsBlue;
		for (int j = -N / 2; j < N / 2 + 1; j++) {
			for (int k = -N / 2; k < N / 2 + 1; k++) {
				int position = i + j*barn.getWidth() + k;
				pixelsRed.push_back((int)barnRed[position]);
				pixelsGreen.push_back((int)barnGreen[position]);
				pixelsBlue.push_back((int)barnBlue[position]);
			}
		}
		barnRedG[i] = mostOccur(pixelsRed,centroidsRed);
		barnGreenG[i] = mostOccur(pixelsGreen,centroidsGreen);
		barnBlueG[i] = mostOccur(pixelsBlue,centroidsBlue);
		if (i % 1000 == 0) cout << "#";
	}
	cout << endl;
	cout << "regroup color finished" << endl;
	unsigned char* groupBarn = (unsigned char*)malloc(sizeof(unsigned char)*(barn.getFrameSize()));
	for (int i = 0; i < barn.getFrameSize(); i = i + 3) {
		groupBarn[i] = barnRedG[i / 3];
		groupBarn[i + 1] = barnGreenG[i / 3];
		groupBarn[i + 2] = barnBlueG[i / 3];
	}
	//writing file
	//first step
	writeFile(requanBarn, barn.getWidth(), barn.getHeight(), 3, "Requantize.raw");
	writeFile(groupBarn,barn.getWidth(),barn.getHeight(),3,"result.raw");
	cout << "writing file finished" << endl;
	getchar();

}