#include<iostream>
#include"Image.h"
#include <cmath>


using namespace std;

double max(double x, double y) {
	return abs(x) > abs(y) ? abs(x) : abs(y);
}

unsigned char* warpDisk1(unsigned char* image, int width, int height) {
	unsigned char* warpImage = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
	//warpImage = { 0 };
	double half = (double)(width - 1) / 2.0;
	for (int i = 0; i < width * height; i++) {
		warpImage[i] = 0;
	}
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double xPrime = (double)j - half;
			double yPrime = half - (double)i;
			double length = sqrt(xPrime*xPrime + yPrime * yPrime);
			double x;
			double y;
			if (abs(xPrime) > abs(yPrime)) {
				x = length * xPrime / abs(xPrime);
				y = abs(x / xPrime) * yPrime;
			}
			else {
				y = length * yPrime / abs(yPrime);
				x = abs(y / yPrime)*xPrime;
			}

			int col = (int)(x + half);
			int row = (int)(half-y);

			if (col >= 0 && col <= width && row >= 0 && row <= height) {
				warpImage[i * width + j] = image[row * width + col];
			}
		}
	}
	return warpImage;
}

unsigned char* warpDisk5(unsigned char* image, int width, int height) {
	unsigned char* warpImage = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
	double half = (double)(width - 1) / 2.0;
	for (int i = 0; i < width * height; i++) {
		warpImage[i] = 0;
	}
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double xPrime = (double)j - half;
			double yPrime = half - (double)i;

			double xnp = xPrime / half;
			double ynp = yPrime / half;
			double length = xnp*xnp + ynp*ynp;
			if (length <= 1) {
				double y = ynp;
				double a = (1 - ynp*ynp) / (xnp*xnp);
				double x = sqrt(1 / a) * xnp / abs(xnp);
				x = x * half;
				y = y * half;
				int col = (int)(x + half);
				int row = (int)(half - y);
				warpImage[i * width + j] = image[row * width + col];
			}

		}
	}
	return warpImage;
}

unsigned char* restore(unsigned char* image, int width, int height) {
		unsigned char* restoreImage = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
		double half = (double)(width - 1) / 2.0;
		for (int i = 0; i < width * height; i++) {
			restoreImage[i] = 0;
		}
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				double xPrime = (double)j - half;
				double yPrime = half - (double)i;

				double xnp = xPrime / half;
				double ynp = yPrime / half;

				double y = ynp;
				double a = 1 / (xnp*xnp);
				double x = sqrt((1 - ynp*ynp) / a)* xnp / abs(xnp);
				x = x * half;
				y = y * half;
				int col = (int)(x + half);
				int row = (int)(half - y);
				restoreImage[i * width + j] = image[row * width + col];
			}
		}
		return restoreImage;
}

int main() {
	Image image = Image(512, 512, 3, "panda.raw");

	unsigned char* data = image.imageData();
	unsigned char* redCom = image.getRedCom();
	unsigned char* greenCom = image.getGreenCom();
	unsigned char* blueCom = image.getBlueCom();

	int size = image.getWidth() * image.getHeight();

	unsigned char* warpRed2 = warpDisk5(redCom, image.getWidth(), image.getHeight());
	unsigned char* warpGreen2 = warpDisk5(greenCom, image.getWidth(), image.getHeight());
	unsigned char* warpBlue2 = warpDisk5(blueCom, image.getWidth(), image.getHeight());

	unsigned char* warpedImage2 = Image::reconstruct(warpRed2, warpGreen2, warpBlue2, size * 3);
	Image::writeFile(warpedImage2, image.getWidth(), image.getHeight(), 3, "warpedpanda.raw");

	unsigned char* restoreRed = restore(warpRed2, image.getWidth(), image.getHeight());
	unsigned char* restoreGreen = restore(warpGreen2, image.getWidth(), image.getHeight());
	unsigned char* restoreBlue = restore(warpBlue2, image.getWidth(), image.getHeight());

	unsigned char* restoreImage = Image::reconstruct(restoreRed, restoreGreen, restoreBlue, size * 3);
	Image::writeFile(restoreImage, image.getWidth(), image.getHeight(), 3, "restorepanda.raw");

	return 0;
}