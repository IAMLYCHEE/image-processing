#include<opencv2\opencv.hpp>
#include<opencv2\highgui\highgui.hpp>
#include <iostream>
#include<opencv2\core\core.hpp>
#include<vector>

using namespace cv;
using namespace std;

int PCAtest(int windowSize,int slidingSize, double rv) {


	Mat originData = imread("House_noisy.jpg");
	if (originData.empty()) {
		return -1;
	}

	cvtColor(originData, originData, cv::COLOR_RGB2GRAY);


	int width = 256;
	int height = 256;
	int windowSize = 9;
	int slidingSize = 17;
	Mat result = Mat::zeros(width, height, CV_32F);
	originData.copyTo(result);
	for (int i =0; i < height -windowSize; i++) {
		for (int j = 0; j < width-windowSize; j++) {
			vector<Mat> datas;
			Mat dataji = Mat::zeros(windowSize, windowSize, CV_32F);
			originData(Rect(j, i, windowSize, windowSize)).copyTo(dataji);
			datas.push_back(dataji.reshape(1, 1));
			for (int k = -slidingSize / 2; k < slidingSize / 2 + 1; k+=(windowSize-1)/2) {
				for (int h = -slidingSize / 2; h < slidingSize / 2 + 1; h+=(windowSize - 1) / 2) {
					int x = j + h;
					int y = i + k;
					if (x < 0) {
						x = 0;
					}
					if (x  > width -  windowSize) {
						x = width - windowSize ;
					}
					if (y < 0) {
						y = 0;
					}
					if (y  > height - windowSize) {
						y = height - windowSize ;
					}

					//cout << x << " " << y << endl;
					Mat data = Mat::zeros(windowSize, windowSize, CV_32F);
					originData(Rect(x, y, windowSize, windowSize)).copyTo(data);
					//cout << data << endl;
					//cout << data.reshape(1,1) << endl;
					datas.push_back(data.reshape(1, 1));
				}
			}
			Mat stackedData = Mat::zeros(datas.size(), windowSize*windowSize, CV_32F);
			//cout << datas.at(0) << endl;
			for (int index = 0; index < datas.size(); index++) {
				datas.at(index).copyTo(stackedData.row(index));
			}
			//cout << stackedData << endl; test OK!
			PCA pca(stackedData, Mat(), PCA::DATA_AS_ROW, rv);
			Mat dst = pca.project(stackedData);
			Mat reconstruction = pca.backProject(dst);
			Mat newdata1 = reconstruction.row(0).reshape(1, windowSize);

			newdata1.copyTo(result(cv::Rect(j, i, windowSize, windowSize)));
			datas.clear();
			//just for fun, show the process
			cout << "[[";
			double process = (double)(i*width + j) / (double)((width-windowSize)*(height-windowSize)) * 100;
			for (int numOfStar = 0; numOfStar < (int)process / 2; numOfStar++) {
				cout << "#";
			}
			for (int numOfStar = 0; numOfStar < (int)(100 - process) / 2; numOfStar++) {
				cout << " ";
			}

			cout << "]] " << process << "%\r";
		}
	}


	namedWindow("origin", WINDOW_AUTOSIZE);
	imshow("origin", originData);
	result.convertTo(result, CV_8UC1);
	namedWindow("PCA", WINDOW_AUTOSIZE);
	imshow("PCA", result);
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_params.push_back(100);

	imwrite("housePCA.jpg", result, compression_params);
	waitKey(0);

	getchar();
}

double PSNR(Mat ori, Mat noi) {
	Mat error = ori - noi;
	double sum = 0.0;
	for (int i = 0; i < error.rows; i++) {
		for (int j = 0; j < error.cols; j++) {
			sum = sum + pow(error.at<unsigned char>(i, j),2);
		}
	}
	double MSE = sum / (ori.cols*ori.rows);
	return (10.0*log10(255.0*255.0 / MSE));
}

int main(int argc,char* argv[]) {

	int windowsize = atoi(argv[1]);
	int slidingsize = atoi(argv[2]);
	double retainv = atof(argv[3]);
	

	
	PCAtest(windowsize,slidingsize,retainv);




}