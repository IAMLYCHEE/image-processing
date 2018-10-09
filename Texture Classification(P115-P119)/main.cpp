#include<iostream>
#include<vector>
#include"Image.h"
#include"LawFilters.h"
#include<string>
#include"VQ.h"
using namespace std;

int main() {
	LawFilters lawfilter = LawFilters();
	vector2d imageResponses;
	std::cout << "the responses are:" << endl;
	for (int imageIndex = 1; imageIndex < 13; imageIndex++) {
		//image information
		string filename = "texture" + to_string(imageIndex) + ".raw";
		Image image = Image(128, 128, 1, filename.c_str());
		unsigned char* imagedata = image.imageData();
		int width = image.getWidth();
		int height = image.getHeight();
		//get image mean
		double imageMean = Image::getmean(imagedata, width, height);
		//gather nine filter responses
		vector<double> responses;
		for (int i = 0; i < 9; i++) {
			//get responses for i feature kernel
			vector<double> filterResponse = Image::filter2d(imagedata, lawfilter.getFilter5X5(i), width, height);
			double responseMean = Image::getABSmean(filterResponse);
			responses.push_back(responseMean);
		}
		//check responses
		std::cout << filename << ":";
		std::cout << "{";
		for (auto e : responses) {
			std::cout << e << ", ";
		}
		std::cout << "},";
		std::cout << endl;
		imageResponses.push_back(responses);
	}
	
	//classify
	std::cout << "start classifying" << endl;
	VQ classifier = VQ(imageResponses, 4);
	classifier.dataDescription();
	vector<int> label = classifier.classify(2);//1:init with mean 2: init with rand  3 : init with GA
	std::cout << "result" << endl;
	int imageIndex = 0;
	for (auto e : label) {
		imageIndex++;
		string filename = "texture" + to_string(imageIndex) + ".raw";
		std::cout << filename << " in class " << e << endl ;
	}


	getchar();
	return 0;
}