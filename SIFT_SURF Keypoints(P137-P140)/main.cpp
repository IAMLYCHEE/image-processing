#include <stdio.h>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"
#include<ctime>

using namespace cv;
using namespace cv::xfeatures2d;

/** @function main */
int main(int argc, char** argv)
{


	Mat img_1 = imread("bumblebee.jpg", IMREAD_GRAYSCALE);
	Mat img_2 = imread("Optimus_Prime.jpg", IMREAD_GRAYSCALE);

	if (!img_1.data || !img_2.data)
	{
		std::cout << " --(!) Error reading images " << std::endl; return -1;
	}

	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 7000;

	//==========================SURF==============================//
	Ptr<SURF> detector = SURF::create(minHessian,4,3,false,false);

	std::vector<KeyPoint> keypoints_1, keypoints_2;

	std::cout << "SURF detection time:";
	std::clock_t start;
	start = std::clock();
	detector->detect(img_1, keypoints_1);
	detector->detect(img_2, keypoints_2);
	double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << duration << std::endl;


	//-- Draw keypoints
	Mat img_keypoints_1; Mat img_keypoints_2;

	drawKeypoints(img_1, keypoints_1, img_keypoints_1, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(img_2, keypoints_2, img_keypoints_2, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	//-- Show detected (drawn) keypoints
	imshow("Keypoints 1 SURF", img_keypoints_1);
	imshow("Keypoints 2 SURF", img_keypoints_2);
	imwrite("surf1.jpg", img_keypoints_1);
	imwrite("surf2.jpg", img_keypoints_2);



	//========================SIFT=========================//
	Ptr<SIFT> detector2 = SIFT::create(400,4,0.04,20,1.6);

	std::vector<KeyPoint> keypoints_1_SIFT, keypoints_2_SIFT;

	std::cout << "SIFT detection time:";
	start = std::clock();
	detector2->detect(img_1, keypoints_1_SIFT);
	detector2->detect(img_2, keypoints_2_SIFT);
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << duration << std::endl;


	//-- Draw keypoints
	Mat img_keypoints_1_SIFT; Mat img_keypoints_2_SIFT;

	drawKeypoints(img_1, keypoints_1_SIFT, img_keypoints_1_SIFT, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(img_2, keypoints_2_SIFT, img_keypoints_2_SIFT, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	//-- Show detected (drawn) keypoints
	imshow("Keypoints 1 SIFT", img_keypoints_1_SIFT);
	imshow("Keypoints 2 SIFT", img_keypoints_2_SIFT);
	imwrite("sift1.jpg", img_keypoints_1_SIFT);
	imwrite("sift2.jpg", img_keypoints_2_SIFT);
	waitKey(0);

	return 0;
}