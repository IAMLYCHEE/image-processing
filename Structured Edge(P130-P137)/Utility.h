#pragma once

#ifndef UTILITY_H_
#define UTILITY_H_

#include<iostream>
#include<vector>
#include<fstream>
#include<iomanip>
using namespace std;

typedef vector<vector<double>> vector2d;

class Utility {
public:
	static int saveData(vector2d data,  string filename);
	static int saveData(vector<double> data, string filename);
};

#endif

