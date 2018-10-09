#include "opencv2/xfeatures2d.hpp"
#include <opencv2\core.hpp>
#include<opencv2\features2d.hpp>
#include<opencv2\highgui.hpp>
#include<iostream>

using namespace cv;
int main() {

	Mat img_1 = imread("ferrari1.jpg", IMREAD_GRAYSCALE);
	Mat img_2 = imread("bumblebee.jpg", IMREAD_GRAYSCALE);

	if (!img_1.data || !img_2.data)
	{
		std::cout << " --(!) Error reading images " << std::endl; return -1;
	}

	cv::Ptr<Feature2D> f2d = xfeatures2d::SIFT::create(100,4,0.04,20,1.6);

	//-- Step 1: Detect the keypoints:
	std::vector<KeyPoint> keypoints_1, keypoints_2;
	f2d->detect(img_1, keypoints_1);
	f2d->detect(img_2, keypoints_2);

	//-- Step 2: Calculate descriptors (feature vectors)    
	Mat descriptors_1, descriptors_2;
	f2d->compute(img_1, keypoints_1, descriptors_1);
	f2d->compute(img_2, keypoints_2, descriptors_2);

	//-- Step 3: Matching descriptor vectors using BFMatcher :
	BFMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match(descriptors_1, descriptors_2, matches);
	Mat matchImg;
	drawMatches(img_1, keypoints_1, img_2, keypoints_2, matches, matchImg);
	imshow("match image", matchImg);
	imwrite("matchSift100.jpg", matchImg);



	cv::Ptr<Feature2D> f2dsurf = xfeatures2d::SURF::create(5000, 4, 3, false, false);

	//-- Step 1: Detect the keypoints:
	std::vector<KeyPoint> keypoints_1_surf, keypoints_2_surf;
	f2dsurf->detect(img_1, keypoints_1_surf);
	f2dsurf->detect(img_2, keypoints_2_surf);

	//-- Step 2: Calculate descriptors (feature vectors)    
	Mat descriptors_1_surf, descriptors_2_surf;
	f2d->compute(img_1, keypoints_1_surf, descriptors_1_surf);
	f2d->compute(img_2, keypoints_2_surf, descriptors_2_surf);

	//-- Step 3: Matching descriptor vectors using BFMatcher :
	BFMatcher matcher_surf;
	std::vector< DMatch > matches_surf;
	matcher_surf.match(descriptors_1_surf, descriptors_2_surf, matches_surf);
	Mat matchImg_surf;
	drawMatches(img_1, keypoints_1_surf, img_2, keypoints_2_surf, matches_surf, matchImg_surf);
	imshow("match image surf", matchImg_surf);
	imwrite("matchSurf.jpg", matchImg_surf);


	waitKey();
	return 0;
}