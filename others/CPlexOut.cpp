#include <fstream>
#include <string>
#include <iostream>
#include <new>
#include <stdlib.h>
#include <vector>
#include <set>

using namespace std;

int main(){

	ifstream ResultFile("Results.txt");	// Results from the YenKf algorithm
	string Source;
	ResultFile >> Source;
	//getline(ResultFile, Source);		// Get the filename of the data file for the YenKf algorithm
	

	ifstream GraphEdges(Source);		// Edges data for the YenKf algorithm
	int Nodes, i, j, k;
	GraphEdges >> Nodes;				// Get the number of nodes
	//Nodes = 19;

	cout << "Criando matriz...";
	
	vector<vector<double>> EdgeMatrix(Nodes);				// Matrix that will store the edges' weights
	for (i = 0; i < Nodes; i++) {
		EdgeMatrix[i] = vector<double>(Nodes);
	}
	
	for (i = 0; i < Nodes; i++) {
		for (j = 0; j < Nodes; j++) {
			EdgeMatrix[i][j] = 0;		// Fills the edge matrix with 0's
		}
	}
	
	int ori, dest;

	set <int> NodeL;					// set to store the node numbers; set chosen because it rejects repeat elements at insertion
	set <int>::iterator it;

	while (GraphEdges >> ori) {
		GraphEdges >> dest;
		NodeL.insert(ori);
		NodeL.insert(dest);

		GraphEdges.ignore(10000, '\n');
	}
	
	GraphEdges.close();

	vector<int> NodeList(NodeL.size());	// vector to be filled with info from the set; vector used for easier access with integer index
	i = 0;
	for (it = NodeL.begin(); it != NodeL.end(); it++) {
		NodeList[i] = *it;
		i++;
	}


	ifstream GraphEdges2(Source);
	GraphEdges2.ignore(10000, '\n');

	while (GraphEdges2 >> ori) {
		GraphEdges2 >> dest;


		i = 0;
		j = 0;
		while (NodeList[i] != ori) {		// Searching for the index for origin and destination nodes
			i++;			
		}

		while (NodeList[j] != dest) {
			j++;
		}

		GraphEdges2 >> EdgeMatrix[i][j];	// Fills the matrix with the edges' weights
		
	}
	GraphEdges2.close();

	/*for (i = 0; i < Nodes; i++) {						//
		for (j = 0; j < Nodes; j++) {					//
			if (i > j && EdgeMatrix[i][j] != 0) {		// Excludes the "way back" in non-oriented graphs
				EdgeMatrix[i][j] = 0;					//
			}											//
		}												//
	}*/													//

	ofstream Output;
	Output.open("CplexOut.lp", ios_base::trunc);		// Output file for Cplex
	Output << "minimize\n";

	/*ifstream Input("InputYen.txt");	// Input file for the YenKf algorithm
	Input.ignore(10000, '\n');
	Input.ignore(10000, '\n');
	int nPares, AnalysisType;
	Input >> AnalysisType;

	if (AnalysisType == 1) {
		ResultFile >> nPares;
		std::cout << nPares;
	}
	else if (AnalysisType == 2){
		Input.ignore(10000, 'n');
		Input >> nPares;				// Gets the number of origin-destination pairs analized
		std::cout << nPares;
	}*/

	int nPares, Kf;
	ResultFile >> nPares;
	ResultFile >> Kf;

	vector<vector<int>> AnalysisPairs(nPares);
	for (i = 0; i < nPares; i++) {
		AnalysisPairs[i] = vector<int>(2);
	}

	int FlowOri, FlowDest, nPaths, largestPath;

	vector<int> PairPaths(nPares);

	vector<vector<vector<double>>> matrices(nPares); // Creates an array with a matrix for each pair
	//for (auto& iPathMatrix : matrices) {
	for (k = 0; k < nPares; k++){
		
		ResultFile >> FlowOri;
		ResultFile >> FlowDest;
		AnalysisPairs[k][0] = FlowOri;
		AnalysisPairs[k][1] = FlowDest;
		ResultFile >> nPaths;
		ResultFile >> largestPath;

		PairPaths[k] = nPaths;

		//iPathMatrix = vector<vector<double>>(nPaths);			//
		matrices[k] = vector<vector<double>>(nPaths);
		for (i = 0; i < nPaths; i++) {							// Creates a matrix for each pair
			//iPathMatrix[i] = vector<double>(largestPath + 2);	//
			matrices[k][i] = vector<double>(largestPath + 2);
		}

		for (i = 0; i < nPaths; i++) {
			for (j = 0; j < largestPath + 2; j++) {
				//ResultFile >> iPathMatrix[i][j];				// Fills the matrix with the data from the Result file
				ResultFile >> matrices[k][i][j];
			}
		}
	}

	

	// Writing the first sum of the objective function

	cout << "\n\nEscrevendo funcao objetivo...";

	bool First = true;
	for (i = 0; i < Nodes; i++){
		for (j = 0; j < Nodes; j++) {
			if (EdgeMatrix[i][j] != 0 && First) {
				Output << "  x" << NodeList[i] << "_" << NodeList[j];
				//Output << "  " << EdgeMatrix[i][j] << " x" << NodeList[i] << "_" << NodeList[j];
				First = false;
			}
			else if (EdgeMatrix[i][j] != 0) {
				Output << " +  x" << NodeList[i] << "_" << NodeList[j];
				//Output << " + " << EdgeMatrix[i][j] << " x" << NodeList[i] << "_" << NodeList[j];
			}
		}
	}	

	// Writing the second sum of the objective function
	for (i = 0; i < nPares; i++) {
		if (PairPaths[i] != 0) {
			int LastPath = PairPaths[i] - 1;
			double longestPathWeight = matrices[i][LastPath][1];
			for (j = 0; j < matrices[i].size(); j++) {
				double currentPathWeight = matrices[i][j][1];
				double penalty = currentPathWeight / longestPathWeight;
				//Output << " + " << shortestPathWeight << " z" << i + 1 << "_" << j + 1;
				Output << " + " << penalty << " z" << AnalysisPairs[i][0] << "_" << AnalysisPairs[i][1] << "_" << j + 1;
			}
		}
	}

	Output << "\n\nsubject to:";

	// Restriction on amount of facilities

	int FacilityAmount;
	cout << "\n\nInsira o numero maximo de instalacoes (0 se nao houver limite): ";
	cin >> FacilityAmount;

	if (FacilityAmount != 0) {
		First = true;
		for (i = 0; i < Nodes; i++) {
			for (j = 0; j < Nodes; j++) {
				if (EdgeMatrix[i][j] != 0 && First) {
					Output << "\n  x" << NodeList[i] << "_" << NodeList[j];
					First = false;
				}
				else if (EdgeMatrix[i][j] != 0) {
					Output << " + x" << NodeList[i] << "_" << NodeList[j];
				}
			}
		}

		Output << " <= " << FacilityAmount;

		Output << "\n";
	}

	// Writing the first restriction
	cout << "\n\nEscrevendo primeira restricao...";

	for (i = 0; i < nPares; i++) {
		for (j = 0; j < matrices[i].size(); j++) {
			for (k = 2; k < matrices[i][j].size() - 1; k++) {
				if (k == 2 && matrices[i][j][k + 1] != -1) {
					Output << "\n  x" << matrices[i][j][k] << "_" << matrices[i][j][k + 1];
				}
				else if (matrices[i][j][k+1] != -1){
					Output << " + x" << matrices[i][j][k] << "_" << matrices[i][j][k + 1];
				}
			}
			Output << " - y" << AnalysisPairs[i][0] << "_" << AnalysisPairs[i][1] << "_" << j + 1 << " >= 0";
		}
	}

	Output << "\n";

	//Writing the second restriction
	cout << "\n\nEscrevendo segunda restricao...";

	for (i = 0; i < nPares; i++) {
		for (j = 0; j < matrices[i].size(); j++) {
			Output << "\n  z" << AnalysisPairs[i][0] << "_" << AnalysisPairs[i][1] << "_" << j + 1;
			Output << " + y" << AnalysisPairs[i][0] << "_" << AnalysisPairs[i][1] << "_" << j + 1 << " <= 1";
		}
	}

	Output << "\n";

	//Writing the third restriction
	cout << "\n\nEscrevendo terceira restricao...";

	for (i = 0; i < nPares; i++) {
		for (j = 0; j < matrices[i].size(); j++) {
			int cont = 0;
			for (k = 2; k < matrices[i][j].size() - 1; k++) {
				if (k == 2 && matrices[i][j][k + 1] != -1) {
					Output << "\n  x" << matrices[i][j][k] << "_" << matrices[i][j][k + 1];
					cont++;
				}
				else if (matrices[i][j][k + 1] != -1) {
					Output << " + x" << matrices[i][j][k] << "_" << matrices[i][j][k + 1];
					cont++;
				}
			}
			Output << " - " << cont << " y" << AnalysisPairs[i][0] << "_" << AnalysisPairs[i][1] << "_" << j + 1 << " <= 0";
		}
	}

	Output << "\n";

	// Writing the fourth restriction
	cout << "\n\nEscrevendo quarta restricao...";

	for (i = 0; i < nPares; i++) {
		for (j = 0; j < matrices[i].size(); j++) {
			Output << "\n  yf" << AnalysisPairs[i][0] << "_" << AnalysisPairs[i][1];
			Output << " - y" << AnalysisPairs[i][0] << "_" << AnalysisPairs[i][1] << "_" << j + 1 << " <= 0";
		}
	}

	Output << "\n";

	// Writing the fifth restriction
	cout << "\n\nEscrevendo quinta restricao...";

	for (i = 0; i < nPares; i++) {
		if (PairPaths[i] != 0) {
			for (j = 0; j < matrices[i].size(); j++) {
				if (j == 0) {
					Output << "\n  z" << AnalysisPairs[i][0] << "_" << AnalysisPairs[i][1] << "_" << j + 1;
				}
				else {
					Output << " + z" << AnalysisPairs[i][0] << "_" << AnalysisPairs[i][1] << "_" << j + 1;
				}
			}
			Output << " + yf" << AnalysisPairs[i][0] << "_" << AnalysisPairs[i][1] << " >= 1";
		}	
	}

	/////////////// BOUNDS ///////////////

	



	/*Output << "\n\nbounds";	

	for (i = 0; i < Nodes; i++) {
		for (j = 0; j < Nodes; j++) {
			if (EdgeMatrix[i][j] != 0) {
				Output << "\n  x" << i + 1 << "_" << j + 1 << " <= 1";
			}
		}
	}

	Output << "\n";

	for (i = 0; i < nPares; i++) {
		for (j = 0; j < matrices[i].size(); j++) {
			Output << "\n  y" << i + 1 << "_" << j + 1 << " <= 1";
		}
	}

	Output << "\n";

	for (i = 0; i < nPares; i++) {
		Output << "\n  yf" << i + 1 << " <= 1";	
	}

	Output << "\n";

	for (i = 0; i < nPares; i++) {
		for (j = 0; j < matrices[i].size(); j++) {
			Output << "\n  z" << i + 1 << "_" << j + 1 << " <= 1";
		}
	}*/

	/////////////// BINARIES ///////////////

	Output << "\n\nbinaries";
	cout << "\n\nDefinindo variaveis binarias";

	for (i = 0; i < Nodes; i++) {
		for (j = 0; j < Nodes; j++) {
			if (EdgeMatrix[i][j] != 0) {
				Output << "\n  x" << NodeList[i] << "_" << NodeList[j];
			}
		}
	}

	Output << "\n";

	for (i = 0; i < nPares; i++) {
		for (j = 0; j < matrices[i].size(); j++) {
			Output << "\n  y" << AnalysisPairs[i][0] << "_" << AnalysisPairs[i][1] << "_" << j + 1;
		}
	}

	Output << "\n";

	for (i = 0; i < nPares; i++) {
		if (PairPaths[i] != 0) {
			Output << "\n  yf" << AnalysisPairs[i][0] << "_" << AnalysisPairs[i][1];
		}
	}

	Output << "\n";

	for (i = 0; i < nPares; i++) {
		for (j = 0; j < matrices[i].size(); j++) {
			Output << "\n  z" << AnalysisPairs[i][0] << "_" << AnalysisPairs[i][1] << "_" << j + 1;
		}
	}

	/////////////// GENERALS ///////////////

	//Output << "\n\ngenerals\n  FacilityAmount";

	Output << "\n\nend";

	cout << "\n";
	system("pause");
	return 0;
}
