#include<iostream>
#include"Image.h"
#include"Dithering.h"
#include<vector>
#include<set>
#include<algorithm>
#include<string>
#include"Jigsaw.h"


using namespace std;

unsigned char* flip(unsigned char* origin, int width, int height) {
	unsigned char* result = (unsigned char*)malloc(sizeof(unsigned char)* width *height);
	for (int j = 0; j < height; j++) {
		for (int k = 0; k < width; k++) {
			result[(height - 1 - j)*width + k] = origin[j*width + k];
		}
	}
	return result;
}


unsigned char* rotate90(unsigned char* origin, int width, int height) {
	unsigned char* result = (unsigned char*)malloc(sizeof(unsigned char) * width*height);
	for (int j = 0; j < height; j++) {
		for (int k = 0; k < width; k++) {
			result[k*height + (height-1 - j)] = origin[j*width + k];
		}
	}
	return result;
}

bool identical(unsigned char* image, unsigned char* pattern,int size) {
	int error = 0;
	for (int i = 0; i < size; i++) {
		if (image[i] != pattern[i]) {
			error++;
		}
	}
	double rate = (double)(size - error) / (double)size;
	if( rate > 0.98) return true;
	return false;
}

int main() {
	//gather information
	Image image = Image(372, 372, 1, "board.raw");
	int width = image.getWidth();
	int height = image.getHeight();
	int size = image.getFrameSize();
	unsigned char* data = image.imageData();

	//preprocess image
	//inverse color
	data = Image::inverse(data, width, height, 1);
	//dithering
	data = Dithering::fixThre(data, width, height, 1, 127);

	//component ananlysis
	//create an label Image and initiate it to all zeros
	unsigned char* labelImage = (unsigned char*)malloc(sizeof(unsigned char)*size);
	for (int i = 0; i < size; i++) {
		labelImage[i] = 0;
	}
	//record current minLabel
	int minLabel = 3;
	//get the label match table
	vector<set<int>> matchTable(100,set<int>());

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int currPosition = i*width + j;
			if (data[currPosition] > 0) {
				//gather label
				vector<int> labels;
				for (int k = -1; k < 2; k++) {
					int position = currPosition - width + k;
					if (labelImage[position] > 0) {
						labels.push_back(labelImage[position]);
					}
					if (labelImage[currPosition - 1] > 0) {
						labels.push_back(labelImage[currPosition - 1]);
					}
				}
				if (!labels.empty()) {
					sort(labels.begin(), labels.end());
					labelImage[currPosition] = labels[0];
				}
				else {
					labelImage[currPosition] = minLabel;
					minLabel++;
				}
				// add information to match table
				if (!labels.empty()) {
					for (int h = 0; h < labels.size(); h++) {
						if (labels[h] != labels[0]) {
							matchTable[labels[0]].insert(labels[h]);
						}
					}
				}//end match table
			}//end current analysis
		}
	}
	cout << "component analysis finished" << endl;
	//second pass
	while (1) {	
		cout << "processing...." << endl;
		int flag = 1;
		for (int i = 0; i < size; i++) {
			if (labelImage[i] > 0) {
				int label = labelImage[i];
				for (int j = 3; j < label; j++) {
					set<int> table = matchTable[j];
					if (!table.empty()) {
						if (table.find(label) != table.end()) {
							labelImage[i] = j;
							flag = 0;
							break;
						}
					}
				}
			}

		}
		cout << endl;
		if (flag == 1) break;

	}


	//test
	//get final labels
    set<int> labelSet;
	for (int i = 0; i < size; i++) {
		if (labelImage[i] > 0) {
			labelSet.insert(labelImage[i]);
		}
	}
	cout << "total number of jigsaws:" << labelSet.size() << endl;
	cout << "start the segmentation job" << endl;
	cout << "different jigsaws are stored in the following file with dimensions:" << endl;
	int count = 1;
	vector<Jigsaw> jigsaws;
	for (std::set<int>::iterator it = labelSet.begin(); it != labelSet.end(); ++it) {
		
		int label = *it;
		//initiate minx,miny,maxx,maxy
		int minJ=0;
		int maxJ=0;
		int minK=0;
		int maxK=0;
		for (int j = 0; j < height; j++) {
			int flag = 0;
			for (int k = 0; k < width; k++) {
				if (labelImage[j*width + k] == label) {
					minJ = j;
					maxJ = j;
					minK = k;
					maxK = k;
					flag = 1;
					break;
				}
			}
			if (flag) break;
		}

		for (int j = 0; j < height; j++) {
			for (int k = 0; k < width; k++) {
				if (labelImage[j*width + k] == label) {
					if (j < minJ) minJ = j;
					if (j > maxJ) maxJ = j;
					if (k < minK) minK = k;
					if (k > maxK) maxK = k;
				}
			}
		}

		int jigsawWidth = maxK - minK + 1;
		int jigsawHeight = maxJ - minJ + 1;
		unsigned char * jigsaw = (unsigned char*)malloc(sizeof(unsigned char*)*(jigsawWidth * jigsawHeight));
		for (int j = minJ; j <= maxJ; j++) {
			for (int k = minK; k <= maxK; k++) {
				jigsaw[(j-minJ)*jigsawWidth + (k-minK)]=data[j* width + k];
			}
		}

		Jigsaw jigsawObject= Jigsaw(jigsawWidth, jigsawHeight, jigsaw, count);
		jigsaws.push_back(jigsawObject);
		//test
		jigsaw = Image::inverse(jigsaw, jigsawWidth, jigsawHeight, 1);
		string filename = "jigsaw" + to_string(count) + ".raw";
		cout << filename << ":    " << "Width:" << jigsawWidth  << "    Height:" << jigsawHeight << endl;
		Image::writeFile(jigsaw,jigsawWidth, jigsawHeight, 1, filename.c_str());

		count++;

	}
	cout << "complete gathering jigsaws" << endl;
	cout << "start find unique" << endl;

	//test rotate 90
	//Jigsaw pattern = jigsaws.at(0);
	//unsigned char * jigrotate90 = rotate90(pattern.getdata(), pattern.getWidth(), pattern.getHeight());
	//Image::writeFile(jigrotate90, pattern.getHeight(), pattern.getWidth(), 1, "rotatetest.raw");
	for (int i = 0; i < jigsaws.size(); i++) {
		//gather the pattern information
		Jigsaw jigsawPattern = jigsaws.at(i);
		int patternWidth = jigsawPattern.getWidth();
		int patternHeight = jigsawPattern.getHeight();
		for (int j = i + 1; j < jigsaws.size(); j++) {
			Jigsaw oneJigsaw = jigsaws.at(j);
			int jigsawWidth = oneJigsaw.getWidth();
			int jigsawHeight = oneJigsaw.getHeight();
			if (jigsawWidth == patternWidth && jigsawHeight == patternHeight) {
				if (identical(oneJigsaw.getdata(), jigsawPattern.getdata(), patternWidth*patternHeight)) {//0 no flip
					cout << "find identical:   "<< "jigsaw" << jigsawPattern.getLabel()<<"   and   " << "jigsaw" << oneJigsaw.getLabel() <<"    the same"<< endl;
					jigsaws.erase(jigsaws.begin() + j);
					j--;
				}
				else {
					unsigned char * jigRotate90 = rotate90(jigsawPattern.getdata(), patternWidth, patternHeight);
					unsigned char * jigRotate180 = rotate90(jigRotate90, patternHeight, patternWidth);
					unsigned char* jigflip = flip(jigsawPattern.getdata(), patternWidth, patternHeight);
					unsigned char* jig180flip = flip(jigflip, patternWidth, patternHeight);
					if (identical(oneJigsaw.getdata(), jigRotate180, patternWidth*patternHeight)) {//180 no flip
						cout << "find identical:   " << "jigsaw" << jigsawPattern.getLabel() << "  and  " << "jigsaw" << oneJigsaw.getLabel() << "    rotate 180 degrees" << endl;
						jigsaws.erase(jigsaws.begin() + j);
						j--;
					}
					else if (identical(oneJigsaw.getdata(), jigflip, patternWidth*patternHeight)) {//0 flip
						cout << "find identical:   " << "jigsaw" << jigsawPattern.getLabel() << "  and  " << "jigsaw" << oneJigsaw.getLabel() << "    flip" << endl;
						jigsaws.erase(jigsaws.begin() + j);
						j--;
					}else if (identical(oneJigsaw.getdata(), jig180flip, patternWidth*patternHeight)) {//180 flip
						cout << "find identical:   " << "jigsaw" << jigsawPattern.getLabel() << "  and  " << "jigsaw" << oneJigsaw.getLabel() << "    rotate 180 degrees then flip" << endl;
						jigsaws.erase(jigsaws.begin() + j);
						j--;
					}
				}
			}
			else if (jigsawWidth == patternHeight && jigsawHeight == patternWidth) {
				unsigned char * jigRotate90 = rotate90(jigsawPattern.getdata(), patternWidth, patternHeight);
				unsigned char * jigRotate180 = rotate90(jigRotate90, patternHeight, patternWidth);
				unsigned char * jigRotate270 = rotate90(jigRotate180, patternWidth, patternHeight);
				unsigned char* jig90flip = flip(jigRotate90, patternHeight, patternWidth);
				unsigned char* jig270flip = flip(jigRotate270, patternHeight, patternWidth);
				if (identical(oneJigsaw.getdata(), jigRotate90, patternHeight*patternWidth)) {//90 no flip
					cout << "find identical:   " << "jigsaw" << jigsawPattern.getLabel() << "  and  " << "jigsaw" << oneJigsaw.getLabel() << "    rotate 90 degrees" << endl;
					jigsaws.erase(jigsaws.begin() + j);
					j--;
				}
				else if (identical(oneJigsaw.getdata(), jigRotate270, patternHeight*patternWidth)) {//270 no flip
					cout << "find identical:   " << "jigsaw" << jigsawPattern.getLabel() << "  and  " << "jigsaw" << oneJigsaw.getLabel() << "    rotate 270 degrees" << endl;
					jigsaws.erase(jigsaws.begin() + j);
					j--;
				}
				else if (identical(oneJigsaw.getdata(), jig90flip, patternHeight*patternWidth)) {//90 flip
					cout << "find identical:   " << "jigsaw" << jigsawPattern.getLabel() << "  and  " << "jigsaw" << oneJigsaw.getLabel() << "    rotate 90 degrees then flip" <<endl;
					jigsaws.erase(jigsaws.begin() + j);
					j--;
				}
				else if (identical(oneJigsaw.getdata(), jig270flip, patternHeight*patternWidth)) {//270 flip
					cout << "find identical:   " << "jigsaw" << jigsawPattern.getLabel() << "  and  " << "jigsaw" << oneJigsaw.getLabel() << "    rotate 270 degrees then flip"<<endl;
					jigsaws.erase(jigsaws.begin() + j);
					j--;
				}
			}
		}
	}
	cout << "number of unique:" << jigsaws.size() << endl;
	getchar();
	return 0;
}