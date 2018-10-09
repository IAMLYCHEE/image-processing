#pragma once
#ifndef LAWFILTERS_H_
#define LAWFILTERS_H_
#include<iostream>
#include<vector>
using namespace std;
typedef vector<vector<double>>  vector2d;
class LawFilters {
public:
	LawFilters();
	vector<vector2d> filterBank5X5;
	vector<vector2d> filterBank3X3;
	vector2d getFilter5X5(int type);
	vector2d getFilter3X3(int type);
};

#endif // !LAWFILTERS_H_
