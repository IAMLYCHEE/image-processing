#include "Utility.h"

int Utility::saveData(vector2d data, string filename)
{
	ofstream outfile;
	outfile.open(filename, std::ios::out);
	if (!outfile.is_open()) {
		cout << "can't open file" << endl;
		return -1;
	}
	else {
		for (auto e : data) {
			for (auto ee : e) {
				outfile << std::fixed << std::setprecision(sizeof(ee)) << ee << " ";
			}
			outfile << endl;
		}
		outfile.close();
	}
	return 0;
}

int Utility::saveData(vector<double> data, string filename)
{
	ofstream outfile;
	outfile.open(filename, std::ios::out);
	if (!outfile.is_open()) {
		cout << "can't open file" << endl;
		return -1;
	}
	else {
		for (auto e : data) {
				outfile << std::fixed << std::setprecision(sizeof(e)) << e << endl;
		}
		outfile.close();
	}
	return 0;
}
