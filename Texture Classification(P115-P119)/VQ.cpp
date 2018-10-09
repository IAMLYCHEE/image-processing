#include "VQ.h"

VQ::VQ(vector2d data, int K)
{
		this->data = data;
		this->D = data.at(0).size();
		this->K = K;
		this->N = data.size();
		this->label = vector<int>(N, 0);
}

vector2d regenerate(vector2d data, int Nrequired, int D, int N) {
	vector2d newDataSet;
	for (int i = 0; i < Nrequired; i++) {
		int index1 = rand() % N;
		int index2 = rand() % N;
		int position = rand() % D;
		vector<double> v1 = data.at(index1);
		vector<double> v2 = data.at(index2);
		vector<double> newdata;
		for (int j = 0; j < position; j++) {
			newdata.push_back(v1[j]);
		}
		for (int j = position; j < D; j++) {
			newdata.push_back(v2[j]);
		}
		newDataSet.push_back(newdata);
	}
	return newDataSet;
}

vector<double> genMean(vector2d data,int D) {
	vector<double> mu;
	for (int i = 0; i < D; i++) {
		double sum = 0.0;
		for (int j = 0; j < data.size(); j++) {
			sum += data[j][i];
		}
		mu.push_back(sum / data.size());
	}
	return mu;
}

vector2d initWithRand(vector2d data,int N,int D) {
	vector2d centroids;
	for (int j = 0; j < 4; j++) {	
		vector2d tempCluster;
		for (int i = 0; i < N / 10; i++) {
			tempCluster.push_back(data.at(rand() % N));
		}
		vector<double> centroid = genMean(tempCluster,D);
		centroids.push_back(centroid);
	}
	return centroids;
}

vector2d initWithGA(vector2d data,int D, int N) {
	//regenerate samples with GA
	vector2d newData = regenerate(data, 50, D, N);
	vector2d centroids = initWithRand(newData, 50, D);
	return centroids;
}



vector2d initWithMean(vector2d data,int N, int D, int K) {
	vector<double> mu;
	vector2d centroids(K, vector<double>(D, 0.0));
	for (int i = 0; i < D; i++) {
		double sum = 0.0;
		for (int j = 0; j < N; j++) {
			sum += data[j][i];
		}
		mu.push_back(sum / N);
	}
	for (int i = 0; i < K; i++) {
		for (int j = 0; j < D; j++) {
			centroids[i][j] = (i + 0.5) * 2 * mu[j] / K;
		}
	}
	return centroids;
}

double calDistance(vector<double> v1, vector<double> v2) {
	double distance = 0.0;
	for (int i = 0; i < v1.size(); i++) {
		distance += pow(v1[i] - v2[i], 2);
	}
	return distance;
}

vector<double> genCentroid(vector2d drawer, int D) {
	vector<double> centroid = vector<double>(D, 0);
	int fileAmount = drawer.size();
	if (fileAmount > 0) {
		for (int j = 0; j < D; j++) {
			double sum = 0.0;
			for (int i = 0; i < fileAmount; i++) {
				sum += drawer[i][j];
			}
			centroid[j] = (sum / fileAmount);
		}
	}
	return centroid;
}

vector2d genCentroids(vector<vector2d> drawers, int K, int D) {
	vector2d centroids;
	for (int i = 0; i < K; i++) {
		vector2d drawer = drawers.at(i);
		vector<double> centroid = genCentroid(drawer, D);
		centroids.push_back(centroid);
	}
	return centroids;
}

bool identical(vector2d v1, vector2d v2,int K,int D) {
	for (int i = 0; i < K; i++) {
		auto e1 = v1.at(i);
		auto e2 = v2.at(i);
		for (int j = 0; j < D; j++) {
			double ee1 = e1[j];
			double ee2 = e2[j];
			if (ee1 != ee2) return false;
		}
	}
	return true;
}

void description(vector2d data) {
	for (auto e : data) {
		for (auto ee : e) {
			cout << ee << " ";
		}
		cout << endl;
	}
}

vector<int> VQ::classify(int initType)
{
	vector2d centroids;
	if (initType == 1) {
		centroids = initWithMean(data, N, D, K);
	}
	else if (initType == 2) {
		centroids = initWithRand(data,N,D);
	}
	else {
		centroids = initWithGA(data, D, N);
	}
	vector<int> labels(N, 0);
	int trainingCount = 1;
	cout << "initial Centroids:" << endl;
	description(centroids);
	while (1) {
		vector<vector2d> drawers(K, vector2d{});
		for (int i = 0; i < N; i++) {
			double minDistance = calDistance(data.at(i), centroids.at(0));
			int label = 0;
			for (int j = 1; j < K; j++) {
				double distance = calDistance(data.at(i), centroids.at(j));
				if (distance < minDistance) {
					minDistance = distance;
					label = j;
				}
			}
			drawers.at(label).push_back(data.at(i));
			labels[i] = label;
		}
		vector2d newCentroids = genCentroids(drawers, K, D);
		if (identical(newCentroids, centroids,K,D)) {
			break;
		}
		else {
			cout << "training:" << trainingCount << endl;
			description(newCentroids);
			trainingCount++;
			centroids = newCentroids;
		}
	}
	this->label = labels;
	return labels;
}

void VQ::dataDescription()
{
	cout << "data size:	" << N << endl;
	cout << "data dimension: " << D << endl;
	cout << "cluster into " << K << " sets" << endl;
}
