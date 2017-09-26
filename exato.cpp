#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <climits>
#include "Hungarian.cpp"

using namespace std;

int n;
int maxCost;
vector<int> bestSolution;
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

	// for(int i = 0; i < n; i++){
	// 	for(int j = 0; j < n; j++){
	// 		printf("%i ", (int)g[i].size());
	// 	}
	// 	printf("\n");
	// }

	//printf("%i\n", g.size());

	int cost = HungAlgo.Solve(g, assignment);
	// printf("%i\n", cost);
	return cost;
}

int getCost(vector<int> solution){
	int cost = 0;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			cost += flowMatrix[i][j] * distanceMatrix[solution[i]-1][solution[j]-1];
		}
	}

	return cost;
}

int getBestSolution(vector<int> &solution){
	vector<int> swapSolution;
	int bestCost = getCost(solution);

	for(int i = 0; i < n; i++){
		swapSolution.push_back(solution[i]);
	}

	for(int i = 0; i < n; i++){
		for(int j = i+1; j < n; j++){
			int aux = swapSolution[i];
			swapSolution[i] = swapSolution[j];
			swapSolution[j] = aux;

			int swapSolutionCost = getCost(swapSolution);
			if(bestCost > swapSolutionCost){
				for(int i = 0; i < n; i++){
					solution[i] = swapSolution[i];
				}
				bestCost = swapSolutionCost;
			}else{
				aux = swapSolution[i];
				swapSolution[i] = swapSolution[j];
				swapSolution[j] = aux;
			}
		}
	}
	//printf("%i\n", bestCost);
	return bestCost;
}

void randomSolution(vector<int> &solution){
	
	for(int i = 1; i <= n; i++){
		solution.push_back(i);
	}

	for(int i = 0; i < n; i++){
		int randomIndex = i + rand()%(n-i);
		int aux = solution[i];
		solution[i] = solution[randomIndex];
		solution[randomIndex] = aux;
	}
}

void upperBound(){
	vector<int> solution;

	randomSolution(solution);
	//printf("%i\n", cost);
	// for(int i = 0; i < n; i++){
	// 	printf("%i ", solution[i]);
	// }
	// printf("\n");

	maxCost = getBestSolution(solution);

	for(int i = 0; i < n; i++){
		bestSolution.push_back(solution[i]);
	//	printf("%i ", solution[i]);
	}
	// printf("\n");

}

void branchAndBound(vector< vector<int> > fMatrix, vector< vector<int> > dMatrix, vector<int> solution, int minCost, int facility){
	for(int i = 0; i < n; i++){
		
		if(solution[i] != 0){
			continue;
		}

		solution[i] = facility;

		vector< vector<int> > fM;
		vector< vector<int> > dM;

		for(int j = 0; j < n; j++){
			vector<int> fV;
			vector<int> dV;
			for(int k = 0; k < n; k++){
				if(j != facility && k != facility){
					fV.push_back(fMatrix[j][k]);
				}else{
					fV.push_back(0);
				}
				if(j != i && k != i){
					dV.push_back(dMatrix[i][j]);
				}else{
					dV.push_back(0);
				}
			}

			if(fV.size() != 0){
				fM.push_back(fV);
			}
			if(dV.size() != 0){
				dM.push_back(dV);
			}
		}

		int cost = gilmore_lawlerBound(fM, dM) + minCost;
		printf("Min: %i Max: %i\n", cost, maxCost);
		
		if(facility == n){
			printf("AQUI\n");
			// int newMaxCost = getCost(solution);
			// if(newMaxCost < maxCost){
			// 	maxCost = newMaxCost;
			// }
		}else{
			if(cost < maxCost){
				branchAndBound(fM, dM, solution, cost,facility++);
			}
			solution[i] = 0;
		}
	}
}

int main(int argc, char* argv[]){
	readInstance(argv[1]);
	srand(time(NULL));

	vector<int> solution;
	for(int i = 0; i < n; i++){
		solution.push_back(0);
	}

	upperBound();
	branchAndBound(flowMatrix, distanceMatrix, solution, 0, 1);

	//printInstance();
	// vector< vector<int> > a{{25, 13, 28}, {28, 15, 4}, {23, 15, 25}, {4, 13, 23}};
	// vector< vector<int> > b{{2, 6, 7}, {6, 6, 5}, {7, 5, 1}, {6, 2, 1}};

	// gilmore_lawlerBound(a, b);
	return 0;
}