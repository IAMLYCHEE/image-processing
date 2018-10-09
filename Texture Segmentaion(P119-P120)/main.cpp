#include<iostream>	
#include"Image.h"
#include"VQ.h"
#include"LawFilters.h"
#include <fstream>
#include<iomanip>
using namespace std;

vector2d normalize(vector2d data,int N,int D) {
	vector2d normedData(N, vector<double>(D - 1,0));
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < D - 1; j++) {
			normedData[i][j] = data[i][j + 1] / data[i][0];
		}
	}
	return normedData;
}

vector<double> genABSMean(vector2d data, int D) {
	vector<double> mu;
	for (int i = 0; i < D; i++) {
		double sum = 0.0;
		for (int j = 0; j < data.size(); j++) {
			sum += abs(data[j][i]);
		}
		mu.push_back(sum / data.size());
	}
	return mu;
}


vector2d genResponses3x3(unsigned char* imageData, int ws, int width, int height) {
	cout << "generate data" << endl;
	LawFilters lawfilter = LawFilters();
	vector2d filterResponses;
	for (int i = 0; i < 9; i++) {
		vector<double> filterResponse = Image::filter2d(imageData, lawfilter.getFilter3X3(i), width, height);
		cout << i << "th filter finished" << endl;
		filterResponses.push_back(filterResponse);
	}

	//transpose data
	int dataSize = filterResponses.at(0).size();
	vector2d responseDataInRow(dataSize,vector<double>(9,0));
	for (int i = 0; i < dataSize; i++) {
		for (int j = 0; j < 9; j++) {
			responseDataInRow[i][j] = filterResponses[j][i];
		}
	}
	cout << "transpose finished" << endl;

	int filterDataWidth = width - 2;
	int filterDataHeight = height - 2;
	vector2d finalData;
	//gather neighbourhood data
	for (int i = 0; i < filterDataHeight; i++) {
		for (int j = 0; j < filterDataWidth; j++) {
			vector2d blockDataset;
			for (int m = -ws / 2; m < ws / 2 + 1; m++) {
				for (int n = -ws / 2; n < ws / 2 + 1; n++) {
					int posV= i+m;
					int posH=j+n;
					if (posV < 0) posV = 0;
					if (posV >= filterDataHeight) posV = filterDataHeight-1;
					if (posH < 0) posH = 0;
					if (posH >= filterDataWidth) posH = filterDataWidth-1;
					blockDataset.push_back(responseDataInRow.at(posV * filterDataWidth + posH));
				}
			}
			vector<double> avgEnergyResponse = genABSMean(blockDataset, 9);
			finalData.push_back(avgEnergyResponse);
			cout << ((double)(i*filterDataWidth+j)/(dataSize) )* 100 << "%"  << " " << i << " " << j << '\r';
		}
	}

	return finalData;
}

int saveData(vector2d data,string filename) {
	ofstream outfile;
	outfile.open(filename, std::ios::out);
	if (!outfile.is_open()) {
		cout << "can't open file" << endl;
		return -1;
	}
	else {
		for (auto e : data) {
			for (auto ee : e) {
				outfile << std::fixed << std::setprecision(sizeof(ee)) << ee << " ";
			}
			outfile << endl;
		}
		outfile.close();
	}
	return 0;
}

int saveLabelData(vector<int> data, string filename) {
	ofstream outfile;
	outfile.open(filename, std::ios::out);
	if (!outfile.is_open()) {
		cout << "can't open file" << endl;
		return -1;
	}
	else {
		for (auto e : data) {
			outfile << std::fixed << std::setprecision(sizeof(e)) << e << " ";
		}
		outfile.close();
	}
	return 0;
}

vector2d readData(string filename,int D) {
	vector2d datas;
	vector<double> data;
	ifstream infile(filename, std::ios::in);
	if (!infile.is_open()) {
		cout << "file can not open" << endl;
	}
	double num = 0.0;

	int count = 0;
	while (infile >> std::setprecision(sizeof(num)) >> std::fixed >> num) {
			data.push_back(num);
			count++;
			if (count % 9 == 0) {
				datas.push_back(data);
				data.clear();
			}
	}
	infile.close();
	return datas;
}

vector<int> readLabelData(string filename) {
	vector<int> labels;
	ifstream infile(filename, std::ios::in);
	if (!infile.is_open()) {
		cout << "file can not open" << endl;
	}
	int num = 0;

	while (infile >> std::setprecision(sizeof(num)) >> std::fixed >> num) {
		labels.push_back(num);
	}
	infile.close();
	return labels;
}

int main() {
	//step1 get data
	Image image = Image(600, 450, 1, "comb.raw");
	unsigned char* imageData = image.imageData();
	int windowSize = 21;
	vector2d data = genResponses3x3(imageData, windowSize, image.getWidth(), image.getHeight());
	cout << "finish gathering data" << endl;
	cout << "saving to file ....." << endl;
	saveData(data, "responsedDataw21.dat");

	data = readData("responsedDataw21.dat",9);
	cout << "data loaded!" << endl;
	VQ classifier1 = VQ(data, 6);
	//normalize data
	vector2d normedData = normalize(data,classifier1.getN(),classifier1.getD());

	//K-mean
	VQ classifier2 = VQ(normedData, 6);
	classifier2.dataDescription();
	vector<int> label = classifier2.classify(2);
	cout << "training finished" << endl;
	saveLabelData(label, "labels.dat");

	//label image
	vector<int> labels = readLabelData("labels.dat");
	int size = (image.getWidth() - 2)*(image.getHeight() - 2);
	unsigned char* labelImage = (unsigned char*)malloc(sizeof(unsigned char)* size);
	for (int i = 0; i < size; i++) {
		labelImage[i] = (unsigned char)labels[i] * 51;
	}
	Image::writeFile(labelImage, image.getWidth() - 2, image.getHeight() - 2, 1, "labelImage21.raw");
	cout << "label image write finished" << endl;
	getchar();
	return 0;
}