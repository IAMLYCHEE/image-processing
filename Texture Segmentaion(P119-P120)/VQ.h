#pragma once
#ifndef VQ_H_
#define VQ_H_
#include<iostream>
#include<vector>
#include<stdlib.h>

using namespace std;
typedef vector<vector<double>> vector2d;

class VQ {
private:
	int N;
	int D;
	int K;
	vector2d data;
	vector<int> label;
public:
	VQ(vector2d data,int K);
	vector<int> classify(int initType);
	void dataDescription();
	int getK() { return K; }
	int getN() { return N; }
	int getD() { return D; }
};

#endif // !VQ_H_
