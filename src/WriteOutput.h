//////////////////////////////////////////////////////
//   Gera arquivo com os caminhos obtidos para o	//
//			par ori-dest desejado					//
//////////////////////////////////////////////////////

#include <fstream>
#include <string>
#include <iostream>
#include "GraphElements.h"
#include <new>
#include <vector>

using namespace std;

inline void Header(string filename) {
	ofstream OutputFile;
	OutputFile.open("Results.txt", ios_base::trunc);
	OutputFile << filename;
	OutputFile.close();

}

inline void CentroidsCount(int count, int nPaths) {
	ofstream OutputFile;
	OutputFile.open("Results.txt", ios_base::app);
	OutputFile << "\n" << count << "\n" << nPaths;
}

inline void CreateAuxFile(int ori, int dest){
	ofstream AuxFile;

	
	AuxFile.open("AuxFile.txt", ios_base::trunc);	// Cria o arquivo auxiliar
	AuxFile << ori << "\n" << dest;					// Escreve o par origem e destino no arquivo auxiliar
	AuxFile.close();
	
}

inline void WritePaths(BasePath* currentPath, int cont){
	ofstream AuxFile;
	AuxFile.open("AuxFile.txt", ios_base::app);
	int i;
	int length = currentPath->length();
	double weight = currentPath->Weight();

	//OutputFile << "\nCaminho " << cont << ":";

	BaseVertex* cur_node;	

	AuxFile << "\n" << length << " " << weight;

	for (i = 0; i < length; i++){
		cur_node = currentPath->GetVertex(i);
		AuxFile << " " << cur_node->getID();

	}

	AuxFile.close();

}

inline void FinishOutput(int rows, int columns) {
	
	int i, j;

	

	int ori, dest;

	ifstream InputFile("AuxFile.txt");
	ofstream OutputFile;
	OutputFile.open("Results.txt", ios_base::app);

	InputFile >> ori;
	InputFile >> dest;
	int iPath_Length;
	double iPath_Weight;
	int cur_node;

	OutputFile << "\n\n" << ori << " " << dest << "\n";
	OutputFile << rows << "\n" << columns;

	for (i = 0; i < rows; i++) {
		InputFile >> iPath_Length;
		InputFile >> iPath_Weight;
		OutputFile << "\n" << iPath_Length << " " << iPath_Weight;

		for (j = 0; j < iPath_Length; j++) {
			InputFile >> cur_node;
			OutputFile << " " << cur_node;
		}

		for (j = iPath_Length; j < columns; j++) {
			OutputFile << " -1";
		}

	}

	
}

