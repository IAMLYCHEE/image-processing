#include "opencv2/xfeatures2d.hpp"
#include <opencv2\core.hpp>
#include<opencv2\features2d.hpp>
#include<opencv2\highgui.hpp>
#include<iostream>
#include<vector>
#include<iomanip>
using namespace cv;

double norml2(std::vector<double> v1, std::vector<double> v2) {
	double dist = 0.0;
	for (int i = 0; i < v1.size(); i++) {
		dist += (v1[i] - v2[i])*(v1[i] - v2[i]);
	}
	return dist;
}


int main() {
	//train
	Mat img_1 = imread("ferrari1.jpg", IMREAD_GRAYSCALE);
	Mat img_2 = imread("Optimus_Prime.jpg", IMREAD_GRAYSCALE);
	Mat img_3 = imread("bumblebee.jpg", IMREAD_GRAYSCALE);
	//test
	Mat img_4 = imread("ferrari_2.jpg", IMREAD_GRAYSCALE);
	if ((!img_1.data || !img_2.data) || (!img_3.data || !img_4.data))
	{
		std::cout << " --(!) Error reading images " << std::endl; return -1;
	}

	int nFeature = 500;

	cv::Ptr<Feature2D> f2d = xfeatures2d::SIFT::create(nFeature,3,0.04,20,1.6);

	//-- Step 1: Detect the keypoints:
	std::vector<KeyPoint> keypoints_1, keypoints_2, keypoints_3, keypoints_4;
	f2d->detect(img_1, keypoints_1);
	f2d->detect(img_2, keypoints_2);
	f2d->detect(img_3, keypoints_3);
	f2d->detect(img_4, keypoints_4);

	//-- Step 2: Calculate descriptors (feature vectors)    
	Mat descriptors_1, descriptors_2,descriptors_3, descriptors_4;
	f2d->compute(img_1, keypoints_1, descriptors_1);
	f2d->compute(img_2, keypoints_2, descriptors_2);
	f2d->compute(img_3, keypoints_3, descriptors_3);
	f2d->compute(img_4, keypoints_4, descriptors_4);

	//generate train data
	Mat descriptors[] = { descriptors_1,descriptors_2,descriptors_3 };
	Mat trainData;
	vconcat(descriptors, 3, trainData);

	//kmean
	Mat centers,labels;
	kmeans(trainData, 8, labels,
		TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 200, 0.001),
		3, KMEANS_PP_CENTERS, centers);


	//get codebook
	std::vector<std::vector<double>> codebook(3, std::vector<double>(8,0.0));
	for (int i = 0; i < labels.rows; i++) {
		if (i < descriptors_1.rows) {
			codebook[0][labels.at<int>(i, 0)]+= 1.0/descriptors_1.rows;
		}
		else if (i < descriptors_1.rows+descriptors_2.rows) {
			codebook[1][labels.at<int>(i, 0)]+=1.0/descriptors_2.rows;
		}
		else {
			codebook[2][labels.at<int>(i, 0)]+=1.0/descriptors_3.rows;
		}
	}

	std::cout << "the codebook is:" << std::endl;
	for (auto e : codebook) {
		for (auto ee : e) {
			std::cout <<std::setw(12) << ee << " ";
		}
		std::cout << std::endl;
	}


	//get feature of test image
	std::vector<double> codeTestImage(8,0.0);
	for (int i = 0; i < descriptors_4.rows; i++) {
		Mat rowData = descriptors_4.row(i);
		Mat centroid = centers.row(0);
		int label = 0;
		double minDist = norm(rowData, centroid, NORM_L2);
		for (int j = 1; j < 8; j++) {
			double dist = norm(rowData, centers.row(j), NORM_L2);
			if (dist < minDist) {
				minDist = dist;
				label = j;
			}
		}
		codeTestImage[label]+= 1.0/descriptors_4.rows;
	}

	std::cout << "the code for test image is:"<<std::endl;
	for (auto e : codeTestImage) {
		std::cout << std::setw(12) << e << " ";
	}

	//find corresponding class
	double minDist = norml2(codeTestImage, codebook.at(0));
	int classIndex = 0;
	for (int i = 1; i < 3; i++) {
		double dist = norml2(codeTestImage, codebook.at(i));
		if (dist < minDist) {
			classIndex = i;
			minDist = dist;
		}
	}
	std::cout << std::endl;
	std::cout << "the result is :" << std::endl;

	switch (classIndex)
	{
	case 0: std::cout << "it is a Ferrari" << std::endl;
		break;
	case 1: std::cout << "it is a Optimus Prime Truck" << std::endl;
		break;
	case 2: std::cout << "it is a Bumblebee" << std::endl;
		break;
	} 

	getchar();

	waitKey();
}