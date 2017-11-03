#include <stdio.h>
#include <time.h>
#include <fstream>
#include <vector>

using namespace std;

int n;
int maxCost;
vector<int> solution;
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

int getCost(vector<int> solution){
	int cost = 0;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			cost += flowMatrix[solution[i]-1][solution[j]-1] * distanceMatrix[i][j];
		}
	}

	return cost;
}

void generateInitialSolution(){
	for(int i = 1; i <= n; i++){
		solution.push_back(i);
	}

	for(int i = 0; i < n; i++){
		int randomIndex = i + rand()%(n-i);
		int aux = solution[i];
		solution[i] = solution[randomIndex];
		solution[randomIndex] = aux;
	}

	maxCost = getCost(solution);
}

void tabooSearch(){

}

int main(int argc, char* argv[]){
	readInstance(argv[1]);
	srand(time(NULL));

	generateInitialSolution();

	// Contar o tempo comecando aqui
	tabooSearch();
	// Parar o tempo aqui

	printf("Best Solution = [");
	//output << "Best Solution = [";
	for(int i = 0; i < solution.size(); i++){
		printf("%i, ", solution[i]);
		//output << solution[i];
	}
	printf("]\n");
	printf("Cost: %i\n", maxCost);
	// output << "Cost: " << maxCost << "\n";

	return 0;
}