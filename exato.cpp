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

				distanceMatrix.push_back(line);
			}
			
			for(int i = 0; i < n; i++){
				vector<int> line;
				for(int j = 0; j < n; j++){
					int x;
					fileData >> x;
					line.push_back(x);
				}

				flowMatrix.push_back(line);
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

	int stack[high - low + 1];

	int top = -1;

	stack[++top] = low;
	stack[++top] = high;
 
	while(top >= 0){
		high = stack[top--];
		low = stack[top--];

		int p = partition(v, low, high);
 
		if(p-1 > low){
			stack[++top] = low;
			stack[++top] = p - 1;
		}
 
		if(p+1 < high){
			stack[++top] = p + 1;
			stack[++top] = high;
		}
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

	return cost;
}

int getCost(vector<int> solution){
	int cost = 0;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			cost += flowMatrix[solution[i]-1][solution[j]-1] * distanceMatrix[i][j];
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

	maxCost = getBestSolution(solution);

	for(int i = 0; i < n; i++){
		bestSolution.push_back(solution[i]);
	}
}

void branchAndBound(vector< vector<int> > fMatrix, vector< vector<int> > dMatrix, vector<int> solution, int minCost, int index){
	if(index == n-1){
		int newMaxCost = getCost(solution);
		if(newMaxCost < maxCost){
			maxCost = newMaxCost;
			for(int j = 0; j < n; j++){
				bestSolution[j] = solution[j];
			}
		}
	}else{
		for(int i = index; i < n; i++){
			int aux = solution[index];
			solution[index] = solution[i];
			solution[i] = aux;

			vector< vector<int> > nSFM; // ot seen flow matrix
			vector< vector<int> > nSDM; // ot seen distance matrix

			long cost = 0;
			for(int j = 0; j <= index; j++){
				for(int k = 0; k <= index; k++){
					cost += flowMatrix[solution[j]-1][solution[k]-1] * distanceMatrix[j][k];
					if(cost >= maxCost){
						return;
					}
				}
			}

			for(int j = index+1; j < n; j++){
				vector<int> nSFV;
				vector<int> nSDV;

				for(int k = index+1; k < n; k++){
					nSFV.push_back(flowMatrix[solution[j]-1][solution[k]-1]);
					nSDV.push_back(distanceMatrix[j][k]);
				}

				if(nSFV.size() != 0){
					nSFM.push_back(nSFV);
				}

				if(nSDV.size() != 0){
					nSDM.push_back(nSDV);
				}
			}

			cost += gilmore_lawlerBound(nSFM, nSDM);

			// printf("Min: %li Max: %i\n", cost, maxCost);

			// printf("[");
			// for(int j = 0; j < solution.size(); j++){
			// 	printf("%i, ", solution[j]);
			// }
			// printf("]\n");

			if(cost <= maxCost){
				branchAndBound(nSFM, nSDM, solution, cost, index+1);
			}

			aux = solution[index];
			solution[index] = solution[i];
			solution[i] = aux;
		}
	}
}

int main(int argc, char* argv[]){
	readInstance(argv[1]);
	srand(time(NULL));
	
	vector<int> solution;
	for(int i = 0; i < n; i++){
		solution.push_back(i+1);
	}

	upperBound();
	branchAndBound(flowMatrix, distanceMatrix, solution, 0, 0);

	printf("Best Solution = [");
	for(int i = 0; i < bestSolution.size(); i++){
		printf("%i, ", bestSolution[i]);
	}
	printf("]\n");
	printf("Cost: %i\n", maxCost);

	return 0;
}