#include<iostream>	
#include"Image.h"
#include"VQ.h"
#include"LawFilters.h"
#include <fstream>
#include<iomanip>
#include<opencv2\opencv.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\core\core.hpp>
using namespace std;
using namespace cv;
vector2d normalize(vector2d data, int N, int D) {
	vector2d normedData(N, vector<double>(D - 1, 0));
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

vector2d genResponses5x5(unsigned char* imageData, int ws, int width, int height) {
	cout << "generate data" << endl;
	LawFilters lawfilter = LawFilters();
	vector2d filterResponses;
	for (int i = 0; i < 24; i++) {
		vector<double> filterResponse = Image::filter2d(imageData, lawfilter.getFilter5X5(i), width, height);
		cout << i << "th filter finished" << endl;
		filterResponses.push_back(filterResponse);
	}

	//transpose data
	int dataSize = filterResponses.at(0).size();
	vector2d responseDataInRow(dataSize, vector<double>(24, 0));
	for (int i = 0; i < dataSize; i++) {
		for (int j = 0; j < 24; j++) {
			responseDataInRow[i][j] = filterResponses[j][i];
		}
	}
	cout << "transpose finished" << endl;

	int filterDataWidth = width - 4;
	int filterDataHeight = height - 4;
	vector2d finalData;
	//gather neighbourhood data
	for (int i = 0; i < filterDataHeight; i++) {
		for (int j = 0; j < filterDataWidth; j++) {
			vector2d blockDataset;
			for (int m = -ws / 2; m < ws / 2 + 1; m++) {
				for (int n = -ws / 2; n < ws / 2 + 1; n++) {
					int posV = i + m;
					int posH = j + n;
					if (posV < 0) posV = 0;
					if (posV >= filterDataHeight) posV = filterDataHeight - 1;
					if (posH < 0) posH = 0;
					if (posH >= filterDataWidth) posH = filterDataWidth - 1;
					blockDataset.push_back(responseDataInRow.at(posV * filterDataWidth + posH));
				}
			}
			vector<double> avgEnergyResponse = genABSMean(blockDataset, 24);
			finalData.push_back(avgEnergyResponse);
			cout << ((double)(i*filterDataWidth + j) / (dataSize)) * 100 << "%" << " " << i << " " << j << '\r';
		}
	}

	return finalData;
}


int saveData(vector2d data, string filename) {
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

vector2d readData(string filename, int D) {
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
		if (count % D == 0) {
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
	int windowSize = 15;
	vector2d data = genResponses5x5(imageData, windowSize, image.getWidth(), image.getHeight());
	cout << "finish gathering data" << endl;
	cout << "saving to file ....." << endl;
	saveData(data, "responsedDataw15.dat");

	data = readData("responsedDataw15.dat", 24);
	//cout << "data loaded!" << endl;

	//PCA

	Mat matData = Mat::zeros(data.size(), 24, CV_32F);
	//cout << data << endl;
	//cout << data.reshape(1,1) << endl;
	for (int i = 0; i < data.size(); i++) {
		for (int j = 0; j < 24; j++) {
			matData.at<float>(i, j)= data[i][j];
		}
	}
	PCA pca(matData, Mat(), PCA::DATA_AS_ROW, 18);
	Mat dstp18 = pca.project(matData);

	PCA pca2(matData, Mat(), PCA::DATA_AS_ROW, 15);
	Mat dstp15 = pca2.project(matData);

	//Mat reconstruction = pca.backProject(dst);
	//cout << reconstruction.at<float>(1);

	//vector2d pcaData;
	//for (int i = 0; i < dst.rows; i++) {
	//	vector<double> rowData;
	//	for (int j = 0; j < dst.cols; j++) {
	//		rowData.push_back(dst.at<float>(i, j));
	//	}
	//	pcaData.push_back(rowData);
	//}
	//K-mean
	//VQ classifier1 = VQ(pcaData, 6);
	//classifier1.dataDescription();
	//vector<int> label = classifier1.classify(2);

	//no pca
	Mat centers, labels;
	kmeans(matData, 6, labels,
		TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 200, 0.001),
		3, KMEANS_PP_CENTERS, centers);

	Mat centersp18, labelsp18;
	kmeans(dstp18, 6, labelsp18,
		TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 200, 0.001),
		3, KMEANS_PP_CENTERS, centersp18);

	Mat centersp15, labelsp15;
	kmeans(dstp15, 6, labelsp15,
		TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 200, 0.001),
		3, KMEANS_PP_CENTERS, centersp15);


	//cout << "training finished" << endl;
	//saveLabelData(label, "labels.dat");


	//label image
	//vector<int> labels = readLabelData("labels.dat");
	int size = (image.getWidth() - 4)*(image.getHeight() - 4);
	unsigned char* labelImage = (unsigned char*)malloc(sizeof(unsigned char)* size);
	for (int i = 0; i < size; i++) {
		labelImage[i] = (unsigned char)( labels.at<int>(i) * 55);
	}
	Image::writeFile(labelImage, image.getWidth() - 4, image.getHeight() - 4, 1, "labelImagew15.raw");
	cout << "label image write finished" << endl;

	unsigned char* labelImagep18 = (unsigned char*)malloc(sizeof(unsigned char)* size);
	for (int i = 0; i < size; i++) {
		labelImage[i] = (unsigned char)(labelsp18.at<int>(i) * 55);
	}
	Image::writeFile(labelImage, image.getWidth() - 4, image.getHeight() - 4, 1, "labelImagew15p18.raw");
	cout << "label image write finished" << endl;

	unsigned char* labelImagep15 = (unsigned char*)malloc(sizeof(unsigned char)* size);
	for (int i = 0; i < size; i++) {
		labelImage[i] = (unsigned char)(labelsp15.at<int>(i) * 55);
	}
	Image::writeFile(labelImage, image.getWidth() - 4, image.getHeight() - 4, 1, "labelImagew15p15.raw");
	cout << "label image write finished" << endl;

	getchar();
	return 0;
}