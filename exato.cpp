#include <stdio.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <climits>
#include "Hungarian.cpp"

using namespace std;

int n;
vector< vector<int> > flowMatrix;
vector< vector<int> > distanceMatrix;

void readInstance(char* fileName){
	ifstream fileData;
	fileData.open(fileName);
	if(fileData.is_open()){
		while(!fileData.eof()){
			fileData >> n;
			
			for(int i = 0; i < n; i++){
				vector<int> line;
				for(int j = 0; j < n; j++){
					int x;
					fileData >> x;
					line.push_back(x);
				}
				
				flowMatrix.push_back(line);
			}
			
			for(int i = 0; i < n; i++){
				vector<int> line;
				for(int j = 0; j < n; j++){
					int x;
					fileData >> x;
					line.push_back(x);
				}
				
				distanceMatrix.push_back(line);
			}
		}
	}
}

void printInstance(){
	printf("Size: %i\n", n);
	printf("________________________________________________\n");
	printf("\n");

	printf("Flow Matrix:\n");
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			printf("%i ", flowMatrix[i][j]);
		}
		printf("\n");
	}
	printf("________________________________________________\n");
	printf("\n");

	printf("Distance Matrix:\n");
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			printf("%i ", distanceMatrix[i][j]);
		}
		printf("\n");
	}
}

int partition(vector<int> &v, int low, int high){
	int pivot = v[high];
 
	int i = (low-1);

	for (int j = low; j <= high- 1; j++){
		if (v[j] <= pivot){
			i++;
			int aux = v[i];
			v[i] = v[j];
			v[j] = aux;
		}
	}
	int aux = v[i+1];
	v[i+1] = v[high];
	v[high] = aux;
	return (i+1);
}

void quickSort(vector<int> &v, int low, int high){
	if (low < high){
		int pi = partition(v, low, high);

		quickSort(v, low, pi - 1);
		quickSort(v, pi + 1, high);
	}
}

int gilmore_lawlerBound(vector< vector<int> > a, vector< vector<int> > b){
	for(int i = 0; i < a.size(); i++){
		quickSort(a[i], 0, a[i].size()-1);
		quickSort(b[i], 0, b[i].size()-1);
		reverse(b[i].begin(), b[i].end());
	}

	vector< vector<double> > g;
	for(int i = 0; i < a.size(); i++){
		vector<int> a_i = a[i];
		vector<double> line;
		for(int j = 0; j < b.size(); j++){
			vector<int> b_i = b[j];
			int scalar = 0;
			for(int k = 0; k < b_i.size(); k++){
				scalar += a_i[k] * b_i[k];
			}
			line.push_back(scalar);
		}
		g.push_back(line);
	}

	// https://github.com/mcximing/hungarian-algorithm-cpp
	HungarianAlgorithm HungAlgo;
	vector<int> assignment;
	int cost = HungAlgo.Solve(g, assignment);
	printf("%i\n", cost);
}

int main(int argc, char* argv[]){
	readInstance(argv[1]);
//	printInstance();
	vector< vector<int> > a{{25, 13, 28}, {28, 15, 4}, {23, 15, 25}, {4, 13, 23}};
	vector< vector<int> > b{{2, 6, 7}, {6, 6, 5}, {7, 5, 1}, {6, 2, 1}};

	 gilmore_lawlerBound(a, b);
	return 0;
}
