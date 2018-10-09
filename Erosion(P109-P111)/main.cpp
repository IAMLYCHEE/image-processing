#include<iostream>
#include<vector>
#include"Image.h"
#include"Dithering.h"
#include<string>
using namespace std;

vector<vector<vector<int>>> sPatterns =
{
	{//1
		{0,0,1,0,1,0,0,0,0},{1,0,0,0,1,0,0,0,0},{0,0,0,0,1,0,1,0,0},{0,0,0,0,1,0,0,0,1}
	},
	{//2
		{0,0,0,0,1,1,0,0,0},{0,1,0,0,1,0,0,0,0},{0,0,0,1,1,0,0,0,0},{0,0,0,0,1,0,0,1,0}
	},
	{//3
		{0,0,1,0,1,1,0,0,0},{0,1,1,0,1,0,0,0,0},{1,1,0,0,1,0,0,0,0},{1,0,0,1,1,0,0,0,0},
		{0,0,0,1,1,0,1,0,0},{0,0,0,0,1,0,1,1,0},{0,0,0,0,1,0,0,1,1},{0,0,0,0,1,1,0,0,1}
	},
	{//4
		{0,0,1,0,1,1,0,0,1},{1,1,1,0,1,0,0,0,0},{1,0,0,1,1,0,1,0,0},{0,0,0,0,1,0,1,1,1}
	},
	{//5
		{1,1,0,0,1,1,0,0,0},{0,1,0,0,1,1,0,0,1},{0,1,1,1,1,0,0,0,0},{0,0,1,0,1,1,0,1,0},
		{0,1,1,0,1,1,0,0,0},{1,1,0,1,1,0,0,0,0},{0,0,0,1,1,0,1,1,0},{0,0,0,0,1,1,0,1,1}
	},
	{//6
		{1,1,0,0,1,1,0,0,1},{0,1,1,1,1,0,1,0,0},
		{1,1,1,0,1,1,0,0,0},{0,1,1,0,1,1,0,0,1},{1,1,1,1,1,0,0,0,0},{1,1,0,1,1,0,1,0,0},
		{1,0,0,1,1,0,1,1,0},{0,0,0,1,1,0,1,1,1},{0,0,0,0,1,1,1,1,1},{0,0,1,0,1,1,0,1,1}
	},
	{//7
		{1,1,1,0,1,1,0,0,1},{1,1,1,1,1,0,1,0,0},{1,0,0,1,1,0,1,1,1},{0,0,1,0,1,1,1,1,1}
	},
	{//8
		{0,1,1,0,1,1,0,1,1},{1,1,1,1,1,1,0,0,0},{1,1,0,1,1,0,1,1,0},{0,0,0,1,1,1,1,1,1}
	},
	{//9
		{1,1,1,0,1,1,0,1,1},{0,1,1,0,1,1,1,1,1},{1,1,1,1,1,1,1,0,0},{1,1,1,1,1,1,0,0,1},
		{1,1,1,1,1,0,1,1,0},{1,1,0,1,1,0,1,1,1},{1,0,0,1,1,1,1,1,1},{0,0,1,1,1,1,1,1,1}
	},
	{//10
		{1,1,1,0,1,1,1,1,1},{1,1,1,1,1,1,1,0,1},{1,1,1,1,1,0,1,1,1},{1,0,1,1,1,1,1,1,1}
	}
};

vector<vector<vector<int>>> tPatterns =
{
	{//4
		{ 0,1,0,0,1,1,0,0,0 },{ 0,1,0,1,1,0,0,0,0 },{ 0,0,0,1,1,0,0,1,0 },{ 0,0,0,0,1,1,0,1,0 },
		{ 0,0,1,0,1,1,0,0,1 },{ 1,1,1,0,1,0,0,0,0 },{ 1,0,0,1,1,0,1,0,0 },{ 0,0,0,0,1,0,1,1,1 }
	},
	{//5
		{ 1,1,0,0,1,1,0,0,0 },{ 0,1,0,0,1,1,0,0,1 },{ 0,1,1,1,1,0,0,0,0 },{ 0,0,1,0,1,1,0,1,0 },
		{ 0,1,1,0,1,1,0,0,0 },{ 1,1,0,1,1,0,0,0,0 },{ 0,0,0,1,1,0,1,1,0 },{ 0,0,0,0,1,1,0,1,1 }
	},
	{//6
		{ 1,1,0,0,1,1,0,0,1 },{ 0,1,1,1,1,0,1,0,0 },
		{ 1,1,1,0,1,1,0,0,0 },{ 0,1,1,0,1,1,0,0,1 },{ 1,1,1,1,1,0,0,0,0 },{ 1,1,0,1,1,0,1,0,0 },
		{ 1,0,0,1,1,0,1,1,0 },{ 0,0,0,1,1,0,1,1,1 },{ 0,0,0,0,1,1,1,1,1 },{ 0,0,1,0,1,1,0,1,1 }
	},
	{//7
		{ 1,1,1,0,1,1,0,0,1 },{ 1,1,1,1,1,0,1,0,0 },{ 1,0,0,1,1,0,1,1,1 },{ 0,0,1,0,1,1,1,1,1 }
	},
	{//8
		{ 0,1,1,0,1,1,0,1,1 },{ 1,1,1,1,1,1,0,0,0 },{ 1,1,0,1,1,0,1,1,0 },{ 0,0,0,1,1,1,1,1,1 }
	},
	{//9
		{ 1,1,1,0,1,1,0,1,1 },{ 0,1,1,0,1,1,1,1,1 },{ 1,1,1,1,1,1,1,0,0 },{ 1,1,1,1,1,1,0,0,1 },
		{ 1,1,1,1,1,0,1,1,0 },{ 1,1,0,1,1,0,1,1,1 },{ 1,0,0,1,1,1,1,1,1 },{ 0,0,1,1,1,1,1,1,1 }
	},
	{//10
		{ 1,1,1,0,1,1,1,1,1 },{ 1,1,1,1,1,1,1,0,1 },{ 1,1,1,1,1,0,1,1,1 },{ 1,0,1,1,1,1,1,1,1 }
	}
};


vector<vector<vector<int>>> kPatterns = 
{
	{//4
		{ 0,1,0,0,1,1,0,0,0 },{ 0,1,0,1,1,0,0,0,0 },{ 0,0,0,1,1,0,0,1,0 },{ 0,0,0,0,1,1,0,1,0 },
		{ 0,0,1,0,1,1,0,0,1 },{ 1,1,1,0,1,0,0,0,0 },{ 1,0,0,1,1,0,1,0,0 },{ 0,0,0,0,1,0,1,1,1 }
	},
	{//5
		{ 0,0,0,0,0,0,0,0,0 }
	},
	{//6
		{ 1,1,1,0,1,1,0,0,0 },{ 0,1,1,0,1,1,0,0,1 },{ 1,1,1,1,1,0,0,0,0 },{ 1,1,0,1,1,0,1,0,0 },
		{ 1,0,0,1,1,0,1,1,0 },{ 0,0,0,1,1,0,1,1,1 },{ 0,0,0,0,1,1,1,1,1 },{ 0,0,1,0,1,1,0,1,1 }
	},
	{//7
		{ 1,1,1,0,1,1,0,0,1 },{ 1,1,1,1,1,0,1,0,0 },{ 1,0,0,1,1,0,1,1,1 },{ 0,0,1,0,1,1,1,1,1 }
	},
	{//8
		{ 0,1,1,0,1,1,0,1,1 },{ 1,1,1,1,1,1,0,0,0 },{ 1,1,0,1,1,0,1,1,0 },{ 0,0,0,1,1,1,1,1,1 }
	},
	{//9
		{ 1,1,1,0,1,1,0,1,1 },{ 0,1,1,0,1,1,1,1,1 },{ 1,1,1,1,1,1,1,0,0 },{ 1,1,1,1,1,1,0,0,1 },
		{ 1,1,1,1,1,0,1,1,0 },{ 1,1,0,1,1,0,1,1,1 },{ 1,0,0,1,1,1,1,1,1 },{ 0,0,1,1,1,1,1,1,1 }
	},
	{//10
		{ 1,1,1,0,1,1,1,1,1 },{ 1,1,1,1,1,1,1,0,1 },{ 1,1,1,1,1,0,1,1,1 },{ 1,0,1,1,1,1,1,1,1 }
	},
	{//11
		{ 1,1,1,1,1,1,0,1,1 },{ 1,1,1,1,1,1,1,1,0 },{ 1,1,0,1,1,1,1,1,1 },{ 0,1,1,1,1,1,1,1,1 }
	}
};

vector<vector<vector<int>>> sUncPatterns = 
{
	{//1
		{0,0,1,0,1,0,0,0,0},{1,0,0,0,1,0,0,0,0}
	},
	{//2
		{0,0,0,0,1,0,0,1,0},{0,0,0,0,1,1,0,0,0}
	},
	{//3
		{ 0,0,1,0,1,1,0,0,0 },{ 0,1,1,0,1,0,0,0,0 },{ 1,1,0,0,1,0,0,0,0 },{ 1,0,0,1,1,0,0,0,0 },
		{ 0,0,0,1,1,0,1,0,0 },{ 0,0,0,0,1,0,1,1,0 },{ 0,0,0,0,1,0,0,1,1 },{ 0,0,0,0,1,1,0,0,1 }
	},
	{//4
		{0,1,1,0,1,0,1,0,0},{0,0,1,0,1,1,1,0,0},{1,0,0,1,1,0,0,0,1},{1,1,0,0,1,0,0,0,1},
		{0,0,1,1,1,0,1,0,0},{0,0,1,0,1,0,1,1,0},{1,0,0,0,1,0,0,1,1},{1,0,0,0,1,1,0,0,1}
	},
	{//5
		{0,1,1,1,1,0,0,0,0},{1,1,0,0,1,1,0,0,0},{0,1,0,0,1,1,0,0,1},{0,0,1,0,1,1,0,1,0}
	},

};

vector<vector<vector<int>>> kUncPatterns=
{
	{//1
		{ 0,0,1,0,1,0,0,0,0 },{ 1,0,0,0,1,0,0,0,0 },{0,0,0,0,1,0,0,0,1},{0,0,0,0,1,0,1,0,0}
	},
	{//2
		{ 0,0,0,0,1,0,0,1,0 },{ 0,0,0,0,1,1,0,0,0 },{0,0,0,1,1,0,0,0,0},{0,1,0,0,1,0,0,0,0}
	},
	{//3
		{0,0,0,0,0,0,0,0,0}
	},
	{//4
		{0,1,0,0,1,1,0,0,0},{0,1,0,1,1,0,0,0,0},{0,0,0,0,1,1,0,1,0},{0,0,0,1,1,0,0,1,0}
	}
};



int getBond(vector<int> data) {
	int bond = 0;
	for (int i = 0; i < 9; i++) {
		bond += (data[i]>0) * (i % 2 + 1);
	}
	return bond - 1;
}

bool fitCorCluK(vector<int> data) {
	if (data[0] > 0 && data[1] > 0 && data[3] > 0 && data[4] > 0) {
		return true;
	}
	if (data[4] > 0 && data[5] > 0 && data[7] > 0 && data[8] > 0) {
		return true;
	}
	return false;
}

bool fitCorClu(vector<int> data) {
	if (data[0] > 0 && data[1] > 0 && data[3] > 0 && data[4] > 0) {
		return true;
	}
	return false;
}

bool fitTeeBranK(vector<int> data) {
	if (data[3] > 0 && data[4] > 0 && data[5] > 0 ) {
		if (data[1] > 0) return true;
		if (data[7] > 0) return true;
	}
	else if (data[1] > 0 && data[4] > 0 && data[7] > 0) {
		if (data[3] > 0) return true;
		if (data[5] > 0) return true;
	}
	return false;
}

bool fitTeeBran(vector<int> data) {
	if (data[3] > 0  && data[4] > 0 && data[5] > 0) {
		if (data[1] > 0) {
			if (data[2] == 0 && data[7] == 0 && data[8] == 0) return true;
			if (data[0] == 0 && data[6] == 0 && data[7] == 0) return true;
		}
		else if (data[7] > 0) {
			if (data[0] == 0 && data[1] == 0 && data[6] == 0) return true;
			if (data[1] == 0 && data[2] == 0 && data[8] == 0) return true;
		}
	}
	else if (data[1] > 0 && data[4] > 0 && data[7] > 0) {
		if (data[3] > 0) {
			if (data[5] == 0 && data[6] == 0 && data[8] == 0) return true;
			if (data[0] == 0 && data[2] == 0 && data[5] == 0) return true;
		}
		else if (data[5] > 0) {
			if (data[0] == 0 && data[2] == 0 && data[3] == 0) return true;
			if (data[3] == 0 && data[6] == 0 && data[8] == 0) return true;
		}
	}
	return false;
}

bool fitVeeBran(vector<int> data) {
	if (data[0] > 0 && data[2] > 0 && data[4] > 0) {
		if ((data[6] || data[7] || data[8]) > 0) {
			return true;
		}
	}
	if (data[0] > 0 && data[4] > 0 && data[6] > 0) {
		if ((data[2] || data[5] || data[8]) > 0) {
			return true;
		}
	}
	if (data[4] > 0 && data[6] > 0 && data[8] > 0) {
		if ((data[0] || data[1] || data[2]) > 0) {
			return true;
		}
	}
	if (data[2] > 0 && data[4] > 0 && data[8] > 0) {
		if ((data[0] || data[3] || data[6]) > 0) {
			return true;
		}
	}
	return false;
}

bool fitDiaBran(vector<int> data) {
	if (data[1] > 0 && data[4] > 0 && data[5] > 0 && data[6] > 0 && data[2] == 0 && data[3] == 0 && data[7] == 0) return true;
	if (data[1] > 0 && data[3] > 0 && data[4] > 0 && data[8] > 0 && data[0] == 0 && data[5] == 0 && data[7] == 0) return true;
	if (data[2] > 0 && data[3] > 0 && data[4] > 0 && data[7] > 0 && data[1] == 0 && data[5] == 0 && data[6] == 0) return true;
	if (data[0] > 0 && data[4] > 0 && data[5] > 0 && data[7] > 0 && data[1] == 0 && data[3] == 0 && data[8] == 0) return true;
	return false;
}

bool fit(vector<int> data,vector<int> pattern) {
	for (int i = 0; i < 9; i++) {
		if ((data[i]/255) != pattern[i]) {
			return false;
		}
	}
	return true;
}

bool fitUnc(vector<int> data) {
	int bond = getBond(data);
	if (bond < 6 && bond >0) {
		vector<vector<int>> bondPatterns = sUncPatterns.at(bond - 1);
		for (int m = 0; m < bondPatterns.size(); m++) {
			if (fit(data, bondPatterns.at(m))) {
				return true;
			}
		}
	}
	if (fitCorClu(data)) return true;
	if (fitTeeBran(data)) return true;
	if (fitVeeBran(data)) return true;
	if (fitDiaBran(data)) return true;
	return false;
}

bool fitUncK(vector<int> data) {
	int bond = getBond(data);
	if (bond < 5 && bond >0) {
		vector<vector<int>> bondPatterns = kUncPatterns.at(bond - 1);
		for (int m = 0; m < bondPatterns.size(); m++) {
			if (fit(data, bondPatterns.at(m))) {
				return true;
			}
		}
	}
	if (fitCorCluK(data)) return true;
	if (fitTeeBranK(data)) return true;
	if (fitVeeBran(data)) return true;
	if (fitDiaBran(data)) return true;
	return false;
}

unsigned char * markImage(unsigned char * binaryImage, int width, int height) {
	int size = width * height;
	unsigned char* result = (unsigned char*)malloc(sizeof(unsigned char)* size);
	for (int i = 0; i < size; i++) {
		if (binaryImage[i] > 0) {
			//result[i] = binaryImage[i];
			result[i] = 0;
			vector<int> blockData;
			for (int j = -1; j < 2; j++) {
				for (int k = -1; k < 2; k++) {
					blockData.push_back(binaryImage[i + j*width + k]);
				}
			}
			int bond = getBond(blockData);
			if (bond < 11&& bond>0) {
				vector<vector<int>> bondPatterns = sPatterns.at(bond - 1);
				for (int m = 0; m < bondPatterns.size(); m++) {
					if (fit(blockData, bondPatterns.at(m))) {
						result[i] = 255;
						break;
					}
				}
			}
		}
		else {
			result[i] = 0;
		}
	}
	return result;
}

unsigned char * markUnc(unsigned char * markImage, int width, int height) {
	int size = width * height;
	unsigned char* result = (unsigned char*)malloc(sizeof(unsigned char)* size);
	for (int i = 0; i < size; i++) {
		if (markImage[i] > 0) {
			result[i] = markImage[i];
			vector<int> blockData;
			for (int j = -1; j < 2; j++) {
				for (int k = -1; k < 2; k++) {
					blockData.push_back(markImage[i + j*width + k]);
				}
			}
			if (fitUnc(blockData)) {
				result[i] = 0;
			}
		}
		else {
			result[i] = 0;
		}
	}
	return result;
}

unsigned char * andImage(unsigned char* image1, unsigned char* image2,int imageSize) {
	unsigned char* result = (unsigned char*)malloc(sizeof(unsigned char)*imageSize);
	for (int i = 0; i < imageSize; i++) {
		result[i] = (image1[i] && image2[i]) * 255;
	}
	return result;
}

unsigned char* complement(unsigned char* image,int imageSize) {
	unsigned char* result = (unsigned char*)malloc(sizeof(unsigned char)*imageSize);
	for (int i = 0; i < imageSize; i++) {
		result[i] = (1 - (image[i] > 0)) * 255;
	}
	return result;
}

unsigned char *shrink(unsigned char* binaryImage, int width, int height) {
	int size = width*height;

	// conditional get mark
	unsigned char* marks = markImage(binaryImage, width, height);
	// unconditional for mark image
	unsigned char* uncMark = markUnc(marks, width, height);
	unsigned char* comUncMark = complement(uncMark, size);

	// shrink operation
	unsigned char* result = andImage(binaryImage, comUncMark, size);

	return result;
}

bool identical(unsigned char* image1, unsigned char *image2, int size) {
	for (int i = 0; i < size; i++) {
		if (image1[i] != image2[i]) {
			return false;
		}
	}
	return true;
}

int count(unsigned char* image, int size) {
	int number = 0;
	for (int i = 0; i < size; i++) {
		if (image[i] > 0) {
			number++;
		}
	}
	return number;
}

unsigned char * markImageT(unsigned char * binaryImage, int width, int height) {
	int size = width * height;
	unsigned char* result = (unsigned char*)malloc(sizeof(unsigned char)* size);
	for (int i = 0; i < size; i++) {
		if (binaryImage[i] > 0) {
			//result[i] = binaryImage[i];
			result[i] = 0;
			vector<int> blockData;
			for (int j = -1; j < 2; j++) {
				for (int k = -1; k < 2; k++) {
					blockData.push_back(binaryImage[i + j*width + k]);
				}
			}
			int bond = getBond(blockData);
			if (bond < 11 && bond>3) {
				vector<vector<int>> bondPatterns = tPatterns.at(bond - 4);
				for (int m = 0; m < bondPatterns.size(); m++) {
					if (fit(blockData, bondPatterns.at(m))) {
						result[i] = 255;
						break;
					}
				}
			}
		}
		else {
			result[i] = 0;
		}
	}
	return result;
}

unsigned char *thin(unsigned char* binaryImage, int width, int height) {
	int size = width*height;

	// conditional get mark
	unsigned char* marks = markImageT(binaryImage, width, height);
	// unconditional for mark image
	unsigned char* uncMark = markUnc(marks, width, height);
	unsigned char* comUncMark = complement(uncMark, size);

	// shrink operation
	unsigned char* result = andImage(binaryImage, comUncMark, size);

	return result;
}

unsigned char * markImageK(unsigned char* binaryImage, int width, int height) {
	int size = width * height;
	unsigned char* result = (unsigned char*)malloc(sizeof(unsigned char)* size);
	for (int i = 0; i < size; i++) {
		if (binaryImage[i] > 0) {
			//result[i] = binaryImage[i];
			result[i] = 0;
			vector<int> blockData;
			for (int j = -1; j < 2; j++) {
				for (int k = -1; k < 2; k++) {
					blockData.push_back(binaryImage[i + j*width + k]);
				}
			}
			int bond = getBond(blockData);
			if (bond <= 11 && bond>3) {
				vector<vector<int>> bondPatterns = kPatterns.at(bond - 4);
				for (int m = 0; m < bondPatterns.size(); m++) {
					if (fit(blockData, bondPatterns.at(m))) {
						result[i] = 255;
						break;
					}
				}
			}
		}
		else {
			result[i] = 0;
		}
	}
	return result;
}

unsigned char* markUncK(unsigned char * markImage, int width, int height) {
	int size = width * height;
	unsigned char* result = (unsigned char*)malloc(sizeof(unsigned char)* size);
	for (int i = 0; i < size; i++) {
		if (markImage[i] > 0) {
			result[i] = markImage[i];
			vector<int> blockData;
			for (int j = -1; j < 2; j++) {
				for (int k = -1; k < 2; k++) {
					blockData.push_back(markImage[i + j*width + k]);
				}
			}
			if (fitUncK(blockData)) {
				result[i] = 0;
			}
		}
		else {
			result[i] = 0;
		}
	}
	return result;
}

unsigned char *skeleton(unsigned char* binaryImage, int width, int height) {
	int size = width*height;
	// conditional get mark
	unsigned char* marks = markImageK(binaryImage, width, height);
	// unconditional for mark image
	unsigned char* uncMark = markUncK(marks, width, height);
	unsigned char* comUncMark = complement(uncMark, size);
	// shrink operation
	unsigned char* result = andImage(binaryImage, comUncMark, size);

	return result;
}

int numberSize1(unsigned char* image, int width, int height) {
	int count = 0;
	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width-1; j++) {
			if (image[i*width + j] > 0) {
				bool flag = true;
				for (int m = -1; m < 2; m++) {
					for (int n = -1; n < 2; n++) {
						if (image[(i + m)*width + j + n] > 0 && ((abs(m) + abs(n)) !=0)) {
							flag = false;
							break;
						}
					}
					if (!flag) break;
				}
				if (flag) {
					count++;
				}
			}
		}
	}
	return count;
}

int main() {

	//============================shrink=============================//
	//gather information
	Image image = Image(640, 480, 1, "stars.raw");
	unsigned char * data = image.imageData();
	int size = image.getFrameSize();
	int width = image.getWidth();
	int height = image.getHeight();

	//change the image into {0,1} image using fixed threshold
	unsigned char * binaryImage = Dithering::fixThre(data, image.getWidth(), image.getHeight(), 1, 127);
	Image::writeFile(binaryImage, image.getWidth(), image.getHeight(), 1, "binary.raw");
	unsigned char* temp;
	int iter = 0;
	cout << "star size: cdf" << endl;

	vector<int> cdf;
	int number = numberSize1(binaryImage, width, height);
	cout << "iter:" << iter << "    one size star number:" << number << endl;
	cdf.push_back(number);
	while (1) {
		string filename = "iter" + to_string(iter) + ".raw";
		temp = shrink(binaryImage, width, height);
		iter++;
		if (identical(binaryImage, temp,size)) {
			break;
		}
		number = numberSize1(temp, width, height);
		cdf.push_back(number);
		cout << "iter:" << iter << "    one size star number:"<<number << endl;
		Image::writeFile(temp, width, height, 1, filename.c_str());
		binaryImage = temp;
	}
	cout << "star size: pdf" << endl;
	for (int i = 1; i < cdf.size(); i++) {
		cout << "size " << i << "  star number: "<<cdf[i] - cdf[i - 1] << endl;
	}

	cout << "complete" << endl;
	cout << "star number:" << count(temp,size) << endl;
	Image::writeFile(temp, width, height, 1, "shrinkResult.raw");


	//===================================thining=================================//
	////gather information
	//Image image = Image(100, 100, 1, "jigsaw_1.raw");
	//unsigned char * data = image.imageData();
	//int size = image.getFrameSize();
	//int width = image.getWidth();
	//int height = image.getHeight();

	////preprocessing image 
	//unsigned char* invData = Image::inverse(data, width, height, 1);
	//unsigned char* binaryImage = Dithering::fixThre(invData, width, height, 1, 127);
	//unsigned char* temp;
	//while (1) {
	//	temp = thin(binaryImage, width, height);
	//	if (identical(binaryImage, temp,size)) {
	//		break;
	//	}
	//	binaryImage = temp;
	//}
	//Image::writeFile(temp, width, height, 1, "thin.raw");
	//cout << "complete" << endl;

	//===================================skeleton=================================//
	//gather information
	//Image image = Image(100, 100, 1, "jigsaw_2.raw");
	//unsigned char * data = image.imageData();
	//int size = image.getFrameSize();
	//int width = image.getWidth();
	//int height = image.getHeight();

	////preprocessing image 
	//unsigned char* invData = Image::inverse(data, width, height, 1);
	//unsigned char* binaryImage = Dithering::fixThre(invData, width, height, 1, 127);
	//Image::writeFile(binaryImage, width, height, 1,"binaryImage.raw");
	//unsigned char* temp;
	//while (1) {
	//	temp = skeleton(binaryImage, width, height);
	//	Image::writeFile(temp, width, height, 1, "skeleton.raw");
	//	if (identical(binaryImage, temp, size)) {
	//		break;
	//	}
	//	binaryImage = temp;
	//}
	//Image::writeFile(temp, width, height, 1, "skeleton.raw");
	//cout << "complete" << endl;



	getchar();
	return 0;

}