#include <stdio.h>
#include <time.h>
#include <fstream>
#include <vector>

using namespace std;

int n;
int bestCost;
int objectiveAvals;
vector<int> bestSolution;
vector< vector<int> > flowMatrix;
vector< vector<int> > distanceMatrix;
vector< vector<int> > tabooList;

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
				vector<int> lists;
				for(int j = 0; j < n; j++){
					int x;
					fileData >> x;
					line.push_back(x);
					lists.push_back(0);
				}

				flowMatrix.push_back(line);
				tabooList.push_back(lists);
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

int getCost(vector<int> solution){
	int cost = 0;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			cost += flowMatrix[solution[i]-1][solution[j]-1] * distanceMatrix[i][j];
		}
	}

	return cost;
}

vector<int> generateInitialSolution(){
	vector<int> solution;
	
	for(int i = 1; i <= n; i++){
		solution.push_back(i);
	}

	for(int i = 0; i < n; i++){
		int randomIndex = i + rand()%(n-i);
		int aux = solution[i];
		solution[i] = solution[randomIndex];
		solution[randomIndex] = aux;
	}

	return solution;
}

void decrementList(){
	for (int i = 0; i < n; i++){
		for (int j = i+1; j < n; j++){
			if (tabooList[i][j] > 0){
				tabooList[i][j] -= 1;
			}
		}
	}
}

void localSearch(vector<int> &solution){
	vector<int> swapSolution;
	int cost = getCost(solution);

	for(int i = 0; i < n; i++){
		swapSolution.push_back(solution[i]);
	}

	bool flag = false;
	for(int i = 0; i < n; i++){
		for(int j = i+1; j < n; j++){
			
			if (objectiveAvals == 0){
				return;
			}

			decrementList();

			int aux = swapSolution[i];
			swapSolution[i] = swapSolution[j];
			swapSolution[j] = aux;

			int swapSolutionCost = getCost(swapSolution);
			objectiveAvals -= 1;
			

			if(tabooList[swapSolution[i]][swapSolution[j]] > 0){
				if(swapSolutionCost > bestCost){
					aux = swapSolution[i];
					swapSolution[i] = swapSolution[j];
					swapSolution[j] = aux;
				}else{
					bestSolution = swapSolution;
					solution = swapSolution;
					bestCost = swapSolutionCost;
					cost = swapSolutionCost;
				}

				continue;
			}

			if(cost > swapSolutionCost){
				flag = true;
				solution = swapSolution;
				cost = swapSolutionCost;
			}else{
				tabooList[swapSolution[j]][swapSolution[i]] = n;
				aux = swapSolution[i];
				swapSolution[i] = swapSolution[j];
				swapSolution[j] = aux;
			}
		}

		if(flag && i == n-1){
			i = -1;
			flag = false;
		}
	}
}

void perturbation(vector<int> &solution){
	for(int i = 0; i < n; i++){
		int randomIndex = i + rand()%(n-i);
		int aux = solution[i];
		solution[i] = solution[randomIndex];
		solution[randomIndex] = aux;
	}
}

void tabooSearch(){
	vector<int> newSolution = bestSolution;

	while(objectiveAvals != 0){
		localSearch(newSolution);
		perturbation(newSolution);
	}
}

int main(int argc, char* argv[]){
	readInstance(argv[1]);
	objectiveAvals = strtol(argv[2], NULL, 10);
	srand(time(NULL));

	bestSolution = generateInitialSolution();
	bestCost = getCost(bestSolution);
	printf("First Cost: %i\n", bestCost);

	// Contar o tempo comecando aqui
	tabooSearch();
	// Parar o tempo aqui

	printf("Best Solution = [");
	//output << "Best Solution = [";
	for(int i = 0; i < bestSolution.size(); i++){
		printf("%i, ", bestSolution[i]);
		//output << solution[i];
	}
	printf("]\n");
	printf("Cost: %i\n", bestCost);
	// output << "Cost: " << bestCost << "\n";

	return 0;
}