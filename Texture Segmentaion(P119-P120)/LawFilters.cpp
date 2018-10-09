#include"LawFilters.h"
using namespace std;

typedef vector<vector<double>>  vector2d;
vector2d filters1d5 =
{
	{ -1.0 / 6.0, -1.0 / 3.0, 0.0, 1.0 / 3.0, 1.0 / 6.0 },
	{ -1.0 / 4.0, 0.0, 1.0 / 2.0, 0.0, -1.0 / 4.0 },
	{ -1.0 / 6.0, 1.0 / 3.0, 0.0, -1.0 / 3.0, 1.0 / 6.0 }
};

vector2d filters1d3 = 
{
	{1.0/6.0, 1.0/3.0,1.0/6.0},
	{1.0/2.0, 0.0, -1.0/2.0},
	{1.0/2.0, -1.0 ,1.0/2.0}
};

vector<vector<double>> genFilterMatrix(vector<double> v1, vector<double> v2) {
	vector<vector<double>> matrix;
	for (int i = 0; i < v1.size(); i++) {
		vector<double> row;
		for (int j = 0; j < v2.size(); j++) {
			row.push_back(v1[i] * v2[j]);
		}
		matrix.push_back(row);
	}
	return matrix;
}


LawFilters::LawFilters() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			this->filterBank5X5.push_back(genFilterMatrix(filters1d5.at(i), filters1d5.at(j)));
			this->filterBank3X3.push_back(genFilterMatrix(filters1d3.at(i), filters1d3.at(j)));
		}
	}
}

vector2d LawFilters::getFilter5X5(int type) {
	//0: e5e5 1:e5s5  2:e5w5
	//3: s5e5 4:s5s5  5:s5w5
	//6: w5e5 7:w5s5  8:w5w5
	return filterBank5X5.at(type);
}

vector2d LawFilters::getFilter3X3(int type) {
	//0: e5e5 1:e5s5  2:e5w5
	//3: s5e5 4:s5s5  5:s5w5
	//6: w5e5 7:w5s5  8:w5w5
	return filterBank3X3.at(type);
}
