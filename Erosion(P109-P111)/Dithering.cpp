#include "Dithering.h"

int I2[2][2] =
{
	{ 1,2 },
	{ 3,0 }
};
int I4[4][4] = { 0 };
int I8[8][8] = { 0 };


vector<vector<double>> floyd = { { 0,0,7.0 / 16.0 },{ 3.0 / 16.0,5.0 / 16.0,1.0 / 16.0 } };
vector<vector<double>> javis = { { 0,0,0,7.0 / 48.0,5.0 / 48.0 },
								{ 3.0 / 48.0,5.0 / 48.0,7.0 / 48.0,5.0 / 48.0,3.0 / 48.0 },
								{ 1.0 / 48.0,3.0 / 48.0,5.0 / 48.0,3.0 / 48.0,1.0 / 48.0 } };
vector<vector<double>> stucki = { { 0,0,0,8.0 / 42.0,4.0 / 42.0 },
								{ 2.0 / 42.0,4.0 / 42.0,8.0 / 42.0,4.0 / 42.0,2.0 / 42.0 },
								{ 1.0 / 42.0,2.0 / 42.0,4.0 / 42.0,2.0 / 42.0,1.0 / 42.0 } };



int generateKer() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int p = 2 * (i / 2) + j / 2;
			int a = (p + 1) % 4;
			I4[i][j] = I2[i % 2][j % 2] * 4 + a;
		}
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			int p = 2 * (i / 4) + j / 4;
			int a = (p + 1) % 4;
			I8[i][j] = I4[i % 4][j % 4] * 4 + a;
		}
	}


	return 0;
}

unsigned char findT(unsigned char data) {
	int value = round((double)data / 255.0 * 3.0) * 85;
	return (unsigned char)value;
}

unsigned char * Dithering::fixThre(unsigned char * data, int width, int height, int channel, int Thre)
{
	int dataSize = width * height * channel;
	unsigned char * result = (unsigned char*)malloc(sizeof(unsigned char)*dataSize);
	for (int i = 0; i < dataSize; i++) {
		if ((int)data[i] >= Thre) {
			result[i] = (unsigned char)255;
		}
		else {
			result[i] = (unsigned char)0;
		}
	}
	return result;
}


unsigned char * Dithering::fixThreT4(unsigned char * data, int width, int height, int channel)
{
	int dataSize = width * height * channel;
	unsigned char * result = (unsigned char*)malloc(sizeof(unsigned char)*dataSize);
	for (int i = 0; i < dataSize; i++) {
		result[i] = findT(data[i]);
	}
	return result;
}

unsigned char * Dithering::randThre(unsigned char * data, int width, int height, int channel)
{
	int dataSize = width * height * channel;
	unsigned char * result = (unsigned char*)malloc(sizeof(unsigned char)*dataSize);

	//random generator
	default_random_engine generator;
	uniform_int_distribution<int> distribution(0, 255);

	for (int i = 0; i < dataSize; i++) {
		int Thre = distribution(generator);
		if ((int)data[i] >= Thre) {
			result[i] = (unsigned char)255;
		}
		else {
			result[i] = (unsigned char)0;
		}
	}
	return result;
}

unsigned char * Dithering::matrixN2(unsigned char * data, int width, int height, int channel)
{
	generateKer();
	int dataSize = width * height*channel;
	unsigned char * matrixI2 = (unsigned char*)malloc(sizeof(unsigned char)*dataSize);
	int N = 2;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double Thre = (I2[i % N][j % N] + 0.5) / (double(N * N));
			if (((double)data[i*width + j]) >(Thre*255.0)) {
				matrixI2[i*width + j] = (unsigned char)255;
			}
			else {
				matrixI2[i*width + j] = (unsigned char)0;
			}
		}
	}
	return matrixI2;
}

unsigned char * Dithering::matrixN4(unsigned char * data, int width, int height, int channel)
{
	generateKer();
	int dataSize = width * height*channel;
	unsigned char * matrixI4 = (unsigned char*)malloc(sizeof(unsigned char)*dataSize);
	int N = 4;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double Thre = (I4[i % N][j % N] + 0.5) / (double(N * N));
			if (((double)data[i*width + j]) >(Thre*255.0)) {
				matrixI4[i*width + j] = (unsigned char)255;
			}
			else {
				matrixI4[i*width + j] = (unsigned char)0;
			}
		}
	}
	return matrixI4;
}


unsigned char * Dithering::matrixN4keep(unsigned char * data, int width, int height, int channel)
{
	generateKer();
	int dataSize = width * height*channel;
	unsigned char * matrixI4 = (unsigned char*)malloc(sizeof(unsigned char)*dataSize);
	int N = 4;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double Thre = (I4[i % N][j % N] + 0.5) / (double(N * N));
			if (((double)data[i*width + j]) <= (Thre*255.0)) {
				matrixI4[i*width + j] = (unsigned char)0;
			}
			else {
				matrixI4[i*width + j] = data[i*width + j];
			}
		}
	}
	return matrixI4;
}

unsigned char * Dithering::matrixN8(unsigned char * data, int width, int height, int channel)
{
	generateKer();
	int dataSize = width * height*channel;
	unsigned char * matrixI8 = (unsigned char*)malloc(sizeof(unsigned char)*dataSize);
	int N = 8;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double Thre = (I8[i % N][j % N] + 0.5) / (double(N * N));
			if (((double)data[i*width + j]) >(Thre*255.0)) {
				matrixI8[i*width + j] = (unsigned char)255;
			}
			else {
				matrixI8[i*width + j] = (unsigned char)0;
			}
		}
	}
	return matrixI8;
}


unsigned char * Dithering::matrixN4T4(unsigned char * data, int width, int height, int channel)
{
	generateKer();
	int dataSize = width * height*channel;
	unsigned char * matrixI4 = (unsigned char*)malloc(sizeof(unsigned char)*dataSize);
	int N = 4;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double Thre = (I4[i % N][j % N] + 0.5) / (double(N * N));
			if (((double)data[i*width + j]) >(Thre*255.0)) {
				matrixI4[i*width + j] = findT(data[i*width + j]);
			}
			else {
				matrixI4[i*width + j] = (unsigned char)0;
			}
		}
	}
	return matrixI4;
}


double constraint(double value) {
	if (value < 0) {
		return 
		0.0;
	}
	else if (value > 255) {
		return 255.0;
	}
	return value;
}

unsigned char * Dithering::errorDif(unsigned char * imageData, int width, int height, int method, int wSize) {
	vector<vector<vector<double>>> matrixs;
	matrixs.push_back(floyd);
	matrixs.push_back(javis);
	matrixs.push_back(stucki);

	vector<vector<double>> wMatrix = matrixs.at(method);
	vector<double> data;

	for (int i = 0; i < width * height; i++) {
		data.push_back((double)imageData[i]);
	}

	unsigned char * result = (unsigned char *)malloc(sizeof(unsigned char)*width * height);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int p = i * width + j;
			int oldvalue = (int)data[p];
			result[p] = (unsigned char)(oldvalue / 127 * 255);
			int err = oldvalue - result[p];
			for (int k = wSize / 2 + 1; k < wSize; k++) {
				if ((j + k) < width) {
					data[p + k - wSize / 2] += (wMatrix[0][k] * err);
					data[p + k - wSize / 2] = constraint(data[p + k - wSize / 2]);
				}
			}
			for (int jj = 1; jj < wSize / 2 + 1; jj++) {
				for (int k = -wSize / 2; k < wSize / 2 + 1; k++) {
					if ((i + jj) < height && (j + k) < width && (j+k)>0) {
						data[p + jj*width + k] += (wMatrix[jj][k + wSize / 2] * err);
						data[p + jj*width + k] = constraint(data[p + jj*width + k]);
					}
				}
			}
		}
	}

	return result;
}

